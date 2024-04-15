#include <SPI.h>                                         
#include <nRF24L01.h>                                    
#include <RF24.h>        
#include <Adafruit_NeoPixel.h>
#include <Servo.h>


#define SPEED_1      5 
#define DIR_1        4
 
#define SPEED_2      6
#define DIR_2        7


#define MATRIX_PIN    0
#define LED_COUNT 16


Adafruit_NeoPixel matrix = Adafruit_NeoPixel(LED_COUNT, MATRIX_PIN, NEO_GRB + NEO_KHZ800);


RF24 radio(8, 9); // nRF24L01+ (CE, CSN)
int data[5]; 


Servo rotServo; 
int servoAngle;


void setup(){
  Serial.begin(9600);
    
  radio.begin();                                        
  radio.setChannel(5);
  radio.setDataRate (RF24_1MBPS);
  radio.setPALevel (RF24_PA_HIGH);
  radio.openReadingPipe (1, 0x1234567890LL);
  radio.startListening ();                            
  //  radio.stopListening ();                         


  // motor pins
  for (int i = 4; i < 8; i++) {     
    pinMode(i, OUTPUT);
  }


  matrix.begin();
  rotServo.attach(2);

}


void loop(){
    if(radio.available()){                                
        radio.read(&data, sizeof(data));


        int btnState = data[2];
        int tmblrState = data[3];


        int ptmrVal = data[4];
        servoAngle = map(ptmrVal, 0, 1023, 0, 180); 
        rotServo.write(servoAngle); 
        Serial.println(ptmrVal);


        if (tmblrState == 1) {    
          if (btnState) {
            colorWipe(matrix.Color(255, 0, 0), 50);
            colorWipe(matrix.Color(0, 255, 0), 50);
            colorWipe(matrix.Color(0, 0, 255), 50);
            colorWipe(matrix.Color(0, 0, 0), 50);
          }   
          else {
            crossBlink(matrix.Color(255, 0, 0), 50);
          }
        }
        else {
          Serial.println("no");
          crossBlink(matrix.Color(0, 0, 0), 50);
        }


        // 0 - Forward, 1 - backward, 2 - right, 3 - left
        switch (data[0]) {
          case 0: {
            digitalWrite(DIR_1, HIGH); // set direction
            analogWrite(SPEED_1, 255); // set speed


            digitalWrite(DIR_2, HIGH); 
            analogWrite(SPEED_2, 255); 


            break;
          }
          case 1: {
            digitalWrite(DIR_1, LOW);
            analogWrite(SPEED_1, data[1]); 


            digitalWrite(DIR_2, LOW);
            analogWrite(SPEED_2, data[1]); 


            break;
          }
          case 2: {
            digitalWrite(DIR_1, HIGH); 
            analogWrite(SPEED_1, data[1]); 


            digitalWrite(DIR_2, LOW);
            analogWrite(SPEED_2, data[1]); 


            break;
          }
          case 3: {
            digitalWrite(DIR_1, LOW); 
            analogWrite(SPEED_1, data[1]); 


            digitalWrite(DIR_2, HIGH); 
            analogWrite(SPEED_2, data[1]); 


            break;
          }
          case 4: {
            analogWrite(SPEED_1, 0); 
            analogWrite(SPEED_2, 0); 


            break;
          }
        }        
    }
}


void crossBlink(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < matrix.numPixels(); i++) {
    if (i % 5 == 0 || i % 3 == 0) { 
      matrix.setPixelColor(i, c);
      matrix.show();
      //delay(wait);
    }
  }
}


void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < matrix.numPixels(); i++) {
    matrix.setPixelColor(i, c);
    matrix.show();
  }
  //delay(wait);
}
