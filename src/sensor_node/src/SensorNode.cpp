#include "SensorNode.hpp"
#include <chrono>

SensorNode::SensorNode()
: Node("sensor_node")
{
  using namespace std::chrono_literals;


  m_publisher = this->create_publisher<custom_interfaces::msg::RoverTelemetry>("rover_telemetry", 10);

  m_timer = this->create_wall_timer(500ms, [this]{ sensorCallback(); });

  m_emergency_stop_service = this->create_service<custom_interfaces::srv::EmergencyStop>(
    "emergency_stop",
    std::bind(&SensorNode::emergencyStopCallback, this, std::placeholders::_1, std::placeholders::_2));

  RCLCPP_INFO(this->get_logger(), "Sensor node started.");
}

void SensorNode::sensorCallback()
{
  m_subsystem.simulateReadings();

  auto message = custom_interfaces::msg::RoverTelemetry();
  message.battery_level = m_subsystem.getBatteryLevel();
  message.motor_temp = m_subsystem.getMotorTemp();
  message.wheel_speed = m_subsystem.getWheelRPM();

  RCLCPP_INFO(this->get_logger(),
    "Publishing — Battery Level: %.1f%% | Motor Temp: %.1f°C | Wheel Speed: %.1f RPM",
    message.battery_level, message.motor_temp, message.wheel_speed);

  m_publisher->publish(message);
}

void SensorNode::emergencyStopCallback(
  const std::shared_ptr<custom_interfaces::srv::EmergencyStop::Request> request,
  std::shared_ptr<custom_interfaces::srv::EmergencyStop::Response> response)
{
  RCLCPP_ERROR(this->get_logger(), "EMERGENCY STOP received. Reason: %s", request->reason.c_str());
  m_subsystem.reset();
  response->acknowledged = true;
  RCLCPP_INFO(this->get_logger(), "System reset. Resuming nominal operations.");
}