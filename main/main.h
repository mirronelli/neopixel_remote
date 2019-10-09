#include "mMqttClient.h"
#include "mWifiClient.h"
#include "uartCommandSender.h"
#include <string>

class Main
{
public:
	Main(){};
	void Run();

private:
	void StartWifi();
	void ProcessCommand(std::string command);
	static void HandleMqttMessage(std::string topic, std::string message, void* arg);

	mMqttClient *mqttClient = nullptr;
	mWifiClient *wifiClient = nullptr;
	UartCommandSender *commandSender = nullptr;
};