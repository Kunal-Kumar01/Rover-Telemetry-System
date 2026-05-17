#include "SensorNode.hpp"
#include <chrono>

SensorNode::SensorNode()
: Node("sensor_node"),
  m_is_stopped(false)
{
  using namespace std::chrono_literals;

  m_callback_group = this->create_callback_group(
    rclcpp::CallbackGroupType::Reentrant);

  m_publisher = this->create_publisher<custom_interfaces::msg::RoverTelemetry>("rover_telemetry", 10);

  m_timer = this->create_wall_timer(500ms, [this]{ sensorCallback(); }, m_callback_group);

  m_emergency_stop_service = this->create_service<custom_interfaces::srv::EmergencyStop>(
    "emergency_stop",
    std::bind(&SensorNode::emergencyStopCallback, this, std::placeholders::_1, std::placeholders::_2),
    rmw_qos_profile_services_default,
    m_callback_group);

  m_reset_service = this->create_service<std_srvs::srv::Trigger>(
  "reset_rover",
  std::bind(&SensorNode::resetCallback, this,
    std::placeholders::_1, std::placeholders::_2),
  rmw_qos_profile_services_default,
  m_callback_group);


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
  RCLCPP_ERROR(this->get_logger(),
    "EMERGENCY STOP received. Reason: %s", request->reason.c_str());

  m_is_stopped = true;

  RCLCPP_WARN(this->get_logger(),
    "Rover halted. Call /reset_rover service to resume operations.");

  response->acknowledged = true;
}

void SensorNode::resetCallback(
  const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
  std::shared_ptr<std_srvs::srv::Trigger::Response> response)
{
  (void)request;

  if (!m_is_stopped) {
    RCLCPP_WARN(this->get_logger(), "Reset called but rover is not stopped.");
    response->success = false;
    response->message = "Rover is not stopped.";
    return;
  }

  m_subsystem.reset();
  m_is_stopped = false;

  RCLCPP_INFO(this->get_logger(), "Rover reset. Resuming nominal operations.");

  response->success = true;
  response->message = "Rover reset successfully.";
}