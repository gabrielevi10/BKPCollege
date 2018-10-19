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

def goToPosition(x_goal, y_goal, z_goal):
    rospy.init_node("percepcao", anonymous=True)
    vel = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    vel_msg = Twist()
    position = rospy.Subscriber('/base_pose_ground_truth', Odometry, odomCallback)
    front_sonar = rospy.Subscriber('/sonar_front', LaserScan, frontSonarCallback)
    back_sonar = rospy.Subscriber('/sonar_back', LaserScan, backSonarCallback)
    hokuyo_scan = rospy.Subscriber('/hokuyo_scan', LaserScan, hokuyoScanCallback)

    global x, y, z, front 

    xy_goal_tolerance = 0.1
    yaw_goal_tolerance = 0.1
    z_goal = math.radians(z_goal)
    
    if z_goal > 3.14 :
        aux = z_goal  - pi
        z_goal = -pi + aux

    go = True

    while len(front) < 1:
        pass

    rate = rospy.Rate(20)
    while not rospy.is_shutdown():
        if abs(x - x_goal) > xy_goal_tolerance or abs(y - y_goal) > xy_goal_tolerance :
            right = front[0] + front[1] + front[2] + front[3] + front[4]
            left = front[9] + front[8] + front[7] + front[6] + front[5]
            while abs(z - turn_position(x , y, x_goal, y_goal)) > yaw_goal_tolerance :
                vel_msg.linear.x = 0
                vel_msg.angular.z = -1
                vel.publish(vel_msg)
    
            while front[3] < 2 or front[4] < 2 or front[5] < 2 or front[6] < 2 :
                vel_msg.linear.x = 0
                vel_msg.angular.z = 1
                vel.publish(vel_msg)

            while front[4] > 2 and front[5] > 2:
                if front[2] >= 5 and front[3] >= 5 and front[4] >= 5 and front[5] >= 5 and front[6] >= 5 and front[7] >= 5:
                    while abs(z - turn_position(x , y, x_goal, y_goal)) > yaw_goal_tolerance:
                        vel_msg.linear.x = 0
                        vel_msg.angular.z = -1
                        vel.publish(vel_msg)
                vel_msg.linear.x = 1
                vel_msg.angular.z = 0
                vel.publish(vel_msg)
        else:
            while abs(z - z_goal) > yaw_goal_tolerance:
                vel_msg.linear.x = 0
                vel_msg.angular.z = -1
                vel.publish(vel_msg)

        rate.sleep()
        


if __name__ == '__main__':
    try:
        goToPosition(1, 1, 270) # in degrees
    except rospy.ROSInterruptException:
        print("algo deu ruim hein")
