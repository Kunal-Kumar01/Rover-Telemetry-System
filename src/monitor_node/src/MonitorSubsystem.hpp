#ifndef MONITOR_NODE__MONITORSUBSYSTEM_HPP_
#define MONITOR_NODE__MONITORSUBSYSTEM_HPP_

#include <string>

enum class AlertLevel {
  NOMINAL,
  WARNING,
  CRITICAL
};

struct Thresholds {
  float battery_warning;
  float battery_critical;
  float motor_temp_warning;
  float motor_temp_critical;
  float wheel_speed_warning;
  float wheel_speed_critical;
};

class MonitorSubsystem {
public:
  MonitorSubsystem();

  void setThresholds(const Thresholds & thresholds);

  AlertLevel checkThresholds(float battery, float motor_temp, float wheel_speed);

  std::string formatAlert(AlertLevel level, float battery, float motor_temp, float wheel_speed);

private:
  Thresholds m_thresholds;
};

#endif 