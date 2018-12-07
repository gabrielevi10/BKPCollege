#!/usr/bin/env python

from __future__ import print_function
import rospy
import tf
from std_msgs.msg import Float32
from nav_msgs.msg import Odometry
from sensor_msgs.msg import LaserScan
import geometry_msgs
from geometry_msgs.msg import Twist
import math
from math import pi
import time
from collections import deque
import sys

front = ()
back = ()
laser = ()

x = 0.0
y = 0.0
z = 0.0

xy_goal_tolerance = 0.2
yaw_goal_tolerance = 0.1

def odomCallback(data):
    global x, y, z
    x = data.pose.pose.position.x
    y = data.pose.pose.position.y
    quaternion = (
        data.pose.pose.orientation.x,
        data.pose.pose.orientation.y,
        data.pose.pose.orientation.z,
        data.pose.pose.orientation.w)
    euler = tf.transformations.euler_from_quaternion(quaternion)
    roll = euler[0]
    pitch = euler[1]
    yaw = euler[2]
    z = yaw


def frontSonarCallback(data):
    global front
    front = data.ranges


def backSonarCallback(data):
    global back
    back = data.ranges


def hokuyoScanCallback(data):
    global laser
    laser = data.ranges


def getAbsolutePositionOfObstacle(minRange, minAngle):
    angle = z
    position = {'x' : 0, 'y' : 0}
    position['x'] = minRange*math.cos(math.radians(135-minAngle) - angle)
    position['y'] = minRange*math.sin(math.radians(135-minAngle) - angle)
    return position                


def round_of_rating(number):
    return round(number * 2) / 2
    

def make_grade(dist, angle, grade, myPositionX, myPositionY):
    obstaclePosition = getAbsolutePositionOfObstacle(dist, angle)
    col = int(math.floor(obstaclePosition['x']/0.5) + myPositionX)
    line =  int(math.floor(obstaclePosition['y']/0.5) + myPositionY)
    try:
        if grade[line][col] < 10 :
            grade[line][col] += 1
    except IndexError:
        print("fora da area de mapeamento")
    make_empty_cell(dist-0.5, angle, grade, myPositionX, myPositionY)


def make_varredure(vel_msg, vel, grade, myPositionX, myPositionY):
    global z, yaw_goal_tolerance
    z_list = [0.0, pi/2.0, pi, -pi/2.0]

    for q in z_list:
        while abs(z - q) > yaw_goal_tolerance:
            vel_msg.linear.x = 0
            vel_msg.angular.z = 1
            vel.publish(vel_msg)

        for k in range(270):
            if laser[k] < 10:
                make_grade(round_of_rating(laser[k]), k, grade, myPositionX, myPositionY)
            else:
                make_empty_cell(round_of_rating(laser[k]), k, grade, myPositionX, myPositionY)


def turn_position(pos_x, pos_y, x_goal, y_goal):
    return math.atan2(y_goal - pos_y, x_goal - pos_x)

def bfs(root_node, goal_node, adj_list):
    visited = []
    p = []
    for i in range(56):
        p.append(-1)
    f = deque([])
    f.append(root_node)
    visited.append(root_node)
    while len(f) > 0:
        v = f.popleft()
        for adj in adj_list[v]:
            if adj not in visited:
                visited.append(adj)
                f.append(adj)
                p[adj] = v
    return p
    

def make_empty_cell(dist, angle, grade, myPositionX, myPositionY):
    if dist < 0.5:
        return
    empty_position = getAbsolutePositionOfObstacle(dist, angle)
    col = int(math.floor(empty_position['x']/0.5) + myPositionX)
    line =  int(math.floor(empty_position['y']/0.5) + myPositionY)
    try:
        if grade[line][col] > 0:
            grade[line][col] -= 1
    except IndexError:
        print("fora da area de mapeamento")
        # sys.exit()
    make_empty_cell(dist-0.5, angle, grade, myPositionX, myPositionY)

def definePath(back, goal, adj_list):
    s = bfs(back, goal, adj_list)
    path = []
    b = goal
    path.append(-1)
    while(b != -1):
        path.append(b)
        b = s[b]
    path.reverse()
    return path

def goToPosition(x_goal, y_goal, vel_msg, vel):
    global yaw_goal_tolerance, xy_goal_tolerance
    while abs(x - x_goal) > xy_goal_tolerance or abs(y - y_goal) > xy_goal_tolerance :
        while abs(z - turn_position(x, y, x_goal, y_goal)) > yaw_goal_tolerance:
            vel_msg.linear.x = 0
            vel_msg.angular.z = 1
            vel.publish(vel_msg)
        vel_msg.linear.x = 1
        vel_msg.angular.z = 0
        vel.publish(vel_msg)

