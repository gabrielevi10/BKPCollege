#!/usr/bin/env python

import rospy
import tf
from std_msgs.msg import Float32
from nav_msgs.msg import Odometry
import geometry_msgs
from geometry_msgs.msg import Twist
from math import pi

# Cinematica Inversa pelo stage

x = 0.0
y = 0.0
z = 0.0

def walk_circle():
    rospy.init_node("inversa", anonymous=True)
    vel = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    rate = rospy.Rate(1)
    vel_msg = Twist()
    while not rospy.is_shutdown():
        vel_msg.linear.x = 0.2
        vel_msg.angular.z = 0.5  
        vel.publish(vel_msg)
         

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

def walk_square():
    rospy.init_node("inversa", anonymous=True)
    vel = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    vel_msg = Twist()
    position = rospy.Subscriber('/pose', Odometry, odomCallback)
    global x, y, z 
    count = 0
    current_angle = 0
    rate = rospy.Rate(1)
    def stop():
        k = 0
        rospy.loginfo("parando")
        while k < 10:
            vel_msg.linear.x = 0
            vel_msg.angular.z = 0
            vel.publish(vel_msg)
            k += 1
    flag = True
    while not rospy.is_shutdown() and flag:
        while z > -pi/2.0:
            vel_msg.linear.x = 0
            vel_msg.angular.z = -0.3
            vel.publish(vel_msg)
        stop()
        while y > -1:
            vel_msg.linear.x = 1
            vel_msg.angular.z = 0
            vel.publish(vel_msg)
        stop()
        while z > -pi and z <= -pi/2.0:
            vel_msg.linear.x = 0
            vel_msg.angular.z = -0.3
            vel.publish(vel_msg)
        stop()
        while x > -1:
            vel_msg.linear.x = 1
            vel_msg.angular.z = 0
            vel.publish(vel_msg)
        stop()
        while z > pi/2:
            vel_msg.linear.x = 0
            vel_msg.angular.z = -0.3
            vel.publish(vel_msg)
        stop()
        while y < 0:
            vel_msg.linear.x = 1
            vel_msg.angular.z = 0
            vel.publish(vel_msg)
        stop()
        while z > 0:
            vel_msg.linear.x = 0
            vel_msg.angular.z = -0.3
            vel.publish(vel_msg)
        stop()
        while x < 0:
            vel_msg.linear.x = 1
            vel_msg.angular.z = 0
            vel.publish(vel_msg)
        stop()
        flag = False
        
        rate.sleep()
         
        

if __name__ == '__main__':
    try:
        opt = input("1 - Circle\n2 - Square \n")
        if opt == 1:
            walk_circle()
        else:
            walk_square()
    except rospy.ROSInterruptException:
        print("algo deu ruim hein")