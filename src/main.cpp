#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <pitches.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define SDA_1 21
#define SCL_1 22

#define SDA_2 33
#define SCL_2 32

#define OLED_RESET -1

#define DEF_ADDR = 0x3c

#define playerTwoLED 4
#define playerOneLED 0

int buzzer = 16;



Adafruit_SSD1306 displayOne(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 displayTwo(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);

void printToScreenOne(String);
void printToScreenTwo(String);

void setup() {
  Serial.begin(9600);
  Serial.println("ESP On");

  pinMode(playerOneLED, OUTPUT);
  pinMode(playerTwoLED, OUTPUT);

  Wire.begin();
  Wire1.begin(SDA_2, SCL_2);

  displayOne.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  displayTwo.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  
  displayOne.clearDisplay();
  displayOne.setTextSize(2);
  displayOne.setTextColor(WHITE);
  displayTwo.clearDisplay();
  displayTwo.setTextSize(2);
  displayTwo.setTextColor(WHITE);
  displayTwo.setRotation(2);
  displayOne.setRotation(2);

  Serial.println("displays online");

  printToScreenOne("Display One");
  printToScreenTwo("Display Two");

 
}

void loop() {
  printToScreenTwo("waiting...");
  printToScreenOne("My Turn");
  digitalWrite(playerOneLED, HIGH);
  digitalWrite(playerTwoLED, LOW);
  tone(buzzer, NOTE_A3, 10);
  delay(500);
  printToScreenOne("waiting...");
  printToScreenTwo("My Turn");
  digitalWrite(playerTwoLED, HIGH);
  digitalWrite(playerOneLED, LOW);
  tone(buzzer, NOTE_G3, 10);
  delay(500);
}


void printToScreenOne(String out){
  displayOne.clearDisplay();
  displayOne.setCursor(0,0);
  displayOne.print(out);
  displayOne.display();
}

void printToScreenTwo(String out){
  displayTwo.clearDisplay();
  displayTwo.setCursor(0,0);
  displayTwo.print(out);
  displayTwo.display();
}