#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"


//***********************************************************************
//transfer from arduino to nodemcu and opposite
#include<SoftwareSerial.h>
SoftwareSerial Nodemcu_softSerial(D1,D2);//rx,tx

//store data come from arduino
char c;
String datain;
//store data come from firebase
  //led
String ll;
  //door
String hd,md;

// store temperture come form arduino into firebase
String temp;
//varable of img
String img;

//*****************************************************

// Insert your network credentials
#define WIFI_SSID "OnePlus 7T"
#define WIFI_PASSWORD "67eca7e93062"          



#define API_KEY "AIzaSyBjXhUX42hut-GaxN_Q-1bRlqBjuANplpE "

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "smarthomeapp-3a9d9-default-rtdb.firebaseio.com/"
//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
// //*
String count = "led Garage close";
bool signupOK = false;
String intValue;
float floatValue;

int num=0;



void setup(){
  // serial port 
  Serial.begin(115200);
  //*****************
  // speed of arduino and nodemcu
  Nodemcu_softSerial.begin(9600);

  //************
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  
  //*****************

  // get data form arduino and store in variable
  while( Nodemcu_softSerial.available()>0){
    c= Nodemcu_softSerial.read();
    if(c=='\n'){break;}
    else{ datain+=c;}
  }if(c=='\n'){
    Serial.println(datain);
    temp=datain;
    c=0;
    datain="";

  }
  if (Firebase.ready() && signupOK){
    // initial value in firebase from varibles
    while(num<1){
        // inital value of led living room
      if (Firebase.RTDB.setString(&fbdo, "ledLivingRoom", "led Living Room close")){
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
      //*******************************************
      
        // inital value of house door
      if (Firebase.RTDB.setString(&fbdo, "HouseDoor", "House Door close")){
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
        // inital value of main door
      if (Firebase.RTDB.setString(&fbdo, "mainDoor", "Main door close")){
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
      

      num=1;

    }
    //**********************************
      //send data to firebase
    // if (Firebase.RTDB.setString(&fbdo, "temp", temp)){

    //   Serial.println("PASSED");
    //   Serial.println("PATH: " + fbdo.dataPath());
    //   Serial.println("TYPE: " + fbdo.dataType());

    // }
    // else {

    //     Serial.println("FAILED");
    //     Serial.println("REASON: " + fbdo.errorReason());

    //   }
    //*************************************
    //get data from firebase and send to arduino 
    //led
    if (Firebase.RTDB.getString(&fbdo, "/ledLivingRoom")) {

      if (fbdo.dataType() == "string") {

        ll = fbdo.stringData();
        Serial.println(ll);
        Nodemcu_softSerial.print(ll);
        Nodemcu_softSerial.print(" \n");
        // delay(100);

      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }

    //***********************************
    //door
   

    if (Firebase.RTDB.getString(&fbdo, "/HouseDoor")) {

      if (fbdo.dataType() == "string") {

        hd = fbdo.stringData();
        Serial.println(hd);
        Nodemcu_softSerial.print(hd);
        Nodemcu_softSerial.print(" \n");
        // delay(100);

      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }

   
    
    if (Firebase.RTDB.getString(&fbdo, "/mainDoor")) {

      if (fbdo.dataType() == "string") {

        md = fbdo.stringData();
        Serial.println(md);
        Nodemcu_softSerial.print(md);
        Nodemcu_softSerial.print(" \n");
        // delay(100);

      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getString(&fbdo, "/Img")) {

      if (fbdo.dataType() == "string") {

        img = fbdo.stringData();
        Serial.println(img);
        Nodemcu_softSerial.print(img);
        Nodemcu_softSerial.print(" \n");
        // delay(100);

      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }

    

    

  }

}