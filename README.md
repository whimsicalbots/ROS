# ROS
ROS related things
## Install

### Build ROS packages
Assuming you have ROS installed...
Install dependencies:
```
	sudo apt install ros-<distro>-hardware-interface
	sudo apt install ros-<distro>-controller-manager
	sudo apt install ros-<distro>-control-toolbox
	sudo apt install ros-<distro>-rviz
	sudo apt install ros-<distro>-xacro
	sudo apt install ros-<distro>-rosserial-arduino
	sudo apt install ros-<distro>-rosbridge-server
	sudo apt install ros-<distro>-teleop-twist-keyboard
	sudo apt install ros-<distro>-ros-control
	sudo apt install ros-<distro>-ros-controllers
	sudo apt install ros-<distro>-joy
	sudo apt install ros-<distro>-rosbridge-suite
```
Install Arduino
Install Arduino Servo library

```
cd ~/catkin_ws/src
git clone https://github.com/whimsicalbots/ROS.git
cd ..
catkin_make
```
### Install firmware
Currently using [platformio](https://platformio.org) to install firmware into Arduino.
//TODO add something to add libs to platformio project.
``` 
source ~/catkin_ws/install/setup.bash
source ~/catkin_ws/devel/setup.bash
rm -r ~/Arduino/libraries/ros_lib    #necessary so the next script can create them
rosrun rosserial_arduino make_libraries.py ~/Arduino/libraries
cd ~/catkin_ws/src/ROS/w1bot_control/platformio/ramps
mkdir lib
cd lib
ln -s ~/Arduino/libraries/ros_lib/ ros_lib
ln -s ~/Arduino/libraries/Sevro/ Servo

```
also install AccelStepper lib
`git clone https://github.com/waspinator/AccelStepper.git`

add getStepper method to MultiStepper.h

```
AccelStepper* getStepper(int num)
    {
    	if(num<_num_steppers)
    	{
    		return _steppers[num];
    	}
    	else
    	{
    		return 0;
    	}
    }

```

Add FastLED lib
`git clone https://github.com/FastLED/FastLED.git`

    

Plug RAMP board into USB. 
`sudo chmod a+rw /dev/ttyUSB0 `

//TODO going to setup in CMakeList for project.  For now...

```
cd ~/catkin_ws/src/ROS/w1bot_control/platformio/ramps
pio run
```

Launch w1bot_control_serial, this will start roscore and start listening to the ramps controller
```
roslaunch w1bot_control w1bot_control_serial.launch
```

In another terminal, source install and devel, then 
```
roslaunch w1bot_control w1bot_control_teleop.launch
```
