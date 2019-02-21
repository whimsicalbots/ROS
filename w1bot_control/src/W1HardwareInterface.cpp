#include "W1HardwareInterface.h"

namespace w1_ros_ramps
{

    W1HardwareInterface::W1HardwareInterface() {
        ROS_INFO_STREAM("in costructer");
        registerInterface(&jnt_state_interface);
        registerInterface(&jnt_vel_interface);

        pos = new double[2];
        vel = new double[2];
        eff = new double[2];
        cmd = new double[2];
        last_cmd = new double[2];

        for(int i = 0; i < 2; i++) {
            pos[i] = 0;
            vel[i] = 0;
            eff[i] = 0;
            cmd[i] = 0;
            last_cmd[i] = NAN;

        }
    //    std::string joint_name = "left_wheel_joint";
        hardware_interface::JointStateHandle state_handle_l("left_wheel_joint", &pos[0], &vel[0], &eff[0]);
        jnt_state_interface.registerHandle(state_handle_l);

        hardware_interface::JointHandle joint_handle_vel_l(jnt_state_interface.getHandle("left_wheel_joint"), &cmd[0]);
        jnt_vel_interface.registerHandle(joint_handle_vel_l);

        hardware_interface::JointStateHandle state_handle_r("right_wheel_joint", &pos[1], &vel[1], &eff[1]);
        jnt_state_interface.registerHandle(state_handle_r);

        hardware_interface::JointHandle joint_handle_vel_r(jnt_state_interface.getHandle("right_wheel_joint"), &cmd[1]);
        jnt_vel_interface.registerHandle(joint_handle_vel_r);

        ROS_INFO_STREAM("done in costructer");
    }

    W1HardwareInterface::~W1HardwareInterface() {
    }

    void W1HardwareInterface::write() {
        bool changed_cmd = false;
        for (int i  = 0; i < 2; i++) {
            if(last_cmd[i] != cmd[i]) {
                changed_cmd = true;
                last_cmd[i] = cmd[i];
            }
        }
        if(changed_cmd) {
            ROS_INFO_STREAM("w cmd 0: " << cmd[0] << ", cmd 1: "<< cmd[1]);
        }
        
    }

    void W1HardwareInterface::read() {
        //ROS_INFO_STREAM("R cmd 0: " << cmd[0] << ", pos 0: "<< pos[0]<< ", vel 0: "<<vel[0]<< ", eff 0: "<<eff[0]);
        //ROS_INFO_STREAM("R cmd 1: " << cmd[1] << ", pos 1: "<< pos[1]<< ", vel 1: "<<vel[1]<< ", eff 1: "<<eff[1]);
    }

}