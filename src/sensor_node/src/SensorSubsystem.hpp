#ifndef SENSOR_NODE__SENSORSUBSYSTEM_HPP_
#define SENSOR_NODE__SENSORSUBSYSTEM_HPP_

class SensorSubsystem {
    public: 
        SensorSubsystem();

        void simulateReadings();
        float getBatteryLevel() const;
        float getMotorTemp() const;
        float getWheelRPM() const;

        void reset();

    private:
        float m_battery_level;
        float m_motor_temp;
        float m_wheel_speed;

        float randomness(float mins, float max);
};

#endif