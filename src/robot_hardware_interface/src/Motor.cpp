#include "Motor.h"
#include <pigpio.h>
#include <ros/console.h>

void Motor::initParams(ros::NodeHandle& n, const std::string &motorName, int queueSize)
{
    name = motorName;
    n.param<int>("/gpio/" + name + "/pins/direction", pinDirection, 0);
    n.param<int>("/gpio/" + name + "/pins/direction_not", pinDirection2, 0);
    n.param<int>("/gpio/" + name + "/pins/pwm", pinPWM, 0);
    n.param<int>("/gpio/" + name + "/pins/power", pinPower, 0);
    n.param<bool>("/gpio/" + name + "/reverse", reverse, false);
    n.param<std::string>("/gpio/" + name + "/topic", topic, name);
    ROS_INFO_STREAM("Initialized motor:\n"
        << name << ":\n"
        << "    type: motor\n"
        << "    topic: " << topic << "\n"
        << "    pins:" << "\n"
        << "        direction: " << pinDirection << "\n"
        << "        direction_not: " << pinDirection2 << "\n"
        << "        pwm: " << pinPWM << "\n"
        << "        power: " << topic << "\n"
    );
    subscriber = n.subscribe(
        topic
        , queueSize
        , &Motor::command
        , this
    );
}

void initOutputPin(const int pin)
{
    if (pin)
    {
        gpioSetMode(pin, PI_OUTPUT);
        gpioWrite(pin, 0);
    }
}

void Motor::initHardware()
{
    initOutputPin(pinDirection);
    initOutputPin(pinDirection2);
    initOutputPin(pinPWM);
    initOutputPin(pinPower);
}

void writeIf(const int pin, const uint8_t value)
{
    if (pin)
    {
        gpioWrite(pin, value);
    }
}

void pwmIf(const int pin, const uint8_t pwm)
{
    if (pin)
    {
        gpioPWM(pin, pwm);
    }
}

uint8_t getDirection(const float velocity)
{
    return velocity > 0;
}

uint8_t getPWM(const float velocity)
{
    return static_cast<uint8_t>(fabs(velocity));
}

uint8_t getPower(const float velocity)
{
    return velocity != 0.0;
}

void Motor::command(const std_msgs::Float32::ConstPtr& msg)
{
    const float velocity = msg->data;
    const uint8_t direction = reverse ^ getDirection(velocity);
    writeIf(pinDirection, direction);
    writeIf(pinDirection2, !direction);
    pwmIf(pinPWM, getPWM(velocity));
    writeIf(pinPower, getPower(velocity));
}
