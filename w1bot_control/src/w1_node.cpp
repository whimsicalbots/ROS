#include <iostream>
#include <ros/ros.h>
#include <ros/master.h>
#include <controller_manager/controller_manager.h>
#include <ros/callback_queue.h>

#include "W1HardwareInterface.h"


int main(int argc, char **argv) {
    ros::init(argc, argv, "w1_node");
    ros::Time::init();
    ros::NodeHandle nh("~");
    ros::NodeHandle n;

    ros::CallbackQueue queue;
    nh.setCallbackQueue(&queue);
    n.setCallbackQueue(&queue);

    ros::AsyncSpinner spinner(4, &queue);
    spinner.start();

    ROS_INFO("INIT w1 node");

    int rate = 10;
    nh.getParam("rate", rate);
    ROS_INFO_STREAM("Rate: "<<rate);
    ros::Rate loop_rate(rate);    

    w1_ros_ramps::W1HardwareInterface robot(nh);
    
    controller_manager::ControllerManager cm(&robot, n);    
    ros::Time ts = ros::Time::now();

    while(ros::ok) {
        ros::Duration d = ros::Time::now() - ts;
        robot.read();
        ts = ros::Time::now();
        cm.update(ts, d);
        robot.write();
        loop_rate.sleep();
    }

    spinner.stop();
    return 0;
}