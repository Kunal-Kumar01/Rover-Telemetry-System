#include "SensorSubsystem.hpp"
#include <random>
#include <algorithm>


SensorSubsystem::SensorSubsystem()
: m_battery_level(100.0f),
  m_motor_temp(20.0f),
  m_wheel_speed(25.0f)
  {

  }

void SensorSubsystem::simulateReadings(){
    m_battery_level -=0.5f;
    m_battery_level += randomness(-0.2f, 0.2f);
    m_battery_level = std::clamp(m_battery_level, 0.0f, 100.0f);

    m_motor_temp += randomness(-1.0f, 1.5f);
    m_motor_temp = std::clamp(m_motor_temp, 20.0f, 120.0f);

    m_wheel_speed += randomness(-2.0f, 2.0f);
    m_wheel_speed = std::clamp(m_wheel_speed, 0.0f, 50.0f);
}

float SensorSubsystem::getBatteryLevel() const{
    return m_battery_level;
}

float SensorSubsystem::getMotorTemp() const
{
  return m_motor_temp;
}

float SensorSubsystem::getWheelRPM() const
{
  return m_wheel_speed;
}

void SensorSubsystem::reset()
{
  m_battery_level = 100.0f;
  m_motor_temp = 20.0f;
  m_wheel_speed = 25.0f;
}

float SensorSubsystem::randomness(float min, float max){
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng); 
}