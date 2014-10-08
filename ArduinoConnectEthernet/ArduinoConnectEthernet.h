#ifndef ArduinoConnectEthernet_h
#define ArduinoConnectEthernet_h

#include <Arduino.h>
#include <Ethernet.h>
#include <SPI.h>

class ArduinoConnectEthernet
{
	public:
		ArduinoConnectEthernet();
		void progMode(EthernetServer server);
	private:
		bool reading;
		String input;
		String result[4];
		bool connected;
		char buffer[50];
		void parseRequest(String str);
		bool isConnected();
		void setConnected(bool value);
};

#endif