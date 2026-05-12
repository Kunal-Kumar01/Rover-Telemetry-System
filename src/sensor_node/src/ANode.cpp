#include "ANode.hpp"
#include <chrono>

ANode::ANode()
: Node("sensor_node")
{
  using namespace std::chrono_literals;


  publisher_ = this->create_publisher<custom_interfaces::msg::RoverTelemetry>("rover_telemetry", 10);

  timer_ = this->create_wall_timer(500ms, [this]{ sensorCallback(); });

  emergency_stop_service_ = this->create_service<custom_interfaces::srv::EmergencyStop>(
    "emergency_stop",
    std::bind(&ANode::emergencyStopCallback, this, std::placeholders::_1, std::placeholders::_2));

  RCLCPP_INFO(this->get_logger(), "Sensor node started.");
}

void ANode::sensorCallback()
{
  subsystem_.simulateReadings();

  auto message = custom_interfaces::msg::RoverTelemetry();
  message.battery_level = subsystem_.getBatteryLevel();
  message.motor_temp = subsystem_.getMotorTemp();
  message.wheel_speed = subsystem_.getWheelRPM();

  RCLCPP_INFO(this->get_logger(),
    "Publishing — Battery Level: %.1f%% | Motor Temp: %.1f°C | Wheel Speed: %.1f RPM",
    message.battery_level, message.motor_temp, message.wheel_speed);

  publisher_->publish(message);
}

void ANode::emergencyStopCallback(
  const std::shared_ptr<custom_interfaces::srv::EmergencyStop::Request> request,
  std::shared_ptr<custom_interfaces::srv::EmergencyStop::Response> response)
{
  RCLCPP_ERROR(this->get_logger(), "EMERGENCY STOP received. Reason: %s", request->reason.c_str());
  subsystem_.reset();
  response->acknowledged = true;
  RCLCPP_INFO(this->get_logger(), "System reset. Resuming nominal operations.");
}