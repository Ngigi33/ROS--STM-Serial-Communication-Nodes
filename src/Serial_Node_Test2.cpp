#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <libserial/SerialPort.h>
// #include <SerialPort.h>
// #include <SerialStream.h>

// using namespace LibSerial ;

using std::placeholders::_1;

class SimpleSerialTransmitter : public rclcpp::Node
{
public:
  SimpleSerialTransmitter() : Node("simple_serial_transmitter")
  {
    declare_parameter<std::string>("port", "/dev/ttyUSB0");

    port_ = get_parameter("port").as_string();

    // sub_ = create_subscription<std_msgs::msg::String>(
    //     "serial_transmitter", 10, std::bind(&SimpleSerialTransmitter::msgCallback, this, _1));
    
    // pub_ = create_publisher<std_msgs::msg::String>("serial_receiver", 10);
    // timer_ = create_wall_timer(0.01s, std::bind(&SimpleSerialReceiver::timerCallback, this));

    
    arduino_.Open(port_);
    arduino_.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
    timer_ = this->create_wall_timer(std::chrono::seconds(1), std::bind(&SimpleSerialTransmitter::msgCallback, this));

    //msgCallback();
  }

  ~SimpleSerialTransmitter()
  {
    arduino_.Close();
  }
  // void timerCallback()
  // {
  //   auto message = std_msgs::msg::String();
  //   if (rclcpp::ok() && arduino_.IsDataAvailable())
  //   {
  //     arduino_.ReadLine(message.data);
  //   }

  //   pub_->publish(message);
  // }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
  std::string port_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
  // rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::TimerBase::SharedPtr timer_;
  LibSerial::SerialPort arduino_;

  void msgCallback()
  {
    RCLCPP_INFO_STREAM(this->get_logger(), "Sending Message ");
    arduino_.Write("Hello");

    auto message = std_msgs::msg::String();
    if (rclcpp::ok() && arduino_.IsDataAvailable())
    {
      arduino_.ReadLine(message.data);
      RCLCPP_INFO_STREAM(this->get_logger(), "MSG received from Stm:" << message.data);
    }
  }
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SimpleSerialTransmitter>());
  rclcpp::shutdown();
  return 0;
}