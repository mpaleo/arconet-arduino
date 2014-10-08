#include <Arduino.h>
#include <ArduinoConnectEthernet.h>
#include <Base64.h>
#include <RC4.h>
#include <Ethernet.h>
#include <SPI.h>

ArduinoConnectEthernet::ArduinoConnectEthernet()
{
	reading = false;
	input = "";
}

void ArduinoConnectEthernet::progMode(EthernetServer server)
{
	setConnected(true);
	while(isConnected() == true)
	{
		EthernetClient client = server.available();
	    if (client) 
	    {
	        boolean currentLineIsBlank = true;
	        input = "";
	        while (client.connected()) 
	        {
	            if (client.available()) 
	            {
	                char c = client.read();
	                if(reading && c == ' ') reading = false;
	                if(c == '?') 
	                {
	                    reading = true;
	                }

	                if(reading)
	                {
	                	//opt
	                	//Serial.println(c);
	                    if (c!='?') 
	                    {
	                        input += c;
	                    }
	                }

	                if(c == '\n' && currentLineIsBlank)  
	                {
	                    break;
	                }

	                if (c == '\n') 
	                {
	                    currentLineIsBlank = true;
	                }
	                else if (c != '\r') 
	                {
	                    currentLineIsBlank = false;
	                }
	            }
	        }
	        memset(buffer,'\0',sizeof(buffer));
	        String httpValue = "arduinoValue: ";
	        bool readCmd = false;
	        parseRequest(input);
	        if(result[0] == "pinMode")
			{
				if(result[2] == "0")
				{
					result[1].toCharArray(buffer, 50);
					pinMode(atoi(buffer), INPUT);
					Serial.println(result[0] + " " + result[1] + " " + result[2]);
				}
				else if(result[2] == "1")
				{
					result[1].toCharArray(buffer, 50);
					pinMode(atoi(buffer), OUTPUT);
					Serial.println("1");
				}
				else if(result[2] == "2")
				{
					result[1].toCharArray(buffer, 50);
					pinMode(atoi(buffer), INPUT_PULLUP);
					Serial.println("1");
				}
			}
			else if(result[0] == "digitalWrite")
			{
				if(result[2] == "0")
				{
					result[1].toCharArray(buffer, 50);
					digitalWrite(atoi(buffer), LOW);
					Serial.println("lw");
				}
				else if(result[2] == "1")
				{
					result[1].toCharArray(buffer, 50);
					digitalWrite(atoi(buffer), HIGH);
					Serial.println("hi");
				}
			}
			else if(result[0] == "digitalRead")
			{
				result[1].toCharArray(buffer, 50);
				Serial.println(digitalRead(atoi(buffer)));
				if(digitalRead(atoi(buffer)))
				{
					httpValue = httpValue + "HIGH";
				}
				else
				{
					httpValue = httpValue + "LOW";
				}
				readCmd = true;
			}
			else if(result[0] == "analogWrite")
			{
				result[1].toCharArray(buffer, 50);
				char buffer2[50];
				result[2].toCharArray(buffer2, 50);
				analogWrite(atoi(buffer),atoi(buffer2));
				Serial.println("1");
			}
			else if(result[0] == "analogRead")
			{
				result[1].toCharArray(buffer, 50);
				Serial.println(analogRead(atoi(buffer)));

				httpValue = httpValue + analogRead(atoi(buffer));
				readCmd = true;
			}
			else if(result[0] == "continueFlow")
			{
				setConnected(false);
				Serial.println("1");
			}
			client.println("HTTP/1.1 200 OK");
			if(readCmd)
			{
				client.println(httpValue);
	    	}
	    	client.println("Connection: close");
	        client.stop();
	        delay(100);
		}
	}
	
}

void ArduinoConnectEthernet::parseRequest(String str) 
{
    RC4 rc4;
    char b64out[50];
    
    str.toCharArray(buffer, 50);
    
    base64_decode(b64out,buffer,strlen(buffer));
    // Change this key
    rc4.Encrypt(b64out,"SecretKey");
    
    Serial.print("Msg: ");
    Serial.println(b64out);

    char *p = b64out;
    String strtk;
    byte i = 0;
    memset(result,'\0',sizeof(result));
    if(str == "continueFlow")
	{
		result[0] = str;
	}
	else
	{
	    while((strtk = strtok_r(p, ":", &p)) != NULL)
	    {
			result[i] = strtk;
			i++;
			delay(50);
	    }
	}
}

bool ArduinoConnectEthernet::isConnected()
{
	return connected;
}

void ArduinoConnectEthernet::setConnected(bool value)
{
	connected = value;
}