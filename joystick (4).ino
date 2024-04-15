#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
int data[5];  


const int xPin = A1;
const int yPin = A0;
const int buttonPin = 4;
const int ptmrPin = A2;
const int tumblrPin = 0;


int xPosition = 0;
int yPosition = 0;
int buttonState = 0;
int tumblrState = 0;
int ptmrState = 0;


int speed = 0;
int direction = 0;
                                
void setup(){
    radio.begin();
    radio.setChannel(5); // Channel (from 0 to 127), 5 - freq 2,405 GHz (only 1 receiver on a single channel and up to 6 transmitters)
    radio.setDataRate     (RF24_1MBPS); // Speed (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS)
    radio.setPALevel      (RF24_PA_HIGH); // Transmitter power (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    radio.openWritingPipe (0x1234567890LL); 
    
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(tumblrPin, INPUT);
    pinMode(ptmrPin, INPUT);
}
void loop(){
    xPosition = analogRead(xPin);
    yPosition = analogRead(yPin);
    buttonState = digitalRead(buttonPin);
    tumblrState = digitalRead(tumblrPin);
    ptmrState = analogRead(ptmrPin);
    


    if (xPosition > 520) { 
 // идти вперед 
 direction = 3; // 3 - вперед, 2 - назад, 1 - вправо, 0 - влево 
 speed = map(xPosition, 520, 1023, 5, 255);
 }
 if else (xPosition < 510) {
 // двигаться назад 
 direction = 2; 
 speed = map(xPosition, 520, 0, 5, 255);
 }
 if else (yPosition > 520) {
 // идти направо 
 direction = 1; // 0 - вперед, 1 - назад, 2 - вправо, 3 - влево 
 speed = 255; //map (xPosition), 520, 1023, 5, 255);
 }
 if else (yPosition < 510) {
 // идти налево 
 direction = 0; // 0 - вперед, 1 - назад, 2 - вправо, 3 - влево 
 speed = map(xPosition, 520, 0, 5, 255);
 }
 else{ 
 direction = 4;
 speed = 0;
 }


 data[0] = direction;
 data[1] = speed;
 data[2] = !buttonState;
 data[3] = tumblrState;
 data[4] = ptmrState;
 radio.write(&data, sizeof(data)); // проверить доставку: if(radio.write(&data, sizeof(data)))


 Serial.println(ptmrState);
 //Serial.println(speed);
    sleep(100); 
}
