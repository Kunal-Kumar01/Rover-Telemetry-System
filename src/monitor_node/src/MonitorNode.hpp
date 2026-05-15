#ifndef MONITOR_NODE__MONITORNODE_HPP_
#define MONITOR_NODE__MONITORNODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/msg/rover_telemetry.hpp"
#include "custom_interfaces/srv/emergency_stop.hpp"
#include "MonitorSubsystem.hpp"

class MonitorNode : public rclcpp::Node
{
public:
  MonitorNode();

private:
  void monitorCallback(const custom_interfaces::msg::RoverTelemetry & msg);
  void triggerEmergencyStop(const std::string & reason);

  MonitorSubsystem m_subsystem;
  rclcpp::Subscription<custom_interfaces::msg::RoverTelemetry>::SharedPtr m_subscription;
  rclcpp::Client<custom_interfaces::srv::EmergencyStop>::SharedPtr m_emergency_stop_client;
};

#endif 