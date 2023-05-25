

#include <Arduino.h>
#include <ESP32Servo.h>
#include "CTBot.h"
CTBot miBot;
#include <Espalexa.h>
#include <ESPAsyncWebServer.h> 
#include <Arduino_JSON.h>
#include "SPIFFS.h"
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Andre"
#define WIFI_PASSWORD "SeMeAcaboxd"
const String toke = "5701650772:AAFyhtQ8X9PlSTbMp4X4fwnjFKibQbm4ynU";

// Insert Firebase project API Key
#define API_KEY "AIzaSyBKRdafFxngsUZZ1ZqXqWpHJfub4UuMzFY"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "polfaalejandro@gmail.com"
#define USER_PASSWORD "123456"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://alimentador-dbc36-default-rtdb.firebaseio.com/"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

Servo myservo;
Espalexa alexita;

void FuncionAlimenta(uint8_t brightness);
void FuncionBebe(uint8_t brightness);


int servoPin=26;
String servo1;
int a=0;

// Variables to save database paths
String databasePath;
String tempPath;
String humPath;
String presPath;
String presPath1;
String ipPath;
String pathrssi;
String pathssid;
String pathstatus;
String pathhost;
String ledPath;
String bombaPath;
String chartPath;
int LED=21;
// BME280 sensor
//Adafruit_BME280 bme; // I2C
float temperature;
float humidity;
float pressure;
float pressures;
float rssi;
float contador =1.0;

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 5000;

// Initialize BME280
/*void initBME(){
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}*/

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Write float values to the database
void sendFloat(String path, float value){
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)){
    Serial.print("Writing value: ");
    Serial.print (value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}



void sendstring(String path, String value){
  if (Firebase.RTDB.setString(&fbdo, path.c_str(), value)){
    Serial.print("Writing value: ");
    Serial.print (value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}


void Alimenta(){
    readBD(chartPath);
    for (int pos = 0; pos <= 120; pos += 1) { 
          myservo.write(pos);    
          delay(15);             
  }
 
}


void Bebe(){
  digitalWrite(LED, HIGH);
  delay(3000);
  digitalWrite(LED, LOW);
}


void readBD(String path){
  String readIncoming ="";
  if (Firebase.RTDB.getString(&fbdo, path.c_str())){
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
    if(fbdo.dataType()=="int"){
      readIncoming =fbdo.stringData();
      Serial.println("PATHhjlsdbafhjbasdfhabsdfibioasbdfyhasdfbhshdfdd: " + readIncoming);
      sendFloat(path, readIncoming.toInt()+1);
       Serial.println("DATAasxd: " + readIncoming);

     }
    }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}



void readDataxd(String path){
  String readIncoming ="";
  if (Firebase.RTDB.getString(&fbdo, path.c_str())){
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
    if(fbdo.dataType()=="string"){
      readIncoming =fbdo.stringData();
       Serial.println("DATAasxd: " + readIncoming);
      if(readIncoming=="ON"){
          Bebe();
        //digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
        else{
          //alimentaa();
        // digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)
        }
     }
    }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

void readData(String path){
  String readIncoming ="";
  if (Firebase.RTDB.getString(&fbdo, path.c_str())){
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
    if(fbdo.dataType()=="string"){
      readIncoming =fbdo.stringData();
       Serial.println("DATA: " + readIncoming);
      if(readIncoming=="ON"){
          Alimenta();
        //digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
        else{
          //alimentaa();
        // digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)
        }
     }
    }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}




void setup(){
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  // Initialize BME280 sensor
  //initBME();
  initWiFi();

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  //aaaaaaaaaa
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid;

  // Update database path for sensor readings
  tempPath = databasePath + "/temperature"; // --> UsersData/<user_uid>/temperature
  humPath = databasePath + "/humidity"; // --> UsersData/<user_uid>/humidity
  presPath = databasePath + "/pressure"; // --> UsersData/<user_uid>/pressure
  presPath1 = databasePath + "/pressures"; // --> UsersData/<user_uid>/pressure
  ipPath = databasePath + "/direccionip";
  pathrssi=databasePath + "/rssi";
  pathhost=databasePath + "/host";
  pathssid=databasePath + "/ssid";
  pathstatus=databasePath + "/status";
  ledPath=databasePath + "/led";
  bombaPath=databasePath + "/bomba";
  chartPath=databasePath + "/chart";
    miBot.wifiConnect(WIFI_SSID,WIFI_PASSWORD );

  miBot.setTelegramToken(toke);

  if (miBot.testConnection()) {
    Serial.println("\n Conectado");
  }
  else {
    Serial.println("\n Problemas Auxilio");
  }

// alexa

  alexita.addDevice("Alimentador", FuncionAlimenta);
  alexita.addDevice("Bebedero", FuncionBebe);
  alexita.begin();
  myservo.setPeriodHertz(50); 
  myservo.attach(servoPin);

}

void loop(){
  // Send new readings to database
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    

    // Get latest sensor readings
   /* temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure()/100.0F;*/

    ///////COMENTAN ESTA SECCION SI DESEAN UTILIZAR EL BME////////////////
      if(contador>20){contador = 1.0;}
      contador=contador+1;
    temperature = contador*0.1;
    humidity = contador*0.2;
    pressure = contador*0.3;


    // Send readings to database:
    sendFloat(tempPath, temperature);
    sendFloat(humPath, humidity);
    sendFloat(presPath, pressure);
    sendFloat(presPath1, pressures);
    sendFloat(pathrssi, WiFi.RSSI());
    sendstring(ipPath,WiFi.localIP().toString().c_str());
    sendstring(pathhost,String(WiFi.getHostname()));
    sendstring(pathstatus,String(WiFi.status()));
       sendstring(pathssid,String(WiFi.SSID()));

    if(a==0){
      sendFloat(chartPath, 0);
      a=1;
      
    }
    
    

    readData(ledPath);
    readDataxd(bombaPath);
    
  }

      TBMessage msg;
    alexita.loop();
     delay(1);

  if (CTBotMessageText == miBot.getNewMessage(msg)) {
   // Serial << "Mensaje: " << msg.sender.firstName << " - " <<  msg.text << "\n";

    if (msg.text.equalsIgnoreCase("Alimentar")) {
      Serial.println("Alimentando a la mascota");
      Alimenta();
      miBot.sendMessage(msg.sender.id, "Hambre saciada :)");
    }
    else if (msg.text.equalsIgnoreCase("Beber")) {
      Serial.println("Apagar Led");
      Bebe();
      miBot.sendMessage(msg.sender.id, "Sed Saciada");
    }
    else {
     miBot.sendMessage(msg.sender.id, "Bienvenid@ " + msg.sender.firstName + ",intenta usar: Alimentar o Beber ");
    }
  }

  delay(250);

}
void FuncionAlimenta(uint8_t brightness) {

  if (brightness) {
    Alimenta();
    Serial.println(" Encender ");
  }

}

void FuncionBebe(uint8_t brightness) {
  Serial.print("Funcion Bebe -");
  Bebe();
  if (brightness) {
    Serial.println(" Encender ");
  }
 

}
