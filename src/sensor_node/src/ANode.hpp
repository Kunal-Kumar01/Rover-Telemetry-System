#ifndef SENSOR_NODE__ANODE_HPP_
#define SENSOR_NODE__ANODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/msg/rover_telemetry.hpp"
#include "custom_interfaces/srv/emergency_stop.hpp"
#include "ASubsystem.hpp"

class ANode:public rclcpp::Node
{
    public:
        ANode();
    private:
        void sensorCallback();
        void emergencyStopCallback(
            const std::shared_ptr<custom_interfaces::srv::EmergencyStop::Request> request,
            std::shared_ptr<custom_interfaces::srv::EmergencyStop::Response> response);

        ASubsystem subsystem_;
        rclcpp::Publisher<custom_interfaces::msg::RoverTelemetry>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Service<custom_interfaces::srv::EmergencyStop>::SharedPtr emergency_stop_service_;
};
#endif