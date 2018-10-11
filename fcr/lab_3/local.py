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


def goToPosition(coordinate_x, coordinate_y, coordinate_z):
    rospy.init_node("percepcao", anonymous=True)
    vel = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    vel_msg = Twist()
    position = rospy.Subscriber('/pose', Odometry, odomCallback)
    front_sonar = rospy.Subscriber('/sonar_front', LaserScan, frontSonarCallback)
    back_sonar = rospy.Subscriber('/sonar_back', LaserScan, backSonarCallback)
    hokuyo_scan = rospy.Subscriber('/hokuyo_scan', LaserScan, hokuyoScanCallback)

    global x, y, z, front 

    xy_goal_tolerance = 0.1
    yaw_goal_tolerance = 0.1
    coordinate_z = math.radians(coordinate_z)
    
    if coordinate_z > 3.14 :
        aux = coordinate_z  - pi
        coordinate_z = -pi + aux

    flag = True

    rate = rospy.Rate(1)
    while not rospy.is_shutdown():
        if abs(x - coordinate_x) > xy_goal_tolerance :
            flag = True
            if coordinate_x > x:
                while abs(z - 0) > yaw_goal_tolerance :
                    vel_msg.linear.x = 0;
                    vel_msg.angular.z = 1;
                    vel.publish(vel_msg)
            elif coordinate_x < x :
                while abs(z - pi) > yaw_goal_tolerance :
                    vel_msg.linear.x = 0
                    vel_msg.angular.z = 1
                    vel.publish(vel_msg)
            while abs(x - coordinate_x) > xy_goal_tolerance :
                vel_msg.linear.x = 1
                vel_msg.angular.z = 0
                vel.publish(vel_msg)
        if abs(y - coordinate_y) > xy_goal_tolerance :
            flag = True
            if coordinate_y > y :
                while abs(z - pi/2) > yaw_goal_tolerance :
                    vel_msg.linear.x = 0
                    vel_msg.angular.z = 1
                    vel.publish(vel_msg)
            elif coordinate_y < y :
                while abs(z + pi/2) > yaw_goal_tolerance :
                    vel_msg.linear.x = 0
                    vel_msg.angular.z = 1
                    vel.publish(vel_msg)
            while abs(y - coordinate_y) > xy_goal_tolerance :
                vel_msg.linear.x = 1
                vel_msg.angular.z = 0
                vel.publish(vel_msg)
        while abs(z - coordinate_z) > yaw_goal_tolerance and flag:
            vel_msg.linear.x = 0
            vel_msg.angular.z = 1
            vel.publish(vel_msg)
            if abs(z - coordinate_z) < yaw_goal_tolerance :
                flag = False
        
        rate.sleep()
        


if __name__ == '__main__':
    try:
        goToPosition(-2, 8, 270) # in degrees
    except rospy.ROSInterruptException:
        print("algo deu ruim hein")