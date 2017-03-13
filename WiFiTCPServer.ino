#include <ESP8266WiFi.h>

/* #define HTTP */

const char* ssid     = "ZigguratsExtender";
const char* password = "kashimbrush1987";
int pinLeft          = 13;
int pinRight         = 12;
bool ValLeft         = false;
bool ValRight        = false;

WiFiServer server(9055);

void setup() 
{
  Serial.begin(115200);
  delay(100);
  pinMode(pinLeft, OUTPUT);
  pinMode(pinRight, OUTPUT);
  digitalWrite(pinLeft, 0);
  digitalWrite(pinRight, 0);
  WiFi.begin(ssid, password);
  Serial.print("Wait.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }  
  server.begin();
  String addr = WiFi.localIP().toString();
  Serial.println("");
  Serial.print("Connected, address: ");
  Serial.println(addr);
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available())
  {
    delay(1);
  }
  
  String req = client.readStringUntil('\r');
  client.flush();
  #ifdef HTTP
  if (req.indexOf("/gpio13") != -1)
  #else
  if (req.indexOf("gpio13") != -1)
  #endif
  {
    ValLeft  = !ValLeft;
    digitalWrite(pinLeft, (ValLeft) ? HIGH : LOW);
    #ifdef HTTP
      client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    #endif
    client.print("13:");
    client.println(ValLeft);
    Serial.print("GPIO13 state:");
    Serial.println((ValLeft) ? "HIGH" : "LOW");
    client.stop();
  }
  #ifdef HTTP
  else if (req.indexOf("/gpio12") != -1)
  #else
  else if (req.indexOf("gpio12") != -1)
  #endif
  {
    ValRight = !ValRight;
    digitalWrite(pinRight, (ValRight) ? HIGH : LOW);
    #ifdef HTTP
      client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    #endif
    client.print("12:");
    client.println(ValRight);
    Serial.print("GPIO12 state:");
    Serial.println((ValRight) ? "HIGH" : "LOW");
    client.stop();
  }
  else {
    String addr = WiFi.localIP().toString();
    Serial.print("I am:");
    Serial.println(addr);
    #ifdef HTTP
      client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    #endif
    client.println(addr);
    client.stop();
    return;
  }
  client.flush();
  delay(1);
}
