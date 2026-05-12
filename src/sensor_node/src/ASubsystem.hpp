#ifndef SENSOR_NODE__ASUBSYSTEM_HPP_
#define SENSOR_NODE__ASUBSYSTEM_HPP_

class ASubsystem {
    public: 
        ASubsystem();

        void simulateReadings();
        float getBatteryLevel() const;
        float getMotorTemp() const;
        float getWheelRPM() const;

        void reset();

    private:
        float battery_level_;
        float motor_temp_;
        float wheel_speed_;

        float randomness(float mins, float max);
};

#endif