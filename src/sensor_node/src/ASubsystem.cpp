#include "ASubsystem.hpp"
#include <random>
#include <algorithm>


ASubsystem::ASubsystem()
: battery_level_(100.0f),
  motor_temp_(20.0f),
  wheel_speed_(25.0f)
  {

  }

void ASubsystem::simulateReadings(){
    battery_level_ -=0.5f;
    battery_level_ += randomness(-0.2f, 0.2f);
    battery_level_ = std::clamp(battery_level_, 0.0f, 100.0f);

    motor_temp_ += randomness(-1.0f, 1.5f);
    motor_temp_ = std::clamp(motor_temp_, 20.0f, 120.0f);

    wheel_speed_ += randomness(-2.0f, 2.0f);
    wheel_speed_ = std::clamp(wheel_speed_, 0.0f, 50.0f);
}

float ASubsystem::getBatteryLevel() const{
    return battery_level_;
}

float ASubsystem::getMotorTemp() const
{
  return motor_temp_;
}

float ASubsystem::getWheelRPM() const
{
  return wheel_speed_;
}

void ASubsystem::reset()
{
  battery_level_ = 100.0f;
  motor_temp_ = 20.0f;
  wheel_speed_ = 25.0f;
}

float ASubsystem::randomness(float min, float max){
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng); 
}