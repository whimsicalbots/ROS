#include "ros.h"
#include <std_srvs/SetBool.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <w1bot_control/MotorSpeed.h>
#include "pins.h"
#include <Servo.h>
#include "FastLED.h"


ros::NodeHandle nh;

AccelStepper stepper_X(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepper_Y(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper stepper_Z(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);
AccelStepper stepper_E0(AccelStepper::DRIVER, E0_STEP_PIN, E0_DIR_PIN);
AccelStepper stepper_E1(AccelStepper::DRIVER, E1_STEP_PIN, E1_DIR_PIN);

Servo servo[MAX_SERVOS]; 

MultiStepper multistepper;

CRGBArray<NUM_LEDS> leds;

class MotorHandler
{
public:
  MotorHandler(byte pin, float period)
  : pin_(pin), period_(period),
    subscriber_("set_motor_speed", &MotorHandler::set_period_callback, this),
    subsrciber_servo_("set_servo", &MotorHandler::set_servo_callback, this),
    service_server_("activate_blinker", &MotorHandler::service_callback, this)
  {}

  void init(ros::NodeHandle& nh)
  {
    pinMode(pin_, OUTPUT);
    nh.subscribe(subscriber_);
    nh.subscribe(subsrciber_servo_);
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
    if(msg.speed != 0.0) {
      multistepper.getStepper(msg.motor)->enableOutputs();
      multistepper.getStepper(msg.motor)->setSpeed(msg.speed);
    } else {
      multistepper.getStepper(msg.motor)->disableOutputs();
    }    
  }

  void set_servo_callback(const w1bot_control::MotorSpeed& msg) {
    servo[msg.motor].write(msg.speed);
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
  ros::Subscriber<w1bot_control::MotorSpeed, MotorHandler> subscriber_;
  ros::Subscriber<w1bot_control::MotorSpeed, MotorHandler> subsrciber_servo_;
  ros::ServiceServer<std_srvs::SetBool::Request, std_srvs::SetBool::Response, MotorHandler> service_server_;
};

MotorHandler motorHandler(LED_BUILTIN, 500);

void setup()
{
  stepper_X.setAcceleration(1000);
  stepper_X.setMaxSpeed(1000);
  stepper_X.setEnablePin(X_ENABLE_PIN);
  stepper_X.setPinsInverted(false, false, true);
  
  stepper_Y.setAcceleration(1000);
  stepper_Y.setMaxSpeed(1000);
  stepper_Y.setEnablePin(Y_ENABLE_PIN);
  stepper_Y.setPinsInverted(true, false, true);  // invert direction so diff drive works //TODO should this be done differently?
  
  stepper_Z.setAcceleration(1000);
  stepper_Z.setMaxSpeed(1000);
  stepper_Z.setEnablePin(Z_ENABLE_PIN);
  stepper_Z.setPinsInverted(false, false, true);
  
  stepper_E0.setAcceleration(1000);
  stepper_E0.setMaxSpeed(1000);
  stepper_E0.setEnablePin(E0_ENABLE_PIN);
  stepper_E0.setPinsInverted(false, false, true);
  
  stepper_E1.setAcceleration(1000);
  stepper_E1.setMaxSpeed(1000);
  stepper_E1.setEnablePin(E1_ENABLE_PIN);
  stepper_E1.setPinsInverted(false, false, true);
  
  multistepper.addStepper(stepper_X);
  multistepper.addStepper(stepper_Y);
  multistepper.addStepper(stepper_Z);
  multistepper.addStepper(stepper_E0);
  multistepper.addStepper(stepper_E1);

  nh.initNode();
  motorHandler.init(nh);

  servo[0].attach(SERVO0_PIN);
  servo[1].attach(SERVO1_PIN);
  servo[2].attach(SERVO2_PIN);

  FastLED.addLeds<NEOPIXEL, WS2812_PIN>(leds, NUM_LEDS);
  
}

void loop()
{
  for (int i = 0; i < 5; i++) {
    multistepper.getStepper(i)->runSpeed();
  }
  motorHandler.run();
  nh.spinOnce();
  delay(1);
  leds[0] = CRGB::White;
  static uint8_t hue;
  for(int i = 0; i < NUM_LEDS/2; i++) {
    leds.fadeToBlackBy(40);
    leds[i] = CHSV(hue++, 255,255);

    leds(NUM_LEDS/2, NUM_LEDS -1) =leds(NUM_LEDS/2 -1, 0);
    FastLED.delay(33);
  }
  FastLED.show();
}