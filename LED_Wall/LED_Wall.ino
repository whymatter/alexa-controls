#include <ESP8266WiFi.h>

WiFiServer server(80);
unsigned int initialized = 0;

String prepareHtmlPage()
{
  String htmlPage =
     String("HTTP/1.1 204 No Content\r\n") +
            "Connection: close\r\n" +  // the connection will be closed after completion of the response
            "\r\n";
  return htmlPage;
}

void transmit_ir(unsigned int data)
{
  digitalWrite(2, LOW);

  delayMicroseconds(9000);
  
  digitalWrite(2, HIGH);

  delayMicroseconds(4500);
  
  for (unsigned int i; i < 32; i++) {
    digitalWrite(2, LOW);
  
    delayMicroseconds(562);
    
    digitalWrite(2, HIGH);

    if ((data & 0x1) == 1) {
      delayMicroseconds(1687);
    } else {
      delayMicroseconds(562);
    }

    data >>= 1;
  }
  
  digitalWrite(2, LOW);

  delayMicroseconds(562);
  
  digitalWrite(2, HIGH);
}

void setup()
{  

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  
  transmit_ir(0xfc03ef00);
  delay(500);
  transmit_ir(0xfb04ef00);
  delay(500);
  
  Serial.begin(115200);
  Serial.println();

  WiFi.begin("seitz", "www.G00GL3.d3");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop()
{

  if (initialized == 0)
  {
    transmit_ir(0xfa05ef00);
    initialized = 1;
  }

  unsigned int led_command = 0x0;
  
  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          client.println(prepareHtmlPage());
          break;
        }
        else if (line.startsWith("GET /led/") && line.length() >= 19)
        {
          String string_command = line.substring(9, 19);
          Serial.println("\n\nLED command, string: " + string_command);
          const char * cstr = string_command.c_str();
          led_command = strtoul(cstr, NULL, 16);
          Serial.print("LED command, hex: 0x");
          Serial.println(led_command, HEX);
        }
      }
    }
    delay(1000); // give the web browser time to receive the data

    // close the connection:
    client.stop();
    Serial.println("[Client disonnected]");
  }

  if (led_command != 0x0) {
    transmit_ir(led_command);
  }


    //transmit_ir(0xf906ef00);
    //delay(2000);
    //transmit_ir(0xfa05ef00);
    //delay(2000);
    //transmit_ir(0xfb04ef00);
    //delay(2000);
}
