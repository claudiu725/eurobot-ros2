#ifndef MOTOR_H_
#define MOTOR_H_

#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <string>

class Motor
{
public:
    void initParams(ros::NodeHandle& n, const std::string &motorName, int queueSize);
    void initHardware();

private:
    void command(const std_msgs::Float32::ConstPtr& msg);
private:
    ros::Subscriber subscriber;
    int pinDirection = 0, pinDirection2 = 0, pinPWM = 0, pinPower = 0;
    bool reverse = false;
    std::string topic;
    std::string name;
};

#endif // MOTOR_H_
