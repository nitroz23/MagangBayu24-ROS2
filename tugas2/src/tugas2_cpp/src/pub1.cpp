#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/string.hpp"

class RandomPublisher1 : public rclcpp::Node
{
public:
  RandomPublisher1()
    : Node("random_publisher1")
  {
    publisher_ = this->create_publisher<std_msgs::msg::Bool>("random_bool1", 10);
    string_publisher_ = this->create_publisher<std_msgs::msg::String>("status1", 10);

    timer_ = this->create_wall_timer(
      std::chrono::seconds(2),
      std::bind(&RandomPublisher1::publishRandomBool, this));
  }

private:
  void publishRandomBool()
  {
    std_msgs::msg::Bool bool_msg;
    bool_msg.data = (std::rand() % 2) == 0;
    publisher_->publish(bool_msg);

    std_msgs::msg::String string_msg;
    string_msg.data = bool_msg.data ? "true" : "false";
    string_publisher_->publish(string_msg);
    RCLCPP_INFO(this->get_logger(), "publisher - 1 - %s", string_msg.data.c_str());
  }

  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr publisher_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr string_publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RandomPublisher1>());
  rclcpp::shutdown();
  return 0;
}
