#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy, sys
import moveit_commander
from control_msgs.msg import GripperCommand

class MoveItFkDemo:
    def __init__(self):
        # 初始化move_group的API
        moveit_commander.roscpp_initialize(sys.argv)

        # 初始化ROS节点
        rospy.init_node('moveit_fk_demo', anonymous=True)
 
        # 初始化需要使用move group控制的机械臂中的arm group
        armrobot = moveit_commander.MoveGroupCommander('arm_robot')
        
        
        # 设置机械臂和夹爪的允许误差值
        armrobot.set_goal_joint_tolerance(0.001)

        
        # 控制机械臂先回到初始化位置
        armrobot.set_named_target('arm_home')
        armrobot.go()
        rospy.sleep(0.001)

        
         
        # 设置机械臂的目标位置，使用六轴的位置数据进行描述（单位：弧度）
        armrobot.set_named_target('arm_up')
        armrobot.go()
        rospy.sleep(0.001)

        armrobot.set_named_target('arm_down')
        armrobot.go()
        rospy.sleep(0.001)

        armrobot.set_named_target('arm_left')
        armrobot.go()
        rospy.sleep(0.001)

        armrobot.set_named_target('arm_right')
        armrobot.go()
        rospy.sleep(0.001)

        armrobot.set_named_target('arm_dance_left')
        armrobot.go()
        rospy.sleep(0.001)

        armrobot.set_named_target('arm_dance_right')
        armrobot.go()
        rospy.sleep(0.001)

        
        # 控制机械臂先回到初始化位置
        armrobot.set_named_target('arm_home')
        armrobot.go()
        rospy.sleep(0.001)

        
        # 关闭并退出moveit
        moveit_commander.roscpp_shutdown()
        moveit_commander.os._exit(0)

if __name__ == "__main__":
    try:
        MoveItFkDemo()
    except rospy.ROSInterruptException:
        pass
