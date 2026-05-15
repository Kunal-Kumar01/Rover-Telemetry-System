#include "MonitorSubsystem.hpp"

MonitorSubsystem::MonitorSubsystem()
{
  // default thresholds
  m_thresholds.battery_warning = 30.0f;
  m_thresholds.battery_critical = 15.0f;
  m_thresholds.motor_temp_warning = 75.0f;
  m_thresholds.motor_temp_critical = 90.0f;
  m_thresholds.wheel_speed_warning = 40.0f;
  m_thresholds.wheel_speed_critical = 48.0f;
}

void MonitorSubsystem::setThresholds(const Thresholds & thresholds)
{
  m_thresholds = thresholds;
}

AlertLevel MonitorSubsystem::checkThresholds(float battery, float motor_temp, float wheel_speed)
{
  if (battery <= m_thresholds.battery_critical ||
    motor_temp >= m_thresholds.motor_temp_critical ||
    wheel_speed >= m_thresholds.wheel_speed_critical)
  {
    return AlertLevel::CRITICAL;
  }

  if (battery <= m_thresholds.battery_warning ||
    motor_temp >= m_thresholds.motor_temp_warning ||
    wheel_speed >= m_thresholds.wheel_speed_warning)
  {
    return AlertLevel::WARNING;
  }

  return AlertLevel::NOMINAL;
}

std::string MonitorSubsystem::formatAlert(AlertLevel level, float battery, float motor_temp, float wheel_speed)
{
  std::string status;
  if (level == AlertLevel::NOMINAL) status = "NOMINAL";
  else if (level == AlertLevel::WARNING) status = "WARNING";
  else status = "CRITICAL";

  return "[" + status + "] Battery: " + std::to_string(battery) +
    "% | Motor Temp: " + std::to_string(motor_temp) +
    "C | Wheel Speed: " + std::to_string(wheel_speed) + " RPM";
}