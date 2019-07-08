#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>
#include <Password.h>
#include <Keypad.h>
#include <LCD.h>

#define ledHijau 6
#define ledMerah  7

Servo myservo;
Servo myservo2;
Ultrasonic ultrasonic (12, 13) ;//trigger and echo pin ditace
Ultrasonic ultrasonic2(10, 11) ;//trigger and echo pin isi tong
Password password = Password( "45" ); //Ubah password
int readsuccess;
int distance;
int isitong;
int pos = 0; 
int pos2 = 0;
const byte baris = 4; //Baris
const byte kolom = 3; //Kolom

char keys[baris][kolom] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};


byte barisPin[baris] = {0,A0,2,3};
byte kolomPin[kolom]= {4,5,A1};


 // Buat objek keypad
Keypad keypad = Keypad( makeKeymap(keys), barisPin, kolomPin, baris, kolom );
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);  // set the LCD address to 0x27 for a 16 chars and 2 line display



void setup() {
 
  lcd.begin(16,2);
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
 
  lcd.clear();
  //keypad.addEventListener(keypadEvent);
  Serial.begin(9600);
  myservo.attach(9); 
  myservo2.attach(8);
  myservo.write(0);
  myservo2.write(90);
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad    
  pinMode(ledHijau,OUTPUT);
  pinMode(ledMerah,OUTPUT);
 }

void loop() {
  // put your main code here, to run repeatedly:

menu:
while(1)
{
  lcd.setCursor(3,0); 
  lcd.print("TONG KOSONG");
  digitalWrite(ledHijau,HIGH); 
  delay(50);
  Serial.println(keypad.getKey());
  distance = ultrasonic.read(); // untuk membuka tong
  isitong = ultrasonic2.read();// membaca isi
  Serial.println(distance);
  Serial.println(isitong);  
  if (distance <= 30 && isitong >6 ){
    myservo.write(180);   
    digitalWrite(ledHijau,HIGH); 
    digitalWrite(ledMerah,LOW);// tell servo to go to position in variable 'pos'
    delay(500); 
  }

  else if (distance <= 30 && isitong <=6 ){
    lcd.clear();
    myservo.write(0);              // tell servo to go to position in variable 'pos'
    delay(50); 
    digitalWrite(ledHijau,LOW); 
    digitalWrite(ledMerah,HIGH);
    delay(300);
    goto menu1;
  }
  else {
    myservo.write(0);              // tell servo to go to position in variable 'pos'
    delay(5);
  }
}

menu1:
while(1)
{
  isitong = ultrasonic2.read();// membaca isi

  Serial.println(isitong);
  lcd.setCursor(3,0); 
  lcd.print("TONG PENUH");
  delay(50);
  const int ace =  keypad.getKey();
  keypad.getState();
  keypad.addEventListener(keypadEvent);
  delay(50);


if (pos2 == 90 && isitong >=9 ){
   lcd.clear();
   digitalWrite(ledMerah,LOW);
   delay(300);
   goto menu;
  } 
}  
}


void keypadEvent(KeypadEvent eKey){
 
  switch (keypad.getState()){
    case PRESSED:
  lcd.setCursor(3,1);
  lcd.print(eKey);
  switch (eKey){
    case '*': checkPassword(); break;
    case '0': password.reset();lcd.clear(); break;
    case '#':tutup();break;
    default: password.append(eKey);
     }
  }
}

void checkPassword(){
  if (password.evaluate()){
   Serial.println("Success");
   lcd.setCursor(3,1);
   lcd.print("Akses Diterima!");
   myservo2.write(0); 
    
    // tell servo to go to position in variable 'pos'
   delay(2000); 
   lcd.clear();
    //Add code to run if it works
  }else{
    Serial.println("kode salah");
    lcd.setCursor(3,1);
    lcd.print("Akses Ditolak!");
    delay(1000);
    lcd.clear();
    //add code to run if it did not work
  }
}
void tutup(){
   for (pos2 = 0; pos2 = 90; pos2 += 90) { 
    myservo2.write(pos2);  
     Serial.println(pos2);
    delay(300);
    return; 
   } 
}