def OcupeTheGrade():
    rospy.init_node("grade", anonymous=True)
    vel = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    vel_msg = Twist()
    position = rospy.Subscriber('/base_pose_ground_truth', Odometry, odomCallback)
    front_sonar = rospy.Subscriber('/sonar_front', LaserScan, frontSonarCallback)
    back_sonar = rospy.Subscriber('/sonar_back', LaserScan, backSonarCallback)
    hokuyo_scan = rospy.Subscriber('/hokuyo_scan', LaserScan, hokuyoScanCallback)

    global x,y,z,yaw_goal_tolerance,xy_goal_tolerance

    while len(laser) < 1:
        pass

    map_file = open("mapa.txt", "r")
    configs_file = open("config.txt", "r")
    
    r = []
    aux = []
    aux2 = []
    adj_list = []
    coord_list = []
    
    # carrega o grafo em memoria
    for line in map_file.readlines():
        r = (line.split(" "))
        aux = r[1].split("|")
        for i in aux:
            aux2.append(int(i))
        adj_list.append(aux2)
        aux2 = []

    # coordenadas que cada node do grafo corresponde
    for line in configs_file.readlines():
        r = (line.split(" "))
        aux = r[1].split(",")
        for i in aux:
            aux2.append(float(i))
        coord_list.append(aux2)
        aux2 = []

    rate = rospy.Rate(20)
    l = 0.5
    last = 0
    grade =[[]]
    ok = False
    myPositionX = int(10/l)
    myPositionY = int(10/l)
    list_of_grades = [None] * 56

    while not rospy.is_shutdown():
        goal = 10
        path = definePath(last, goal, adj_list)
        last = goal
        print("Nodes do grafo a serem percorridos: " + str(path))
        for i in path:
            if i is not -1: 
                if i is not -1:
                    goToPosition(coord_list[i][0], coord_list[i][1], vel_msg, vel)
            else:
                ok = True
            print("Node atual:" + str(i))
            grade = [[5 for o in range(int(20/l*2))] for u in range(int(20/l*2))]
            for q in range(1):
                make_varredure(vel_msg, vel, grade, myPositionX, myPositionY)
            
            grade[myPositionX][myPositionY] = 0

            output = open("grade/node" + str(i) + ".txt", "w");

            for a in range(int(10/l*2)) :
                for b in range(int(10/l*2)) :
                    if grade[a][b] == 5:
                        output.write(".")
                        print(".", end='')
                    else:
                        if grade[a][b] == 10:
                            output.write("#")
                            print("#", end='')
                        elif grade[a][b] == 0:
                            output.write(" ")
                            print(" ", end='')
                        else: 
                            output.write(str(grade[a][b]))
                            print(str(grade[a][b]), end='')
                print()
                output.write("\n")
            list_of_grades[i] = grade
            output.close()

            while ok and goal is not -1:
                print("Coordenadas atuais: " + str(x) + " " + str(y))
                print("Informe a coordenada X e Y que deseja ir, dentro do mapa e em coordenadas globais")
                x_goal = input("X: ")
                y_goal = input("Y: ")
                diff_x = 999
                diff_y = 999
                a_x = 0
                a_y = 0
                
                for t in coord_list:
                    if abs(x_goal - t[0]) < diff_x and abs(y_goal - t[1]) < diff_y:
                        diff_x = abs(x_goal - t[0])
                        diff_y = abs(y_goal - t[1])
                        a_x = t[0]
                        a_y = t[1]
                goal = 0
                for t in coord_list:
                    if t[0] == a_x and t[1] == a_y:
                        break
                    goal += 1

                print("goal: " + str(goal))
                print("back: " + str(last))
                path = definePath(last, goal, adj_list)
                last = goal
                print(path)

                fx = x
                fy = y
                line = 0
                col = 0
                
                back_i = path[0]
                current = back_i
                for i in path:
                    back_x = x
                    back_y = y
                    if i is not -1: 
                        while abs(x - coord_list[i][0]) > xy_goal_tolerance or abs(y - coord_list[i][1]) > xy_goal_tolerance :
                            while abs(z - turn_position(x, y, coord_list[i][0], coord_list[i][1])) > yaw_goal_tolerance:
                                vel_msg.linear.x = 0
                                vel_msg.angular.z = 1
                                vel.publish(vel_msg)
                            vel_msg.linear.x = 1
                            vel_msg.angular.z = 0
                            vel.publish(vel_msg)
            
                            if abs(back_x - x) >= 0.5 or abs(back_y - y) >= 0.5:
                                if (line > 10/l*2 or col > 10/l*2) or (line < 0 or col < 0):
                                    current = i
                                    fx = coord_list[i][0]
                                    fy = coord_list[i][1]
                                
                                ax = x - fx
                                ay = fy - y
                                back_x = x
                                back_y = y
                                col = int(math.floor(ax/0.5) + myPositionX)
                                line =  int(math.floor(ay/0.5) + myPositionY)

                                # printa a grade de ocupacao com o robo andando sobre ela
                                print()
                                for a in range(int(10/l*2)) :
                                    for b in range(int(10/l*2)) :
                                        if a == line and b == col:
                                            print("R", end='')
                                        elif list_of_grades[current][a][b] == 5:
                                            print(".", end='')
                                        else:
                                            if list_of_grades[current][a][b] == 10:
                                                print("#", end='')
                                            elif list_of_grades[current][a][b] == 0:
                                                print(" ", end='')
                                            else: 
                                                print(str(list_of_grades[current][a][b]), end='')
                                    print()
                    back_i = i

        rate.sleep()



if __name__ == '__main__':
    try:
        print("""Saida no arquivo output.txt
                Legenda: # = Parede (preto)
                         R = robo ao fim da execucao
                         . = nao visto (cinza)
                         espaco em branco = desocupado (branco) 
                         Numeros de 1 a 9 = celulas pouco visualizadas """)
        OcupeTheGrade() # in degrees
    except rospy.ROSInterruptException:
        print("algo deu ruim hein")
