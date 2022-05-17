#include <WiFiNINA.h>

char ssid[] = "2E10_TC01";    
char pass[] = "LindaDoyle";    

void setup() {
 Serial.begin(9600);
 WiFi.begin(ssid, pass);
// IPAddress ip = WiFi.localIP();
// Serial.print("IP Address:");
// Serial.println(ip);
}

void loop() {

  IPAddress ip = WiFi.localIP();
 Serial.print("IP Address:");
 Serial.println(ip);
  
}

// 192.168.43.47
