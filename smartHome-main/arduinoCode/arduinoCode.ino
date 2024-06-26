//data
#include<SoftwareSerial.h>
SoftwareSerial Arduino_softSerial(10,11);//rx,tx

// data
char c;
String datain;

//keypad
#include <Keypad.h>
// تعريف حجم وترتيب المفاتيح في لوحة المفاتيح
const byte ROWS = 4; // عدد الصفوف في لوحة المفاتيح
const byte COLS = 3; // عدد الأعمدة في لوحة المفاتيح
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// تحديد أرجحة المتاحفة المستخدمة لتوصيل لوحة المفاتيح بالأردوينو
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8};

//إعداد لوحة المفاتيح
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
//variable
char pass[6];
char storePass[] = "123456";
int valueKeypad, count, attempts = 0;
//key of keypad
char key;
//variable of interface of passward
int open=0;
//main door
//motor

//door
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int angle=0;

//variable of main door
int maindoor=1;

//variable of buzzer
//14
int buz = 9;
//dht



//dht
#include <DHT.h>
//9
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHT11);
float temp=0.0;
float hum=0.0;
//variable to control of high temp
int hightemp=0;

//led

//variable of led in 
int ledLivingRoom =17;

//variable of interface
int lLivingRoom=0;

//variable of check of led
int LL=0;



//variable of Photoresistor
int valuePhotoresistor;

int ledPhotoresistor = 27;
//for check 
int LP=0;
//
//Img
int Img=0;

// imgcheck
int Imgopen=0;



void setup() {
  //serial
  Serial.begin(115200);
  //speed of transfer data 
  Arduino_softSerial.begin(9600);

 ///
  //led
  pinMode(ledLivingRoom,OUTPUT);
 ///
  //door
  myservo.attach(13); 
  //puzzer
  pinMode(buz ,OUTPUT);
 //*****************************
  //ledPhotoresistor
  pinMode (ledPhotoresistor , OUTPUT);



}

void loop() {
   if(Arduino_softSerial.available()>0){

    while( Arduino_softSerial.available()>0){

      c= Arduino_softSerial.read();
      if(c=='\n'){break;}
      else{ datain+=c;}

  } 
  if(c=='\n'){
    Serial.println(datain);
      //data led receive
    if(datain[2]=='l'&&datain[6]=='L'&&datain[18]=='o'){
    lLivingRoom=1;
   }
   else if(datain[2]=='l'&&datain[6]=='L'&&datain[18]=='c'){
    lLivingRoom=0;
   } 
   
    else if(datain[2]=='H'&&datain[8]=='D'&&datain[13]=='o'){
      // HouseD  =1;
     open=1;

    }
    else if(datain[2]=='H'&&datain[8]=='D'&&datain[13]=='c'){
      // HouseD  =0;
      open=0;

    }
    //Img 
    else if(datain[0]=='I'&&datain[4]=='o'){
      
     Img=1;

    }
    else if(datain[0]=='I'&&datain[4]=='c'){

      Img=0;

    }
   
      Serial.println(datain);
      c=0;
     datain="";
  }
   else{
    Serial.println("not vailable");
   }

  }
  delay(250);
  



  //tempurature
  temper();

  //led
  LEDLivingRoom();

  //Photoresistor
  photoresistor();


   if(open==1){
     passwor();
  }

    //  opendoor()
    // 

  if(Img==1){

    if(Imgopen==0){

      Imgopendoor();
      Imgopen=1;
    }
      

  }else if(Img==0){
    if(Imgopen==1){
      Imgclosedoor();
      Imgopen=0;

    }
     
   }
  


}





void LEDLivingRoom(){
  if(lLivingRoom==1){
    if(LL==0){
      digitalWrite(ledLivingRoom,HIGH);
      // Arduino_softSerial.print("led Living Room open\n");
      // delay(500);
      LL=1;

    }
    
  }else{
    if(LL==1){
       digitalWrite(ledLivingRoom,LOW);
      // Arduino_softSerial.print("led Living Room close \n");
      // delay(500);
      LL=0;

    }
   
  }
}

