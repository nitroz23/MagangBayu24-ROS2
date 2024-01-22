#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <string>
#include <iostream>
#include <sstream>

class AnswerSubscriberNode : public rclcpp::Node {
public:
  AnswerSubscriberNode() : Node("answer_subscriber_node") {
    answer_subscription_ = create_subscription<std_msgs::msg::String>(
      "equation_topic", 10, std::bind(&AnswerSubscriberNode::answer_callback, this, std::placeholders::_1));
  }

private:
  void answer_callback(const std_msgs::msg::String::SharedPtr message) {
    auto equation = message->data;
    auto answer = calculate_answer(equation);
    RCLCPP_INFO(get_logger(), "'%s' = '%f'", equation.c_str(), answer);
  }

  float calculate_answer(const std::string& equation) {
    float operand1, operand2, operand3;
    char operation1, operation2;

    std::istringstream iss(equation);
    iss >> operand1 >> operation1 >> operand2 >> operation2 >> operand3;

    float result1, result2;
    
    if (operation1 == '*' || operation1 == '/' || operation1 == '%') {
        result1 = perform_operation(operand1, operand2, operation1);
        result2 = perform_operation(result1, operand3, operation2);
    } else if (operation1 == operation2) {
        result1 = perform_operation(operand1, operand2, operation1);
        result2 = perform_operation(result1, operand3, operation2);        
    } else {
        result1 = perform_operation(operand2, operand3, operation2);
        result2 = perform_operation(operand1, result1, operation1); 
    }
    return result2;
    }

  float perform_operation(float operand1, float operand2, char operation) {
    switch (operation) {
      case '+':
        return operand1 + operand2;
      case '-':
        return operand1 - operand2;
      case '*':
        return operand1 * operand2;
      case '/':
        if (operand2 != 0.0) {
          return operand1 / operand2;
        } else {
          RCLCPP_ERROR(get_logger(), "Division by zero error!");
          return 0.0;
        }
      case '%':
        if (operand2 != 0.0) {
          return static_cast<int>(operand1) % static_cast<int>(operand2);
        } else {
          RCLCPP_ERROR(get_logger(), "Modulus by zero error!");
          return 0.0;
        }
      default:
        RCLCPP_ERROR(get_logger(), "Invalid operation in equation: '%c'", operation);
        return 0.0;
    }
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr answer_subscription_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<AnswerSubscriberNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
