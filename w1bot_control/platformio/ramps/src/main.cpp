#include <ros.h>
#include <std_srvs/SetBool.h>
#include <std_msgs/Float64.h>
#include <AccelStepper.h>
#include <w1bot_control/MotorSpeed.h>

ros::NodeHandle nh;
AccelStepper stepper_left(AccelStepper::DRIVER, 54, 55);

class Blinker
{
public:
  Blinker(byte pin, float period)
  : pin_(pin), period_(period),
    subscriber_("set_blink_period", &Blinker::set_period_callback, this),
    service_server_("activate_blinker", &Blinker::service_callback, this)
  {}

  void init(ros::NodeHandle& nh)
  {
    pinMode(pin_, OUTPUT);
    nh.subscribe(subscriber_);
    nh.advertiseService(service_server_);
  }

  void run()
  {
    if(active_ && ((millis() - last_time_) >= period_))
    {
      last_time_ = millis();
      digitalWrite(pin_, !digitalRead(pin_));
    }
  }

  void set_period_callback(const w1bot_control::MotorSpeed& msg)
  {
    period_ = msg.speed;
    stepper_left.setSpeed(period_);
  }

  void service_callback(const std_srvs::SetBool::Request& req,
                              std_srvs::SetBool::Response& res)
  {
    active_ = req.data;
    res.success = true;
    if(req.data)
      res.message = "Blinker ON";
    else
      res.message = "Blinker OFF";
  }

private:
  const byte pin_;
  bool active_ = true;
  float period_;
  uint32_t last_time_;
  ros::Subscriber<w1bot_control::MotorSpeed, Blinker> subscriber_;
  ros::ServiceServer<std_srvs::SetBool::Request, std_srvs::SetBool::Response, Blinker> service_server_;
};

Blinker blinker(LED_BUILTIN, 500);

void setup()
{
  stepper_left.setAcceleration(10000);
  stepper_left.setEnablePin(38);
  stepper_left.setPinsInverted(false, false, true);
  stepper_left.setMaxSpeed(10000);
  stepper_left.enableOutputs();
  
  nh.initNode();
  blinker.init(nh);

}

void loop()
{
  stepper_left.runSpeed();
  blinker.run();
  nh.spinOnce();
  delay(1);
}