//photoresistor
void photoresistor() {
  valuePhotoresistor = analogRead(A2);
  Serial.print("Photoresistor value is:  ");
  Serial.println(valuePhotoresistor);
  int i=0;

 if (valuePhotoresistor < 300) {

    if(LP==0){
      // Serial.println("open..........");
      

      digitalWrite(ledPhotoresistor ,HIGH);
      // Arduino_softSerial.print("Gardan led open \n");
      // for(int i=0 ;i<255;i++){
      //   analogWrite(ledPhotoresistor,i);
      //   delay(10);

      // }
      // // delay(500);
      // while (i<255){
      //   analogWrite(27,map(50,0,100,0,255));
      //   i++;
      //   // delay(10);
      // }

      LP=1;

    }
    
  }
  else {
    if(LP==1){
      // Serial.println("close..........");
      digitalWrite(ledPhotoresistor ,LOW);
    //  delay(500);
    //  for(int i=255 ;i>0;i--){
    //     analogWrite(ledPhotoresistor,i);
    //     delay(10);

    //   }

      // while(i>0){

      //     analogWrite(27,map(9,0,100,0,255));
      //   i--;
      //   // delay(50);
      // }
     
      
     LP=0;

    }
   
  }
}



//temperature and fan
void temper(){
  temp=dht.readTemperature();
  hum=dht.readHumidity();
  Serial.println("*");
  Serial.print("Temperature: ");
  Serial.print(temp,1);
  Serial.println(" *C");
  Serial.print("Humidity: ");
  Serial.print(hum,1);
  Serial.println(" %");
  Serial.println("*");

  

  if(temp >32){
    if(hightemp==0){
      digitalWrite(buz ,HIGH);
      hightemp=1;
    }

  }else{
    if(hightemp==1){
      digitalWrite(buz ,LOW);
      hightemp=0;
    }

  }

  

  

}



void passwor(){
  
  while(key !='#' && valueKeypad <= 6){
    key = keypad.getKey();
    if(key>'0'&&key!='#'){
      // فحص ما إذا كانت هناك أي زر مضغوط
      Serial.print(key);
      pass[valueKeypad] = key;
      valueKeypad++;
    }
    }

  if(valueKeypad=6){
    for(int check=0;check<6;check++){
      if(pass[check]==storePass[check]){
        count++;
      }
    }
  if(count==6){
    Serial.println("\n Correct Password ...... ");
    digitalWrite(buz ,LOW);
    opendoor();
    attempts = 0;
    refresh();
  

  }else{
   Pprint();
  }
 }else{
  Pprint();

  }

}

void refresh(){
    valueKeypad = 0;
    count = 0;
    key='0';

}


void Pprint(){
  Serial.println("\n Wrong Password ...... ");
  // closedoor();
  refresh();
  attempts++;
  if(attempts>3){
    digitalWrite(buz , HIGH);
     Serial.println("stop");
  }

}

// void closedoor(){
//     if(maindoor==0){
//       for(angle=180;angle>=0;angle--){
//       myservo.write(angle);
//       delay(15);


//    }
//  maindoor=1;
// }
// }

//open servo with password
void opendoor(){
  if(maindoor==1){

  for(angle=0;angle<=120;angle++){
  myservo.write(angle);
  delay(15);
    }
  delay(1000);

    for(angle=120;angle>=0;angle--){
    myservo.write(angle);
    delay(15);
     }
    maindoor=1;
    open=0;

  }
}

//open servo with face detection
void Imgopendoor(){

  for(angle=0;angle<=120;angle++){
    myservo.write(angle);
    delay(15);
  }


}
//close servo with face detection
void Imgclosedoor(){

    for(angle=120;angle>=0;angle--){
    myservo.write(angle);
    delay(15);
    }
    maindoor=1;
    open=0;

}