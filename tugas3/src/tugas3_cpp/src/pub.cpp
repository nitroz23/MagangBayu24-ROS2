#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
    : Node("minimal_publisher"), i(0)
  {
    publisher_ = create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
    timer_ = create_wall_timer(std::chrono::seconds(2), std::bind(&MinimalPublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    auto msg = std::make_unique<geometry_msgs::msg::Twist>();
    msg->linear.x = translasi[i];
    msg->angular.z = rotasi___[i];
    publisher_->publish(std::move(msg));
    i = (i + 1) % (sizeof(translasi) / sizeof(translasi[0]));
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t i;

  double translasi[13] = {2.00, 0.00, 6.28, 0.00, 4.00, 0.00, 6.28, 0.00, 3.90, 0.00, 6.28, 0.00, 2.00};
  double rotasi___[13] = {0.00, 0.52, 3.14, 0.52, 0.00, 0.52, 3.14, 0.52, 0.00, 0.52, 3.14, 0.52, 0.00};
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto minimal_publisher = std::make_shared<MinimalPublisher>();
  rclcpp::spin(minimal_publisher);
  rclcpp::shutdown();
  return 0;
}
