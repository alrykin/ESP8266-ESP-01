// Author : Mudassar Tamboli
// Date   : 6/7/2018

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ap_ssid     = "Esp8266AP";
const char *ap_password = "thereisnospoon";

const char *ssid     = "AndroidAP";  //-- TODO
const char *password = "123456!@#$"; //-- TODO

#define GPIO2_LED 2

ESP8266WebServer server(80);


void handleNotFound() {
    server.send(404, "text/html", 
                    "<BR>"
                    "<CENTER><h4>This is a HTTP Web Server running inside ESP8266-01 </h4> "
                    "<BR><font style=\"background-color:white; color:blue\">Page Not Found</font>"
                    "</CENTER>");
                                
}

void handleHtmlRoot() {

  if (digitalRead(GPIO2_LED) == LOW) {
  
      server.send(200, "text/html", 
                        "<BR>"
                        "<CENTER><h4>This is a HTTP Web Server running inside ESP8266-01 </h4> "
                        "GPIO2 Led is <font style=\"background-color:red; color:white\">OFF</font>"
                        "<BR><BR><a style=\"background-color:green; color:white\" color=\"green\" href=\"/GPIO2/ON\">ON </a>"
                        "&nbsp;&nbsp;"
                        "<a style=\"background-color:red; color:white\" bgcolor=red href=\"/GPIO2/OFF\">OFF</a>"
                        "<BR>"
                        "</CENTER>");
  } else {
      server.send(200, "text/html", 
                        "<BR>"
                        "<CENTER><h4>This is a HTTP Web Server running inside ESP8266-01 </h4> "
                        "GPIO2 Led is <font style=\"background-color:green; color:white\">ON</font>"
                        "<BR><BR><a style=\"background-color:green; color:white\" color=\"green\" href=\"/GPIO2/ON\">ON </a>"
                        "&nbsp;&nbsp;"
                        "<a style=\"background-color:red; color:white\" bgcolor=red href=\"/GPIO2/OFF\">OFF</a>"
                        "<BR>"
                        "</CENTER>");
    
  }
}

void handleHtmlGpio2On() {
    server.send(200, "text/html", 
                    "<BR>"
                    "<CENTER><h4>This is a HTTP Web Server running inside ESP8266-01 </h4> "
                    "GPIO2 Led is <font style=\"background-color:green; color:white\">ON</font>"
                    "<BR><BR><a style=\"background-color:green; color:white\" color=\"green\" href=\"/GPIO2/ON\">ON </a>"
                    "&nbsp;&nbsp;"
                    "<a style=\"background-color:red; color:white\" bgcolor=red href=\"/GPIO2/OFF\">OFF</a>"
                    "<BR>"
                    "</CENTER>");
    digitalWrite(GPIO2_LED, HIGH);                                                                      
}

void handleHtmlGpio2Off() {
    server.send(200, "text/html", 
                    "<BR>"
                    "<CENTER><h4>This is a HTTP Web Server running inside ESP8266-01 </h4> "
                    "GPIO2 Led is <font style=\"background-color:red; color:white\">OFF</font>"
                    "<BR><BR><a style=\"background-color:green; color:white\" color=\"green\" href=\"/GPIO2/ON\">ON </a>"
                    "&nbsp;&nbsp;"
                    "<a style=\"background-color:red; color:white\" bgcolor=red href=\"/GPIO2/OFF\">OFF</a>"
                    "<BR>"
                    "</CENTER>");
    digitalWrite(GPIO2_LED, LOW);                                   
}


void handlePlainRoot() {

   if (digitalRead(GPIO2_LED) == LOW) {
      server.send(200, "text/plain", 
                        "OFF");
   } else {
      server.send(200, "text/plain", 
                        "ON");
   }
}

void handlePlainGpio2On() {

    digitalWrite(GPIO2_LED, HIGH);
    server.send(200, "text/plain", 
                     "GPIO2 is ON");
}

void handlePlainGpio2Off() {

    digitalWrite(GPIO2_LED, LOW);
    server.send(200, "text/plain", 
                        "GPIO2 is OFF");
}

void initWebServer() {

  server.on("/plain",           handlePlainRoot);
  server.on("/plain/GPIO2/ON",  handlePlainGpio2On);
  server.on("/plain/GPIO2/OFF", handlePlainGpio2Off);

  server.on("/",          handleHtmlRoot);
  server.on("/GPIO2/ON",  handleHtmlGpio2On);
  server.on("/GPIO2/OFF", handleHtmlGpio2Off);
  
  server.onNotFound(handleNotFound);
  
  server.begin();
}

void initWifiStation() {

    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);    
    Serial.print("\nConnecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
       delay(1000);        
       Serial.print(".");
    }
    Serial.println(String("\nConnected to the WiFi network (") + ssid + ")" );

    Serial.print("\nStation IP address: ");
    Serial.println(WiFi.localIP()); 

}

void initWifiAP() {

    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ap_ssid, ap_password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
}

void setup() {
  
  pinMode(GPIO2_LED, OUTPUT);
  Serial.begin(115200);
  initWifiStation();
  initWifiAP();
  initWebServer();//it will output plain text which is not decorated by html tags
  Serial.print("\nHTTP server started");

}

void loop() {
  server.handleClient();
}
