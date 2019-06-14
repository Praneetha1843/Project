#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define SSD1306_LCDHEIGHT 64
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
#include "DHT.h"
#include <Servo.h>
Servo myservo;
#define  DHTPIN D0
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
int temp_led=D7;
int fan_led=D8;
#define echopin D3
#define trigpin D4
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial
#include <SimpleTimer.h>
SimpleTimer timer;


char auth[] = "e77812f7ef6447a094f30e0fad83f02b";
const char* ssid = "kevin";
const char* password = "12345678i";
char pass[] = "12345678i";



#define ORG "gawt0d"
#define DEVICE_TYPE "praneetha"
#define DEVICE_ID "1843"
#define TOKEN "123456789"
String command;



char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char pubtopic[] = "iot-2/evt/poultry/fmt/json";
char topic[] = "iot-2/cmd/poultry/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
//Serial.println(clientID);

WiFiClient wifiClient;
void callback(char* topic, byte* payload, unsigned int payloadLength);
PubSubClient client(server, 1883, callback, wifiClient);
void setup() {
  Serial.begin(9600);
  dht.begin();
//  pinMode(light,OUTPUT);
  Serial.println();
  wifiConnect();
  mqttConnect();
    Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(temp_led,OUTPUT);
  pinMode(fan_led,OUTPUT);
  pinMode(D3,INPUT);
  pinMode(D4,OUTPUT);
  myservo.attach(D5);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,10);
  display.print("poultry farm");
  Serial.println("poultry farm");
  display.display();
     myservo.write(0);                           
  delay(100);
}

 BLYNK_WRITE(10)
{
if (param.asInt()) {
digitalWrite(fan_led, HIGH);
command="LIGHTON";
} else {
digitalWrite(fan_led, LOW);
command=="LIGHTOFF";
}
}

void loop() {
  //float h = dht.readHumidity();
  //float t = dht.readTemperature();
  //Serial.print("Humidity: ");
  //Serial.println(h);
  //Serial.print("Temperature: ");
  //Serial.println(t);
  //if (isnan(h) || isnan(t)) 
    //Serial.println("Failed to read from DHT sensor!");
    //return;
    
   Blynk.run(); 
  timer.run();


  
  //gas sensor start
  int analogSensor = analogRead(A0);
  float h = analogSensor;
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  delay(500);
 if (analogSensor >200)
  {
   display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Please switch on the Exhaust fans");
   display.display();
      char k[]=("Please switch on the Exhaust fans");
 Blynk.virtualWrite(V7, k);
      delay(100);
      Serial.println("Please switch on the Exhaust fans");
  }
else
  {
     display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Please switch off the Exhaust fans");
   display.display();
         char o[]=("Please switch off the Exhaust fans");
 Blynk.virtualWrite(V7, o);
   Serial.println("Please switch off the Exhaust fans");
    
    }
  delay(100);
  //gas sensor end
  
  //dht11 start
  float t = dht.readTemperature();
  Serial.println(t);
  delay(500);
 Blynk.virtualWrite(V3, t);
  
if(t>35)
  {
   display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Temperature is HIGH");
   display.display();
   Serial.println("Temperature is HIGH");
   digitalWrite(temp_led, HIGH); //Red
   delay(200);
      char i[]=("Temperature is High");
 Blynk.virtualWrite(V5, i);
      delay(100);
  }
else if(t<25)
  {
   display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Temperature is LOW");
   display.display();
   Serial.println("Temperature is LOW");
      char h[]=("Temperature is Low");
 Blynk.virtualWrite(V5, h);
      delay(100);
  }
else
{
  digitalWrite(temp_led,  LOW);
     char j[]=("Temperature is Normal");
 Blynk.virtualWrite(V5, j);
      delay(100);
}
//dht11 end

//ultrasoinc start
  digitalWrite(D4,HIGH);
  delay(1000);
  digitalWrite(D4,LOW);
  int duration = pulseIn(echopin,HIGH);
  int distance = (duration/2)*0.0343;
  Serial.println("distance:  ");
  Serial.println(distance);
  delay(500);
if(distance<5)
 {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,10);
  display.print("Please fill up the water tanks");
  display.display();
  Serial.println("Please fill up the water tanks");
  delay(500);
   char w[]=("Fill up the Tanks");
 Blynk.virtualWrite(V1, w);
      delay(100);
   myservo.write(90);
  delay(2000);
 }
else if(distance>10)
 {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,10);
  display.print("Water tank is full");
  display.display();
  Serial.println("Water tank is full");
  delay(500);
   char a[]=("Water Quantity is Optimal");
 Blynk.virtualWrite(V1, a);
      delay(100);
   myservo.write(0);                           
  delay(100);
 }
 else
 {
 }
 //ultrasoinc end
 
  //servo start
  //myservo.write(90);
 // delay(2000);
 // myservo.write(0);                           
  //delay(100);
 
 //servo end

  

   if(!client.loop()) {
    mqttConnect();
  }
  PublishData(t,h);
delay(100);
}

void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(topic)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* topic, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); Serial.println(topic);

  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
Serial.println(command);
if(command == "LIGHTON"){
  //digitalWrite(light,LOW);
  Serial.println("Light is ON");
  
}
else if(command == "LIGHTOFF"){
  Serial.println("Light is OFF");
  //digitalWrite(light,HIGH);
}
command ="";
}
void PublishData(float t,float h){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"temperature\":";
  payload += t;
  payload+="," "\"gas\":";
  payload += h;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(pubtopic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}

