#include <Wire.h>
#include <Adafruit_NeoPixel.h>

//strip
#define PIN   2
#define MAX_LED 40

#define ADD true
#define SUB false

int val = 0;
boolean stat = ADD;
Adafruit_NeoPixel strip = Adafruit_NeoPixel( MAX_LED, PIN, NEO_RGB + NEO_KHZ800 );
int MinBrightness = 2;        //value 0-255
int MaxBrightness = 220;      //value 0-255
int fadeInWait = 5;           //lighting up speed, steps.
int fadeOutWait = 5;          //dimming speed, steps.

int VCC = 13;//as power vcc
int D3 = 12; //A
int D2 = 11; //B
int D1 = 10; //C
int D0 = 9;  //D

unsigned char temp1;//mode
volatile int mode = HIGH;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  //strip.init
  strip.begin();
  strip.show();
  
  //433mhz_controller_init
  pinMode(VCC, OUTPUT);
  pinMode(D3, INPUT);
  pinMode(D2, INPUT);
  pinMode(D1, INPUT);
  pinMode(D0, INPUT);

  digitalWrite(VCC, HIGH);//AS VCC 
  delay(500);//Waiting for rf receiver module startup

  //button interrupt
  attachInterrupt(digitalRead(D3), button_scan, CHANGE);
  attachInterrupt(digitalRead(D2), button_scan, CHANGE);
  attachInterrupt(digitalRead(D1), button_scan, CHANGE);
  attachInterrupt(digitalRead(D0), button_scan, CHANGE);
}

//breathe
void rgbBreathe(uint32_t c, uint8_t y) {
  if (mode == 1 || 3) {
    for (uint8_t b = MinBrightness; b < MaxBrightness; b++) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeInWait);
    }
    //strip.setBrightness(MaxBrightness * 255 / 255);
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(y);
    }
    for (uint8_t b = MaxBrightness; b > MinBrightness; b--) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeOutWait);
    }
  }
}

//led_init
void changeColor(uint32_t c) {
  for (uint16_t i = 0; i < MAX_LED; i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(20);
}

//chasing
void rgbChasing(uint32_t c) {
  for (uint16_t i = 0 ; i < MAX_LED; i++) {
    if (mode == 2) {
      i++;
      strip.setPixelColor(i, c);
      strip.setBrightness(255);
      strip.show();
      delay(70);
      if (i == MAX_LED) {
        i = 0;
        changeColor(strip.Color(0, 0, 0));
      }
    }
  }
}

//button detection
void button_scan()
{
  //A
  temp1 = digitalRead(D3);
  if (temp1 == HIGH)
  {
    delay(10);
    temp1 = digitalRead(D3);
    if (temp1 == HIGH)
    {
      mode = 1;
    }
    //finger release_detection
    while (temp1 == HIGH) {
      temp1 = digitalRead(D3);
    }
  }
  //B
  temp1 = digitalRead(D2);
  if (temp1 == HIGH)
  {
    delay(10);
    temp1 = digitalRead(D2);
    if (temp1 == HIGH)
    {
      mode = 2;
    }
    //finger release_detection
    while (temp1 == HIGH) {
      temp1 = digitalRead(D2);
    }
  }
  //C
  temp1 = digitalRead(D1);
  if (temp1 == HIGH)
  {
    delay(10);
    temp1 = digitalRead(D1);
    if (temp1 == HIGH)
    {
      mode = 3;
    }
    //finger release_detection
    while (temp1 == HIGH) {
      temp1 = digitalRead(D1);
    }
  }
  //D
  temp1 = digitalRead(D0);
  if (temp1 == HIGH)
  {
    delay(10);
    temp1 = digitalRead(D0);
    if (temp1 == HIGH)
    {
      mode = 4;
    }
    //finger release_detection
    while (temp1 == HIGH) {
      temp1 = digitalRead(D0);
    }
  }
}
// the loop routine runs over and over again forever:
void loop() {
  button_scan();
  switch (mode) {
    case 1:
      rgbBreathe(strip.Color(255, 0, 0), 5);  //green breathe
      break;
    case 2:
      changeColor(strip.Color(0, 0, 0));
      rgbChasing(strip.Color(255, 0, 0));     //green chasing
      break;
    case 3:
      rgbBreathe(strip.Color(100, 255, 0), 5);//orange breathe
      break;
    case 4:
      changeColor(strip.Color(0, 0, 0));      //led off
      break;
  }
}
