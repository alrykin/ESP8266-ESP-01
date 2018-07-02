// Author : Mudassar Tamboli
// Date   : 6/7/2018
 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "AndroidAP";
const char* password =  "123456!@#$"; //--TODO

int port            = 80;
const char* host_ip = "xx.xxx.xx.xxx"; //--TODO

#define GPIO2_LED   2 

void setup() {
  
    Serial.begin(115200);
    pinMode(GPIO2_LED, OUTPUT);
    digitalWrite(GPIO2_LED, LOW)  ;
    initWifiStation();
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

void getLedStatus()
{
    int httpCode;
    if ((WiFi.status() == WL_CONNECTED)) {

        HTTPClient http;
        String url = "http://";
               url += host_ip;
               url += "/status.php";
           
        http.begin(url);
        httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK) {
            String httpResp = http.getString();
            if (httpResp == "ON") {
                digitalWrite(GPIO2_LED, HIGH)  ;   
            }
            if (httpResp == "OFF") {
                digitalWrite(GPIO2_LED, LOW)  ;
            }
            Serial.println(httpResp);
        } else {
            Serial.printf("%s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
        
    } else {
        Serial.println("\nWifi is not connected");
        initWifiStation();
    }
    
}

void loop() 
{
    getLedStatus();
}
