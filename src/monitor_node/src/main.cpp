#include "MonitorNode.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MonitorNode>());
  rclcpp::shutdown();
}