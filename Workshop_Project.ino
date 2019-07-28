
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

const double VCC = 3.3;             // NodeMCU on board 3.3v vcc
const double R2 = 10000;            // 10k ohm series resistor
const double adc_resolution = 1023; // 10-bit adc

const double A = 0.001129148;   // thermistor equation parameters
const double B = 0.000234125;
const double C = 0.0000000876741;

#define led D1
int s;
// Set these to run example.
#define FIREBASE_HOST "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#define FIREBASE_AUTH "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#define WIFI_SSID "xxxxxxxxxx"
#define WIFI_PASSWORD "xxxxxxxxx"

void setup() {
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}


void loop() {
   double Vout, Rth, temperature, adc_value; 

  adc_value = analogRead(A0);
  Vout = (adc_value * VCC) / adc_resolution;
  Rth = (VCC * R2 / Vout) - R2;


  temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)),3))));   // Temperature in kelvin

  temperature = temperature - 273.15;  // Temperature in degree celsius
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" degree celsius");
  delay(500);
  Firebase.setFloat("Temperature",temperature );
  s=Firebase.getString("state").toInt();
  Serial.println(s);
  if(s==1){
    digitalWrite(led,HIGH);
  }
  if(s==0){
    digitalWrite(led,LOW);
  }
 
}
