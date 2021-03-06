/*
    ESPHelper.h
    Copyright (c) 2017 ItKindaWorks Inc All right reserved.
    github.com/ItKindaWorks

    This file is part of ESPHelper

    ESPHelper is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ESPHelper is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ESPHelper.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef ESP_HELPER_H
#define ESP_HELPER_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "sharedData.h"
#include <functional>

#include "Metro.h"

// #define VERSION "1-5-6"

#define DEBUGESPHELPER

#ifdef DEBUGESPHELPER
	#define debugPrint(x) Serial.print(x) //debug on
	#define debugPrintln(x) Serial.println(x) //debug on
#else
	#define debugPrint(x) {;} //debug off
	#define debugPrintln(x) {;} //debug off
#endif


#define VOID_CALLBACK_SIGNATURE std::function<void()> callback

class ESPHelper{

public:
	ESPHelper();
	ESPHelper(const netInfo *startingNet);
	ESPHelper(netInfo *netList[], uint8_t netCount, uint8_t startIndex = 0);	//remember netInfo *netList[] is really netinfo**
	ESPHelper(const char *ssid, const char *pass, const char *mqttIP);
	ESPHelper(const char *ssid, const char *pass);
	ESPHelper(const char *ssid, const char *pass, const char *mqttIP, const char *mqttUser, const char *mqttPass, const int mqttPort);
	ESPHelper(const char *ssid, const char *pass, const char *mqttIP, const char *willTopic, const char *willMessage);
	ESPHelper(const char *ssid, const char *pass, const char *mqttIP, const char *willTopic, const char *willMessage, const int willQoS, const int willRetain);
	ESPHelper(const char *ssid, const char *pass, const char *mqttIP, const char *mqttUser, const char *mqttPass, const int mqttPort, const char *willTopic, const char *willMessage, const int willQoS, const int willRetain);

	ESPHelper(const char* configFile);


	bool begin(const char* filename);
	bool begin(const netInfo *startingNet);
	bool begin(const char *ssid, const char *pass, const char *mqttIP);
	bool begin(const char *ssid, const char *pass);
	bool begin(const char *ssid, const char *pass, const char *mqttIP, const char *mqttUser, const char *mqttPass, const int mqttPort);
	bool begin(const char *ssid, const char *pass, const char *mqttIP, const char *mqttUser, const char *mqttPass, const int mqttPort, const char *willTopic, const char *willMessage);
	bool begin(const char *ssid, const char *pass, const char *mqttIP, const char *mqttUser, const char *mqttPass, const int mqttPort, const char *willTopic, const char *willMessage, const int willQoS, const int willRetain);
	bool begin();
	void end();

	
	netInfo loadConfigFile(const char* filename);
	bool saveConfigFile(const netInfo config, const char* filename);

	void useSecureClient(const char* fingerprint);

	void broadcastMode(const char* ssid, const char* password, const IPAddress ip);
	void disableBroadcast();

	int loop();

	bool subscribe(const char* topic, int qos);
	bool addSubscription(const char* topic);
	bool removeSubscription(const char* topic);
	bool unsubscribe(const char* topic);

	void publish(const char* topic, const char* payload);
	void publish(const char* topic, const char* payload, bool retain);

	bool setCallback(MQTT_CALLBACK_SIGNATURE);
	void setMQTTCallback(MQTT_CALLBACK_SIGNATURE);

	void setWifiCallback(void (*callback)());
	void setMQTTConnectCallback(void (*callback)());
	void setMQTTDisconnectCallback(void (*callback)());
	void setWillTopicCallback(String (*callback)());

	void reconnect();

	void updateNetwork();	//manually disconnect and reconnecting to network/mqtt using current values (generally called after setting new network values)

	const char* getSSID();
	void setSSID(const char *ssid);

	const char* getPASS();
	void setPASS(const char *pass);

	const char* getMQTTIP();
	void setMQTTIP(const char *mqttIP);
	void setMQTTIP(const char *mqttIP, const char *mqttUser, const char *mqttPass);

	int getMQTTQOS();
	void setMQTTQOS(int qos);

	void setWill(const char *willTopic, const char *willMessage);
	void setWill(const char *willTopic, const char *willMessage, const int willQoS, const int willRetain);

	String getIP();
	IPAddress getIPAddress();

	int getStatus();

	void setNetInfo(netInfo newNetwork);
	void setNetInfo(netInfo *newNetwork);
	// netInfo* getNetInfo();
	netInfo getNetInfo();

	void setHopping(bool canHop);

	void listSubscriptions();

	void enableHeartbeat(int16_t pin);
	void disableHeartbeat();
	void heartbeat();

	void OTA_enable();
	void OTA_disable();
	void OTA_begin();
	void OTA_setPassword(const char* pass);
	void OTA_setHostname(const char* hostname);
	void OTA_setHostnameWithVersion(const char* hostname);
	char* getHostname();

private:

	void init(const char *ssid, const char *pass, const char *mqttIP, const char *mqttUser, const char *mqttPass, const int mqttPort, const char *willTopic, const char *willMessage, const int willQoS, const int willRetain);
	void validateConfig();

	void changeNetwork();

	String macToStr(const uint8_t* mac);

	bool checkParams();

	void resubscribe();

	int setConnectionStatus();

	netInfo _currentNet;

	Metro reconnectMetro = Metro(500);

	PubSubClient client;
	WiFiClient wifiClient;
	WiFiClientSecure wifiClientSecure;
	const char* _fingerprint;
	bool _useSecureClient = false;


	String _clientName;

	void (*_wifiCallback)();
	bool _wifiCallbackSet = false;

	#ifdef ESP8266
		std::function<void(char*, uint8_t*, unsigned int)> _mqttCallback;
	#endif
	#ifdef ESP32
		void(*_mqttCallback)(char*, uint8_t*, unsigned int) ;
	#endif
	bool _mqttCallbackSet = false;


	void (*_mqttConnectCallback)();
	bool _mqttConnectCallbackSet = false;

	void (*_mqttDisconnectCallback)();
	bool _mqttDisconnectCallbackSet = false;

	String (*_willTopicCallback)();
	bool _willTopicCallbackSet = false;
	String _willTopic;
	

	int _connectionStatus = NO_CONNECTION;

	//AP mode variables
	IPAddress _broadcastIP;
	char _broadcastSSID[64];
	char _broadcastPASS[64];

	//netInfo array vars (total + current index)
	uint8_t _netCount = 0;
	uint8_t _currentIndex = 0;

	bool _ssidSet = false;
	bool _passSet = false;
	bool _mqttSet = false;
	bool _mqttUserSet = false;
  	bool _mqttPassSet = false;
	bool _willTopicSet = false;
  	bool _willMessageSet = false;

	bool _useOTA = false;
	bool _OTArunning = false;

	bool _hoppingAllowed = false;

	bool _hasBegun = false;

	netInfo **_netList;

	int16_t _ledPin = 2;
	bool _heartbeatEnabled = false;

	subscription _subscriptions[MAX_SUBSCRIPTIONS];

	char _hostname[64];

	int _qos = DEFAULT_QOS;

	IPAddress _apIP = IPAddress(192, 168, 1, 1);


};

#endif
