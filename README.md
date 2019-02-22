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
cd ~/catkin_ws/src/ROS/w1bot_control/platformio/ramps
mkdir lib
cd lib
ln -s ~/Arduino/libraries/ros_lib/ ros_lib
```
also install AccelStepper lib in similar way.

Plug RAMP board into USB. 

//TODO going to setup in CMakeList for project.  For now...

```
cd ~/catkin_ws/src/ROS/w1bot_control/platformio/ramps
pio run
```
