#ifndef SENSOR_NODE__SENSORNODE_HPP_
#define SENSOR_NODE__SENSORNODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/msg/rover_telemetry.hpp"
#include "custom_interfaces/srv/emergency_stop.hpp"
#include "SensorSubsystem.hpp"
#include "std_srvs/srv/trigger.hpp"

class SensorNode:public rclcpp::Node
{
    public:
        SensorNode();
    private:
        void sensorCallback();
        void emergencyStopCallback(
            const std::shared_ptr<custom_interfaces::srv::EmergencyStop::Request> request,
            std::shared_ptr<custom_interfaces::srv::EmergencyStop::Response> response);

        void resetCallback(
            const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
            std::shared_ptr<std_srvs::srv::Trigger::Response> response);

        SensorSubsystem m_subsystem;
        rclcpp::Publisher<custom_interfaces::msg::RoverTelemetry>::SharedPtr m_publisher;
        rclcpp::TimerBase::SharedPtr m_timer;
        rclcpp::Service<custom_interfaces::srv::EmergencyStop>::SharedPtr m_emergency_stop_service;
        rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr m_reset_service;
        rclcpp::CallbackGroup::SharedPtr m_callback_group;

        bool m_is_stopped;
};
#endif