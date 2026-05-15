#include "MonitorNode.hpp"

using std::placeholders::_1;

MonitorNode::MonitorNode()
: Node("monitor_node")
{
  m_subscription = this->create_subscription<custom_interfaces::msg::RoverTelemetry>(
    "rover_telemetry", 10,
    std::bind(&MonitorNode::monitorCallback, this, _1));

  m_emergency_stop_client = this->create_client<custom_interfaces::srv::EmergencyStop>(
    "emergency_stop");

  RCLCPP_INFO(this->get_logger(), "Monitor node started.");
}

void MonitorNode::monitorCallback(const custom_interfaces::msg::RoverTelemetry & msg)
{
  AlertLevel level = m_subsystem.checkThresholds(
    msg.battery_level, msg.motor_temp, msg.wheel_speed);

  std::string alert = m_subsystem.formatAlert(
    level, msg.battery_level, msg.motor_temp, msg.wheel_speed);

  if (level == AlertLevel::NOMINAL) {
    RCLCPP_INFO(this->get_logger(), "%s", alert.c_str());
  } else if (level == AlertLevel::WARNING) {
    RCLCPP_WARN(this->get_logger(), "%s", alert.c_str());
  } else {
    RCLCPP_ERROR(this->get_logger(), "%s", alert.c_str());
    triggerEmergencyStop("Critical threshold breached.");
  }
}

void MonitorNode::triggerEmergencyStop(const std::string & reason)
{
  if (!m_emergency_stop_client->wait_for_service(std::chrono::seconds(1))) {
    RCLCPP_ERROR(this->get_logger(), "Emergency stop service not available.");
    return;
  }

  auto request = std::make_shared<custom_interfaces::srv::EmergencyStop::Request>();
  request->reason = reason;

  auto future = m_emergency_stop_client->async_send_request(request,
    [this](rclcpp::Client<custom_interfaces::srv::EmergencyStop>::SharedFuture response) {
      if (response.get()->acknowledged) {
        RCLCPP_INFO(this->get_logger(), "Emergency stop acknowledged by sensor node.");
      } else {
        RCLCPP_ERROR(this->get_logger(), "Emergency stop NOT acknowledged.");
      }
    });
}