#include "uartCommandSender.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include <string>

#define PIN_RTS  (UART_PIN_NO_CHANGE)
#define PIN_CTS  (UART_PIN_NO_CHANGE)

UartCommandSender::UartCommandSender(gpio_num_t rx, gpio_num_t tx)
{
	/* Configure parameters of an UART driver,
     * communication pins and install the driver */
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
	uart_param_config(UART_NUM_1, &uart_config);
	uart_set_pin(UART_NUM_1, tx, rx, PIN_RTS, PIN_CTS);
	uart_driver_install(UART_NUM_1, 256, 256, 0, NULL, 0);
}

UartCommandSender::~UartCommandSender()
{
	uart_driver_delete(UART_NUM_1);
}

void UartCommandSender::SendCommand(std::string command)
{
	uart_write_bytes(UART_NUM_1, command.c_str(), command.length());
	uart_write_bytes(UART_NUM_1, "\r", 1);
}