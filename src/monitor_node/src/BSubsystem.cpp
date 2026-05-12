#include "BSubsystem.hpp"

BSubsystem::BSubsystem()
{
  // default thresholds
  thresholds_.battery_warning = 30.0f;
  thresholds_.battery_critical = 15.0f;
  thresholds_.motor_temp_warning = 75.0f;
  thresholds_.motor_temp_critical = 90.0f;
  thresholds_.wheel_speed_warning = 40.0f;
  thresholds_.wheel_speed_critical = 48.0f;
}

void BSubsystem::setThresholds(const Thresholds & thresholds)
{
  thresholds_ = thresholds;
}

AlertLevel BSubsystem::checkThresholds(float battery, float motor_temp, float wheel_speed)
{
  if (battery <= thresholds_.battery_critical ||
    motor_temp >= thresholds_.motor_temp_critical ||
    wheel_speed >= thresholds_.wheel_speed_critical)
  {
    return AlertLevel::CRITICAL;
  }

  if (battery <= thresholds_.battery_warning ||
    motor_temp >= thresholds_.motor_temp_warning ||
    wheel_speed >= thresholds_.wheel_speed_warning)
  {
    return AlertLevel::WARNING;
  }

  return AlertLevel::NOMINAL;
}

std::string BSubsystem::formatAlert(AlertLevel level, float battery, float motor_temp, float wheel_speed)
{
  std::string status;
  if (level == AlertLevel::NOMINAL) status = "NOMINAL";
  else if (level == AlertLevel::WARNING) status = "WARNING";
  else status = "CRITICAL";

  return "[" + status + "] Battery: " + std::to_string(battery) +
    "% | Motor Temp: " + std::to_string(motor_temp) +
    "C | Wheel Speed: " + std::to_string(wheel_speed) + " RPM";
}