#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <cstdlib>
#include <ctime>
#include <sstream>

class EquationPublisherNode : public rclcpp::Node {
public:
  EquationPublisherNode() : Node("equation_publisher_node") {
    equation_publisher_ = create_publisher<std_msgs::msg::String>("equation_topic", 10);
    timer_ = create_wall_timer(std::chrono::seconds(1), std::bind(&EquationPublisherNode::publish_equation, this));

    std::srand(std::time(0));
  }

private:
  void publish_equation() {
    auto equation = generate_random_equation();
    RCLCPP_INFO(get_logger(), "Publishing Equation: '%s'", equation.c_str());

    auto message = std_msgs::msg::String();
    message.data = equation;
    equation_publisher_->publish(message);
  }

  std::string generate_random_equation() {
    float operand1 = static_cast<float>(std::rand() % 10000 + 1);
    float operand2 = static_cast<float>(std::rand() % 10000 + 1);
    float operand3 = static_cast<float>(std::rand() % 10000 + 1);

    int operation1 = std::rand() % 5; 
    int operation2 = std::rand() % 5;

    std::string equation;
    switch (operation1) {
      case 0:
        equation = std::to_string(operand1) + " + ";
        break;
      case 1:
        equation = std::to_string(operand1) + " - ";
        break;
      case 2:
        equation = std::to_string(operand1) + " * ";
        break;
      case 3:
        equation = std::to_string(operand1) + " / ";
        break;
      case 4:
        equation = std::to_string(static_cast<int>(operand1)) + " % ";
        break;
      default:
        break;
    }

    switch (operation2) {
      case 0:
        equation += std::to_string(operand2) + " + " + std::to_string(operand3);
        break;
      case 1:
        equation += std::to_string(operand2) + " - " + std::to_string(operand3);
        break;
      case 2:
        equation += std::to_string(operand2) + " * " + std::to_string(operand3);
        break;
      case 3:
        equation += std::to_string(operand2) + " / " + std::to_string(operand3);
        break;
      case 4:
        equation += std::to_string(static_cast<int>(operand2)) + " % " + std::to_string(static_cast<int>(operand3));
        break;
      default:
        break;
    }

    return equation;
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr equation_publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<EquationPublisherNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
