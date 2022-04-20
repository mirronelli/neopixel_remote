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
static const gpio_num_t ledPin = GPIO_NUM_27;

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
	gpio_set_direction(ledPin, GPIO_MODE_OUTPUT);
	commandSender = new UartCommandSender(GPIO_NUM_5, GPIO_NUM_4);
	wifiClient = new mWifiClient(CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD, Main::WifiConnectHandler, Main::WifiDisconnectHandler, this);
	mqttClient = new mMqttClient(mqttBrokerAddress, Main::MqttConnectHandler, Main::MqttDisconnectHandler, this);
	// mqttCommandReturnTopic = mqttCommandTopic + "Ret";

	while (true)
	{
		if (!wifiClient->IsConnected())
		{
			wifiClient->Connect(5000);
		}

		vTaskDelay(30'000/portTICK_RATE_MS);
	}
}

void Main::ProcessWifiConnect()
{
	ESP_LOGI(logTag, "Wifi Connected.");
	mqttClient->Start();
}

void Main::ProcessWifiDisconnect()
{
	ESP_LOGI(logTag, "Disconnected.");
}

void Main::HandleMqttMessage(string topic, string message, void* arg)
{
	Main* instance = (Main*)arg;
	instance->Blink(100);
	printf("sending command: %s\n", message.c_str());
	instance->commandSender->SendCommand(message);
	instance->Blink(100);
	instance->Blink(100);
}

void Main::ProcessMqttConnect()
{
	ESP_LOGI(logTag, "Mqtt connected.");
	ESP_LOGI(logTag, "Subscribing.");
	mqttClient->Subscribe(mqttCommandTopic, 1, Main::HandleMqttMessage, this);
};

void Main::ProcessMqttDiconnect()
{
	ESP_LOGI(logTag, "Mqtt disconnected.");
};

void Main::WifiConnectHandler(void* arg)
{
	Main* instance = (Main*)arg;
	instance->ProcessWifiConnect();
}

void Main::WifiDisconnectHandler(void* arg)
{
	Main* instance = (Main*)arg;
	instance->ProcessWifiDisconnect();
}

void Main::MqttConnectHandler(void* arg)
{
	Main* instance = (Main*)arg;
	instance->ProcessMqttConnect();
};

void Main::MqttDisconnectHandler(void* arg)
{
	Main* instance = (Main*)arg;
	instance->ProcessMqttDiconnect();
};

void Main::Blink(int delay)
{
	HalfBlink(delay);
	HalfBlink(delay);
}

void Main::HalfBlink(int delay)
{
	ledState = !ledState;
	gpio_set_level(ledPin, ledState);
	vTaskDelay(delay / (2 * portTICK_RATE_MS));
}
