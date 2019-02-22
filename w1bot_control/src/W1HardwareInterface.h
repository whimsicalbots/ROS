#include <iostream>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>
#include <w1bot_control/MotorSpeed.h>

namespace w1_ros_ramps
{
    using namespace hardware_interface;
    class W1HardwareInterface : public hardware_interface::RobotHW {

    
    private:
        ros::Publisher pub;
        hardware_interface::JointStateInterface jnt_state_interface;
        hardware_interface::VelocityJointInterface jnt_vel_interface;
        
        double *cmd;
        double *pos;
        double *vel;
        double *eff;
        double *last_cmd;
        float power_factor = 50;
        
    public:
        W1HardwareInterface(ros::NodeHandle& root_nh);
        virtual ~W1HardwareInterface();
        void write();
        void read();
        bool init();
    };
}