#!/usr/bin/env python

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

front = ()
back = ()
laser = ()

x = 0.0
y = 0.0
z = 0.0

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
    # print(front)

def backSonarCallback(data):
    global back
    back = data.ranges

def hokuyoScanCallback(data):
    global laser
    laser = data.ranges

def getAbsolutePositionOfObstacle():
    minRange = 30
    minAngle = 0
    angle = 0
    position = {"x" : 0, "y" : 0}
    for r in laser:
        if r < minRange:
            minRange = r
            minAngle = angle
        angle += 1
    position["x"] = x + minRange*math.cos(math.radians(135-minAngle))
    position["y"] = y + minRange*math.sin(math.radians(135-minAngle))
    return position

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
                


def goToPosition():
    rospy.init_node("percepcao", anonymous=True)
    vel = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    vel_msg = Twist()
    position = rospy.Subscriber('/base_pose_ground_truth', Odometry, odomCallback)
    front_sonar = rospy.Subscriber('/sonar_front', LaserScan, frontSonarCallback)
    back_sonar = rospy.Subscriber('/sonar_back', LaserScan, backSonarCallback)
    hokuyo_scan = rospy.Subscriber('/hokuyo_scan', LaserScan, hokuyoScanCallback)

    global x, y, z, front

    xy_goal_tolerance = 0.2
    yaw_goal_tolerance = 0.1
    z_goal = 0
    z_goal = math.radians(z_goal)

    if z_goal > 3.14 :
        aux = z_goal  - pi
        z_goal = -pi + aux

    go = True

    while len(front) < 1:
        pass

    map_file = open("mapa.txt", "r")
    configs_file = open("config.txt", "r")
    
    x = []
    aux = []
    aux2 = []
    adj_list = []
    coord_list = []

    for line in map_file.readlines():
        x = (line.split(" "))
        aux = x[1].split("|")
        for i in aux:
            aux2.append(int(i))
        adj_list.append(aux2)
        aux2 = []

    for line in configs_file.readlines():
        x = (line.split(" "))
        aux = x[1].split(",")
        for i in aux:
            aux2.append(float(i))
        coord_list.append(aux2)
        aux2 = []


    rate = rospy.Rate(20)
    back = 0
    goal = 0
    while not rospy.is_shutdown() and goal is not -1:
        goal = int(input("Informe a posicao do mapa que deseja ir, de acordo com config.txt\n"))
        s = bfs(back, goal, adj_list)
        path = []
        b = goal
        path.append(goal)
        while(b != -1):
            path.append(b)
            b = s[b]
        path.reverse()
        back = goal
        print("Nodes do grafo a serem percorridos: " + str(path))
        for i in path:
            if i is not -1: 
                while abs(x - coord_list[i][0]) > xy_goal_tolerance or abs(y - coord_list[i][1]) > xy_goal_tolerance :
                    while abs(z - turn_position(x, y, coord_list[i][0], coord_list[i][1])) > yaw_goal_tolerance :
                        vel_msg.linear.x = 0
                        vel_msg.angular.z = 1
                        vel.publish(vel_msg)
                    vel_msg.linear.x = 1
                    vel_msg.angular.z = 0
                    vel.publish(vel_msg)

        rate.sleep()



if __name__ == '__main__':
    try:
        goToPosition() # in degrees
    except rospy.ROSInterruptException:
        print("algo deu ruim hein")
