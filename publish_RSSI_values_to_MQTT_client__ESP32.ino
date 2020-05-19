/*
 Basic ESP8266 MQTT publish client example for a moisture sensor
*/
#include <WiFi.h>
#include <PubSubClient.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#define LED 2
int states=LOW;
int scanTime = 5; //In seconds
int n=0;
BLEScan* pBLEScan;

int counter=0;
int rssiarray[100]={};
const char* addarray[100]={};
String a1="{";
String a2="\"";
String a3="id";
String a4=":17";
String a5="}";
String a7=",";
String keyvalue=a1+a2+a3+a2+a4;


// Update these with values suitable for your network.
const char* ssid = "Nspot";
const char* password = "123456789";
const char* mqtt_server = "mqtt.eclipse.org";

WiFiClient espClient;
PubSubClient client(espClient);

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
   void onResult(BLEAdvertisedDevice advertisedDevice) {
     int rssi=advertisedDevice.getRSSI();
     String address=advertisedDevice.getAddress().toString().c_str();
     counter+=1;
     keyvalue+=a7;
     keyvalue+= "\""+address+"\""+":"+String(rssi);
   }
   
};

void setup_wifi() {
  // Connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  Serial.println("In reconnect...");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(3000);
    }
  }
}

void setup() {
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  // put your main code here, to run repeatedly:
  

  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  
//  String a6="}";
  keyvalue=keyvalue+"}d";
  counter=0;

  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  int m = foundDevices.getCount();
  Serial.println("Scan done!");
  
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory

  char msg[1000];
  char msg1[1000];

//  sprintf(msg1,"No of devices : %i",m);
//  client.publish("out", msg1);
  
  String themsg;
  if (!client.connected()) {
    reconnect();
  }

//  for (int i = 0; i < m; i++) {
//    Serial.println(rssiarray[i]);
//    Serial.println(addarray[i]);
//    
//    sprintf(msg,"%i",rssiarray[i]);
////    Serial.println(addarray[i]);
////    sprintf(msg,addarray[i]);
//    client.publish("out", msg);
//  }

  int len=keyvalue.length();
  Serial.print("len: ");
  Serial.println(len);
  char charBuf[len];
  keyvalue.toCharArray(charBuf, len);
//  sprintf(msg,keyvalue);
  client.publish("EN3250/ESP32", charBuf);
  
//  Serial.println(keyvalue);
  Serial.println(charBuf);
  keyvalue=a1+a2+a3+a2+a4;
  states=!states;
  digitalWrite(LED,states);
  delay(3000);
}
