# ROS
ROS related things
## Install

### Build ROS packages
Assuming you have ROS installed...
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
rm -r ~/Arduino/libraries/ros_lib    #necessary so the next script can creat them
rosrun rosserial_arduino make_libraries.py ~/Arduino/libraries
cd ~/catkin_ws/src/ROS/w1bot_control/platformio/ramps
mkdir lib
cd lib
ln -s ~/Arduino/libraries/ros_lib/ ros_lib
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

    

Plug RAMP board into USB. 
`sudo chmod a+rw /dev/ttyUSB0 `

//TODO going to setup in CMakeList for project.  For now...

```
cd ~/catkin_ws/src/ROS/w1bot_control/platformio/ramps
pio run
```
