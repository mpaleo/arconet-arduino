#include <Ethernet.h>
#include <SPI.h>
#include <RC4.h>
#include <Base64.h>
#include <ArduinoConnectEthernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,100);
EthernetServer server = EthernetServer(84);

ArduinoConnectEthernet arcoEthernet;


void setup()
{
    Serial.begin(9600);
    Ethernet.begin(mac, ip);
    server.begin();
    Serial.println(Ethernet.localIP());
}

void loop()
{
    Serial.println("begin progMode");
    arcoEthernet.progMode(server);
    Serial.println("end progMode");
}


