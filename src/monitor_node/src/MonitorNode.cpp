#include "MonitorNode.hpp"

using std::placeholders::_1;

MonitorNode::MonitorNode()
: Node("monitor_node")
{
  this->declare_parameter("battery_warning", 30.0);
  this->declare_parameter("battery_critical", 15.0);
  this->declare_parameter("motor_temp_warning", 75.0);
  this->declare_parameter("motor_temp_critical", 90.0);
  this->declare_parameter("wheel_speed_warning", 40.0);
  this->declare_parameter("wheel_speed_critical", 48.0);

  Thresholds t;
  t.battery_warning = this->get_parameter("battery_warning").as_double();
  t.battery_critical = this->get_parameter("battery_critical").as_double();
  t.motor_temp_warning = this->get_parameter("motor_temp_warning").as_double();
  t.motor_temp_critical = this->get_parameter("motor_temp_critical").as_double();
  t.wheel_speed_warning = this->get_parameter("wheel_speed_warning").as_double();
  t.wheel_speed_critical = this->get_parameter("wheel_speed_critical").as_double();
  m_subsystem.setThresholds(t);

  m_param_callback = this->add_on_set_parameters_callback(
    std::bind(&MonitorNode::parametersCallback, this, std::placeholders::_1));

  m_subscription = this->create_subscription<custom_interfaces::msg::RoverTelemetry>(
    "rover_telemetry", 10,
    std::bind(&MonitorNode::monitorCallback, this, _1));

  m_emergency_stop_client = this->create_client<custom_interfaces::srv::EmergencyStop>(
    "emergency_stop");

  RCLCPP_INFO(this->get_logger(), "Monitor node started.");
}

rcl_interfaces::msg::SetParametersResult MonitorNode::parametersCallback(
  const std::vector<rclcpp::Parameter> & parameters)
{
  Thresholds t;

  t.battery_warning = this->get_parameter("battery_warning").as_double();
  t.battery_critical = this->get_parameter("battery_critical").as_double();
  t.motor_temp_warning = this->get_parameter("motor_temp_warning").as_double();
  t.motor_temp_critical = this->get_parameter("motor_temp_critical").as_double();
  t.wheel_speed_warning = this->get_parameter("wheel_speed_warning").as_double();
  t.wheel_speed_critical = this->get_parameter("wheel_speed_critical").as_double();

  for (const auto & param : parameters) {

    if (param.get_name() == "battery_warning") {
      t.battery_warning = param.as_double();
    }

    else if (param.get_name() == "battery_critical") {
      t.battery_critical = param.as_double();
    }

    else if (param.get_name() == "motor_temp_warning") {
      t.motor_temp_warning = param.as_double();
    }

    else if (param.get_name() == "motor_temp_critical") {
      t.motor_temp_critical = param.as_double();
    }

    else if (param.get_name() == "wheel_speed_warning") {
      t.wheel_speed_warning = param.as_double();
    }

    else if (param.get_name() == "wheel_speed_critical") {
      t.wheel_speed_critical = param.as_double();
    }
  }

  m_subsystem.setThresholds(t);

  RCLCPP_INFO(this->get_logger(), "Thresholds updated.");

  rcl_interfaces::msg::SetParametersResult result;
  result.successful = true;
  return result;
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