#include "BNode.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<BNode>());
  rclcpp::shutdown();
}