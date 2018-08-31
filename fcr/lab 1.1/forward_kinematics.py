#!/usr/bin/env python

import rospy
import tf
from std_msgs.msg import Float32
from nav_msgs.msg import Odometry
import geometry_msgs
from math import pi

# Cinematica Direta pelo gazebo (nao tem paredes :D )

x = 0.0
y = 0.0
z = 0.0

def walk_circle():
    rospy.init_node("direta", anonymous=True)
    right_wheel = rospy.Publisher('/v_right', Float32, queue_size=10)
    left_wheel = rospy.Publisher('/v_left', Float32, queue_size=10)
    rate = rospy.Rate(1)
    while not rospy.is_shutdown():
        right_wheel.publish(2.0)
        left_wheel.publish(1.0)
         

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
    rospy.init_node("direta", anonymous=True)
    right_wheel = rospy.Publisher('/v_right', Float32, queue_size=1)
    left_wheel = rospy.Publisher('/v_left', Float32, queue_size=1)
    position = rospy.Subscriber('/pose', Odometry, odomCallback)
    global x, y, z 
    count = 0
    rate = rospy.Rate(1)
    def stop():
        k = 0
        rospy.loginfo("parando")
        while k < 1000:
            right_wheel.publish(0.0)
            left_wheel.publish(0.0)
            k += 1

    # andando em frente
    while not rospy.is_shutdown():
        rospy.loginfo("andando em frente(1)")
        while x <= 1.0 and count == 0:
            right_wheel.publish(0.5)
            left_wheel.publish(0.5)

        count += 1
        stop()
        rospy.loginfo("virando pra esquerda(1)")
        while z <= pi/2.0 and count == 1:
            right_wheel.publish(0.1)
            left_wheel.publish(-0.1)

        stop()
        count += 1
        rospy.loginfo("andando em frente(2)")
        while y <= 1.0 and count == 2:
            right_wheel.publish(0.5)
            left_wheel.publish(0.5)

        count += 1
        stop()
        rospy.loginfo("virando pra esquerda(2)")
        while z <= pi and z >= pi/2.0 and count == 3:
            right_wheel.publish(0.1)
            left_wheel.publish(-0.1)
        
        count += 1
        stop()
        rospy.loginfo("andando em frente(3)")
        while x >= 0 and count == 4:
            right_wheel.publish(0.5)
            left_wheel.publish(0.5)
        
        count += 1
        stop()
        rospy.loginfo("virando pra esquerda(3)")
        while z >= pi or z <= -pi/2  and count == 5:
            right_wheel.publish(0.1)
            left_wheel.publish(-0.1)

        count += 1
        stop()
        rospy.loginfo("andando em frente(4)")
        while y >= 0 and count == 6:
            right_wheel.publish(0.5)
            left_wheel.publish(0.5)

        count += 1
        stop()
        rospy.loginfo("virando pra esquerda(4)")
        while z < 0 and z >= -pi and count == 7:
            right_wheel.publish(0.1)
            left_wheel.publish(-0.1)

        stop()
        rospy.spin()
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
