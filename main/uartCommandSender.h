#ifndef UartCommandSender_h
#define UartCommandSender_h

#include "esp_types.h"
#include "driver/gpio.h"
#include <string>

class UartCommandSender
{
public:
	UartCommandSender(gpio_num_t rx, gpio_num_t tx);
	~UartCommandSender();
	void SendCommand(std::string command);
};

#endif