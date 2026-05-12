#ifndef MONITOR_NODE__BNODE_HPP_
#define MONITOR_NODE__BNODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/msg/rover_telemetry.hpp"
#include "custom_interfaces/srv/emergency_stop.hpp"
#include "BSubsystem.hpp"

class BNode : public rclcpp::Node
{
public:
  BNode();

private:
  void monitorCallback(const custom_interfaces::msg::RoverTelemetry & msg);
  void triggerEmergencyStop(const std::string & reason);

  BSubsystem subsystem_;
  rclcpp::Subscription<custom_interfaces::msg::RoverTelemetry>::SharedPtr subscription_;
  rclcpp::Client<custom_interfaces::srv::EmergencyStop>::SharedPtr emergency_stop_client_;
};

#endif 