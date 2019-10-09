//ESP libs
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include <string>

//my libs
#include "mWifiClient.h"
#include "mMqttClient.h"

#include "main.h"

using namespace std;

static const char *logTag = "Main";

static string mqttCommandTopic = CONFIG_MQTT_COMMAND_TOPIC;
static string mqttCommandReturnTopic; // will be the mqttCommandTopic + "Ret"
static const string mqttBrokerAddress = CONFIG_MQTT_BROKER_ADDRESS;

extern "C"
{
	void app_main(void);
}

void app_main()
{
	Main main = Main();
	main.Run();
}

void Main::Run()
{
	commandSender = new UartCommandSender(GPIO_NUM_5, GPIO_NUM_4);
	wifiClient = new mWifiClient(CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD, 5);
	StartWifi();
	mqttClient = new mMqttClient(mqttBrokerAddress);
	mqttCommandReturnTopic = mqttCommandTopic + "Ret";
	mqttClient->Subscribe(mqttCommandTopic, 1, Main::HandleMqttMessage, this);
}

void Main::StartWifi()
{
	int retryCount = 0;
	while (!wifiClient->Connect(++retryCount * 5000) && retryCount < 5)
	{
		ESP_LOGI(logTag, "Failed to connect. Retrying.. %d/5", retryCount);
	}
}

void Main::HandleMqttMessage(string topic, string message, void* arg)
{
	Main* instance = (Main*)arg;
	printf("sending command: %s\n", message.c_str());
	instance->commandSender->SendCommand(message);
}