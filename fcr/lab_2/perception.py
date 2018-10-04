#!/usr/bin/env python

import rospy
import tf
from std_msgs.msg import Float32
from nav_msgs.msg import Odometry
from sensor_msgs.msg import LaserScan
import geometry_msgs
from geometry_msgs.msg import Twist
import math
import time

x = 0.0
y = 0.0
z = 0.0
front = ()
back = ()
laser = ()

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


def goAndAvoid():
    rospy.init_node("percepcao", anonymous=True)
    vel = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    vel_msg = Twist()
    position = rospy.Subscriber('/pose', Odometry, odomCallback)
    front_sonar = rospy.Subscriber('/sonar_front', LaserScan, frontSonarCallback)
    back_sonar = rospy.Subscriber('/sonar_back', LaserScan, backSonarCallback)
    hokuyo_scan = rospy.Subscriber('/hokuyo_scan', LaserScan, hokuyoScanCallback)

    global x, y, z, front 
    count = 0
    def stop():
        k = 0
        rospy.loginfo("parando")
        while k < 10:
            vel_msg.linear.x = 0
            vel_msg.angular.z = 0
            vel.publish(vel_msg)
            k += 1

    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        # getAbsolutePositionOfObstacle()
        vel_msg.linear.x = 1
        vel_msg.angular.z = 0
        vel.publish(vel_msg)

        if len(front) > 0:
            while front[3] < 1.5 or front[4] < 1.5 or front[5] < 1.5 or front[6] < 1.5:
                if front[0] < 1.5 and front[1] < 1.5 and front[2] < 1.5:
                    vel_msg.linear.x = 0
                    vel_msg.angular.z = 1
                    vel.publish(vel_msg)
                else:
                    vel_msg.linear.x = 0
                    vel_msg.angular.z = -1
                    vel.publish(vel_msg)
        
        rate.sleep()


if __name__ == '__main__':
    try:
        goAndAvoid()
    except rospy.ROSInterruptException:
        print("algo deu ruim hein")