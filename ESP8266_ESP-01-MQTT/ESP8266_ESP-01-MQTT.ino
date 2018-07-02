
// Author : Mudassar Tamboli
// Date   : 6/8/2018

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid         = "AndroidAP";
const char* password     = "123456!@#$";
const char* mqttServer   = "m14.cloudmqtt.com";
const int   mqttPort     = 14205;
const char* mqttUser     = "htmbxcyz";
const char* mqttPassword = "rH2_IZj43nDy";

#define PUB_GPIO2_STATUS "esp8266/gpio2/status"
#define SUB_GPIO2_ACTION "esp8266/gpio2/action"

#define GPIO2_LED 2
 
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

 
void loop() {
    mqttClient.loop();
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
}
 
void initMQTTClient() {

    // Connecting to MQTT server
    mqttClient.setServer(mqttServer, mqttPort);
    mqttClient.setCallback(PubSubCallback);
    while (!mqttClient.connected()) {
        Serial.println(String("Connecting to MQTT (") + mqttServer + ")...");
        if (mqttClient.connect("ESP8266Client", mqttUser, mqttPassword)) {
            Serial.println("MQTT client connected");  
        } else {
            Serial.print("\nFailed with state ");
            Serial.println(mqttClient.state());
            
            if (WiFi.status() != WL_CONNECTED) {
               initWifiStation();
            }
            
            delay(2000);
        }
    }


    // Declare Pub/Sub topics
    mqttClient.publish(PUB_GPIO2_STATUS, "OFF");
    mqttClient.subscribe(SUB_GPIO2_ACTION);

}

void setup() {
 
    Serial.begin(115200);

    // GPIO2 is set OUTPUT
    pinMode(GPIO2_LED, OUTPUT);

    initWifiStation();
    initMQTTClient();
    
}
 
void PubSubCallback(char* topic, byte* payload, unsigned int length) {

    String strTopicGpio2Action = SUB_GPIO2_ACTION;
    String strPayload = "";
    String strON = "ON";
    String strOFF = "OFF";

    Serial.print("Topic:");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        strPayload += (char)payload[i];
    }
    Serial.println();
    Serial.println("-----------------------");
 
    if (strTopicGpio2Action == topic) {
        if (strON == strPayload) {
            digitalWrite(GPIO2_LED, HIGH);
            mqttClient.publish(PUB_GPIO2_STATUS, "ON");
        } else if (strOFF == strPayload) {
            digitalWrite(GPIO2_LED, LOW);
            mqttClient.publish(PUB_GPIO2_STATUS, "OFF");
        }
    }

}

