//database veri gönderme ve alma
// 1 gelirse lamba
// 2 gelirse fan
// 3 gelirse servo pencere 
// 4 gelirse sifre

// ldr 1
// sıcaklık kendi değeri 
// yagmur 3



/****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"                          // alma işlemini bi değişiklik olduğu durumda yapıyor
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "HUAWEI Mate 20 lite"
#define WLAN_PASS       "eac6a6c8217f"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "azimekr"
#define AIO_KEY         "aio_vPLG32hKnjMuVeYEn1zjPp30ZmRI"


/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'webegiden' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish webegiden = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/webegiden");

// Setup a feed called 'webdengelen' for subscribing to changes.
Adafruit_MQTT_Subscribe webdengelen = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/webdengelen");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

int gelen=0;
int lambadeger=1;

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for webdengelen feed.
  mqtt.subscribe(&webdengelen);
}



void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &webdengelen) {
     /* Serial.print(F("Got: "));
      Serial.println((char *)webdengelen.lastread);*/
       char *oku;
      oku = (char*)webdengelen.lastread;
      int oku1 = *oku;
      Serial.println(oku1);

      if(oku1 == 49) // lamba değeri 1 ken ascii karşılığı 49 oluyor  
      {
        Serial.write(1);
        
      }

      if(oku1 == 50) // fan değeri 2 ken ascii karşılığı 50 oluyor  
      {
        Serial.write(2);
        
      }

      if(oku1 == 51) // lamba değeri 3 ken ascii karşılığı 49 oluyor  
      {
        Serial.write(3);
        
      }

      if(oku1 == 52) // lamba değeri 4 ken ascii karşılığı 49 oluyor  
      {
        Serial.write(4);
        
      }
    }
  }

  // Now we can publish stuff!

  if (Serial.available()>0)
  {
     gelen = Serial.read();
     webegiden.publish(gelen);
     Serial.println(gelen);
     delay(2000);
  }
 

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
