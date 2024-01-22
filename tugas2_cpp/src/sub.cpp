#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/string.hpp"

class AndSubscriber : public rclcpp::Node
{
public:
  AndSubscriber()
    : Node("and_subscriber")
  {
    subscription1_ = this->create_subscription<std_msgs::msg::Bool>(
      "random_bool1", 10, std::bind(&AndSubscriber::randomBool1Callback, this, std::placeholders::_1));

    subscription2_ = this->create_subscription<std_msgs::msg::Bool>(
      "random_bool2", 10, std::bind(&AndSubscriber::randomBool2Callback, this, std::placeholders::_1));

    status_subscription1_ = this->create_subscription<std_msgs::msg::String>(
      "status1", 10, std::bind(&AndSubscriber::status1Callback, this, std::placeholders::_1));

    status_subscription2_ = this->create_subscription<std_msgs::msg::String>(
      "status2", 10, std::bind(&AndSubscriber::status2Callback, this, std::placeholders::_1));
  }

private:
  void randomBool1Callback(const std_msgs::msg::Bool::SharedPtr msg)
  {
    bool1_ = msg->data;
    checkAndPublish();
  }

  void randomBool2Callback(const std_msgs::msg::Bool::SharedPtr msg)
  {
    bool2_ = msg->data;
    checkAndPublish();
  }

  void status1Callback(const std_msgs::msg::String::SharedPtr msg)
  {
    status1_ = msg->data;
    checkAndPublish();
  }

  void status2Callback(const std_msgs::msg::String::SharedPtr msg)
  {
    status2_ = msg->data;
    checkAndPublish();
  }

  void checkAndPublish()
  {
    if (!status1_.empty() && !status2_.empty()) {
      std::string result = bool1_ && bool2_ ? "sudah ready nih, gass min!" : "tunggu dulu, kami belum ready!";
      RCLCPP_INFO(this->get_logger(), "pub1 - %s | pub2 - %s = %s", status1_.c_str(), status2_.c_str(), result.c_str());
    }
  }

  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr subscription1_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr subscription2_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr status_subscription1_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr status_subscription2_;
  bool bool1_ = false;
  bool bool2_ = false;
  std::string status1_;
  std::string status2_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<AndSubscriber>());
  rclcpp::shutdown();
  return 0;
}
