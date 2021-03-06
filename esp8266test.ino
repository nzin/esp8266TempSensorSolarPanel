 /*
 *  Simple HTTP get webclient test
 */
 
#include <ESP8266WiFi.h>

#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
 
const char* ssid     = "tomtom2";
const char* password = "xxxxxxx";
 
const char* host = "www.elanprod.com";
const int sleepTimeS = 1200;

void setup() {
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);

  Serial.begin(115200);

  dht.begin();
  delay(1000);
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

//  ESP.deepSleep(1, WAKE_RF_DEFAULT);
//  WiFi.mode(WIFI_STA);  
//  wifi_station_connect();
  WiFi.begin(ssid, password);

  int count=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    count++;
    if (count==20) {
      ESP.deepSleep(1, WAKE_RF_DEFAULT);
    }
  }

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());



  delay(5000);
  /*
   * Read the sensor
   */
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");




  /*
   * Send the info
   */
  WiFiClient client;
  const int httpPort = 80;
  if (client.connect("blog.elanprod.com", httpPort)) {
    client.print(String("GET /setTemp.php?temp=") + String(int(t)) +String("&hum=") +String(int(h)) +String("&heatindex=") +String(int(hic)) +" HTTP/1.1\r\n" +
               "Host: blog.elanprod.com\r\n" + 
               "Connection: close\r\n\r\n");
  }
  
  
  digitalWrite(0, LOW);
  delay(5000);
  digitalWrite(0, HIGH);
  delay(500);

//  WiFi.disconnect(); 
//  WiFi.mode(WIFI_OFF);
//  WiFi.forceSleepBegin();

//  ESP.deepSleep(sleepTimeS * 1000000,WAKE_RF_DISABLED);


//  WiFi.disconnect(); 
//  WiFi.mode(WIFI_OFF);
//  WiFi.forceSleepBegin();
//  delay(1);
//  ESP.deepSleep(sleepTimeS * 1000000, WAKE_RF_DEFAULT);

//  WiFi.forceSleepBegin();
//  delay(1);
  ESP.deepSleep(sleepTimeS * 1000000, WAKE_RF_DEFAULT);
}

void loop() {
  WiFi.forceSleepWake();
  delay(1);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("> IP address: ");
  Serial.println(WiFi.localIP());
  
  
  digitalWrite(0, HIGH);
  delay(500);
  digitalWrite(0, LOW);
  delay(500);


  WiFi.forceSleepBegin();
  delay(1);
  ESP.deepSleep(sleepTimeS * 1000000, WAKE_RF_DEFAULT);


}

