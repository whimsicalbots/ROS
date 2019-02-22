#include "ros.h"
#include <std_srvs/SetBool.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <w1bot_control/MotorSpeed.h>
#include "pins.h"

ros::NodeHandle nh;

AccelStepper stepper_X(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepper_Y(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper stepper_Z(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);
AccelStepper stepper_E0(AccelStepper::DRIVER, E0_STEP_PIN, E0_DIR_PIN);
AccelStepper stepper_E1(AccelStepper::DRIVER, E1_STEP_PIN, E1_DIR_PIN);

MultiStepper multistepper;

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
    multistepper.getStepper(msg.motor)->setSpeed(msg.speed);
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
  stepper_X.setAcceleration(10000);
  stepper_X.setMaxSpeed(10000);
  stepper_X.setEnablePin(X_ENABLE_PIN);
  stepper_X.setPinsInverted(false, false, true);
  stepper_X.enableOutputs();  //TODO move enable to service, so that it can be switched on and off to save energy
  
  stepper_Y.setAcceleration(10000);
  stepper_Y.setMaxSpeed(10000);
  stepper_Y.setEnablePin(Y_ENABLE_PIN);
  stepper_Y.setPinsInverted(true, false, true);  // invert direction so diff drive works //TODO should this be done differently?
  stepper_Y.enableOutputs();
  
  stepper_Z.setAcceleration(10000);
  stepper_Z.setMaxSpeed(10000);
  stepper_Z.setEnablePin(Z_ENABLE_PIN);
  stepper_Z.setPinsInverted(false, false, true);
  stepper_Z.enableOutputs();

  stepper_E0.setAcceleration(10000);
  stepper_E0.setMaxSpeed(10000);
  stepper_E0.setEnablePin(E0_ENABLE_PIN);
  stepper_E0.setPinsInverted(false, false, true);
  stepper_E0.enableOutputs();

  stepper_E1.setAcceleration(10000);
  stepper_E1.setMaxSpeed(10000);
  stepper_E1.setEnablePin(E1_ENABLE_PIN);
  stepper_E1.setPinsInverted(false, false, true);
  stepper_E1.enableOutputs();

  multistepper.addStepper(stepper_X);
  multistepper.addStepper(stepper_Y);
  multistepper.addStepper(stepper_Z);
  multistepper.addStepper(stepper_E0);
  multistepper.addStepper(stepper_E1);

  nh.initNode();
  blinker.init(nh);

}

void loop()
{
  for (int i = 0; i < 5; i++) {
    multistepper.getStepper(i)->runSpeed();
  }
  blinker.run();
  nh.spinOnce();
  delay(1);
}
