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

	static void MqttConnectHandler(void* instance);
	static void MqttDisconnectHandler(void* instance);
	void ProcessMqttConnect();
	void ProcessMqttDiconnect();

	static void WifiConnectHandler(void* instance);
	static void WifiDisconnectHandler(void* instance);
	void ProcessWifiConnect();
	void ProcessWifiDisconnect();

	static void HandleMqttMessage(std::string topic, std::string message, void* arg);

	mMqttClient *mqttClient = nullptr;
	mWifiClient *wifiClient = nullptr;
	UartCommandSender *commandSender = nullptr;

	bool ledState = 0;
	void Blink(int delay);
	void HalfBlink(int delay);
};