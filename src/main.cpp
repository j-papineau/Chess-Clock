#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <pitches.h>
#include <ESP32Encoder.h>
#include <OneButton.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define SDA_1 21
#define SCL_1 22

#define SDA_2 33
#define SCL_2 32

#define playerOneButton 2
#define playerTwoButton 23

#define OLED_RESET -1

#define DEF_ADDR = 0x3c

#define playerTwoLED 4
#define playerOneLED 0

#define ENC_CLK 5
#define ENC_DT 18
#define ENC_SW 19

const unsigned char blackBMP [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0x3c, 0x3c, 0xff, 0xfe, 0x1c, 0x38, 0x7f, 
	0xfe, 0x0c, 0x30, 0x7f, 0x86, 0x1e, 0x78, 0x61, 0x87, 0x1e, 0x78, 0xe1, 0x87, 0x9e, 0x79, 0xe1, 
	0x8f, 0x9e, 0x79, 0xf1, 0xe7, 0x8e, 0x71, 0xe7, 0xe3, 0x8c, 0x31, 0xc7, 0xe3, 0x8c, 0x31, 0xc7, 
	0xf1, 0x84, 0x21, 0x8f, 0xf0, 0x84, 0x21, 0x0f, 0xf0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x0f, 
	0xf0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x0f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0xfe, 0x7f, 0x1f, 
	0xff, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x7f, 
	0xff, 0x80, 0x01, 0xff, 0xfc, 0xfc, 0x3f, 0x3f, 0xfc, 0x07, 0xe0, 0x3f, 0xff, 0xc0, 0x03, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
};

const unsigned char whiteBMP [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xf9, 0xff, 0xff, 0x0f, 0xf0, 0xff, 0xff, 0x6f, 0xf6, 0xff, 
	0xff, 0x6f, 0xf6, 0xff, 0xff, 0x0f, 0xf0, 0xff, 0xcf, 0xcf, 0xf1, 0xf3, 0x87, 0x8f, 0xf1, 0xe1, 
	0x37, 0x87, 0xe5, 0xec, 0x37, 0xb7, 0xed, 0xec, 0x87, 0xb3, 0xcd, 0xe1, 0xe3, 0xb9, 0x9d, 0xc7, 
	0xe1, 0x3d, 0xbc, 0x87, 0xf4, 0x3c, 0x3c, 0x2f, 0xf6, 0x7e, 0x7e, 0x6f, 0xf7, 0x7e, 0x7e, 0xef, 
	0xf3, 0xff, 0xff, 0xcf, 0xf3, 0xff, 0xff, 0xdf, 0xfb, 0xff, 0xff, 0xdf, 0xfb, 0xff, 0xff, 0xdf, 
	0xf9, 0xff, 0xff, 0x9f, 0xf9, 0xff, 0xff, 0x9f, 0xfd, 0xff, 0xff, 0xbf, 0xfc, 0x00, 0x00, 0x3f, 
	0xfd, 0xff, 0xff, 0xbf, 0xfc, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff
};

int buzzer = 16;
int last, value;

int menuitem = 1;
int frame = 1;
int page = 1;
int lastMenuItem = 1;

int playerTime;
int playerIncrement;

ESP32Encoder encoder;
Adafruit_SSD1306 displayOne(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 displayTwo(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);


void printToScreenOne(String);
void printToScreenTwo(String);
void p1btnClicked();
void p2btnClicked();
void setupGame();
void runGame();
void drawP1Time(int);
void drawP2Time(int);

int p1last = HIGH;
int p1Current;

int p2last = HIGH;
int p2Current;

int encLast = HIGH;
int encCurrent;

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
  displayOne.setTextSize(1);
  displayOne.setTextColor(WHITE);
  displayTwo.clearDisplay();
  displayTwo.setTextSize(1);
  displayTwo.setTextColor(WHITE);
  displayTwo.setRotation(2);
  displayOne.setRotation(2);

  Serial.println("displays online");

  printToScreenOne("Display One");
  printToScreenTwo("Display Two");

  encoder.attachHalfQuad(ENC_DT, ENC_CLK);
  encoder.setCount(0);
  last = encoder.getCount();

  //btn setup
  pinMode(playerOneButton, INPUT_PULLUP);
  pinMode(playerTwoButton, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);

}

void loop() {

  setupGame();
  
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

void printToBoth(String out){
  displayOne.clearDisplay();
  displayOne.setCursor(0,0);
  displayOne.print(out);
  displayTwo.clearDisplay();
  displayTwo.setCursor(0,0);
  displayTwo.print(out);
  displayOne.display();
  displayTwo.display();
}

void p1btnClicked(){
  printToScreenOne("p1 clicked!");
}
void p2btnClicked(){
 printToScreenOne("p2 clicked!");
}

void setupGame(){
  bool setupFinished = false;
  encoder.setCount(0);
  long lastCount = encoder.getCount();
  long currentCount;

  int currentMins = 0;
  
  while(!setupFinished){
    //setup time interval for players (total play time)
    encCurrent = digitalRead(ENC_SW);
    currentCount = encoder.getCount();

    if(currentCount > lastCount){
      currentMins++;
      tone(buzzer, NOTE_F3, 10);
    }else if (currentCount < lastCount)
    {
      currentMins--;
      tone(buzzer, NOTE_A3, 10);
    }else if(encLast == LOW && encCurrent == HIGH){
      setupFinished = true;
    }
    
    if(currentMins <= 0){
      currentMins = 0;
      currentCount = 0;
    }
    encLast = encCurrent;
    lastCount = currentCount;
    printToBoth("Player Time: " + String(currentMins) + ":00");
    // tone(buzzer, NOTE_A3, 10);
  }

  playerTime = currentMins;
  setupFinished = false;
  int currentIncrement = 0;
  encoder.setCount(0);
  currentCount = 0;

  while(!setupFinished){
    //setup time increment per turn
    encCurrent = digitalRead(ENC_SW);
    currentCount = encoder.getCount();

    if(currentCount > lastCount){
      currentIncrement++;
      tone(buzzer, NOTE_F3, 10);
    }else if (currentCount < lastCount)
    {
      currentIncrement--;
      tone(buzzer, NOTE_A3, 10);
    }else if(encLast == LOW && encCurrent == HIGH){
      setupFinished = true;
    }
    
    if(currentIncrement <= 0){
      currentIncrement = 0;
      currentCount = 0;
    }
    encLast = encCurrent;
    lastCount = currentCount;

    printToBoth("Turn Increment: " + String(currentIncrement) + "s");
  }
  playerIncrement = currentIncrement;
  setupFinished = false;
  while (!setupFinished)
  {
    //confirm game
    encCurrent = digitalRead(ENC_SW);
    printToBoth("Start Game: \n" + String(playerTime) + ":00 | " + String(playerIncrement) + "s ?");

    if(encLast == LOW && encCurrent == HIGH){
      setupFinished = true;
    }

    encLast = encCurrent;
  }

  runGame();
  
}

void runGame(){
  int p1Time = playerTime * 60;
  int p2Time = playerTime * 60;
  bool gameOver = false;
  bool gameStart = false;
  bool p1Turn = false;
  bool p2Turn = false;
  unsigned long lastUpdateTime = 0;

  displayOne.setTextSize(3);
  displayTwo.setTextSize(3);

  //idle wait for turn start
  while(!gameStart){
    p1Current = digitalRead(playerOneButton);
    p2Current = digitalRead(playerTwoButton);

    drawP1Time(p1Time);
    drawP2Time(p2Time);

    if(p1last == LOW && p1Current == HIGH){
      tone(buzzer, NOTE_F3, 10);
      p2Turn = true;
      gameStart = true;
    }

    if(p2last == LOW && p2Current == HIGH){
      tone(buzzer, NOTE_F3, 10);
      p1Turn = true;
      gameStart = true;
    }

    p1last = p1Current;
    p2last = p2Current;
  }

  while(!gameOver){
    //actual clock logic
    //TODO: add logic for game end

    if(p1Turn){

      digitalWrite(playerOneLED, HIGH);

      while(1){
        p1Current = digitalRead(playerOneButton);
        if(millis() - lastUpdateTime >= 1000){
          p1Time--;
          drawP1Time(p1Time);
          lastUpdateTime = millis();
        }
        if(p1last == LOW && p1Current == HIGH){
          tone(buzzer, NOTE_F3, 10);
          p2Turn = true;
          p1Turn = false;
          digitalWrite(playerOneLED, LOW);
          p1last = HIGH;
          p1Time += playerIncrement;
          drawP1Time(p1Time);
          break;
        }
        p1last = p1Current;
      }     
    }

    if(p2Turn){
      
      digitalWrite(playerTwoLED, HIGH);

      while(1){
        p2Current = digitalRead(playerTwoButton);
        if(millis() - lastUpdateTime >= 1000){
          p2Time--;
          drawP2Time(p2Time);
          lastUpdateTime = millis();
        }
        if(p2last == LOW && p2Current == HIGH){
          tone(buzzer, NOTE_F3, 10);
          p2Turn = false;
          p1Turn = true;
          p2last = HIGH;
          p2Time += playerIncrement;
          digitalWrite(playerTwoLED, LOW);
          drawP2Time(p2Time);
          break;
        }
        p2last = p2Current;
      }
    }
  }
}

void drawP1Time(int time){
  displayOne.clearDisplay();
  displayOne.setCursor(0,0);

  int mins = time / 60;
  int sec = time - (mins * 60);

  displayOne.drawBitmap(0, 0, blackBMP, 32, 32, WHITE);
  displayOne.setCursor(34,0);
  
  if(sec >= 10){
      displayOne.print(String(mins) + ":" + String(sec));
    }else{
      displayOne.print(String(mins) + ":0" + String(sec));
    }

  displayOne.display();

  // printToScreenOne(String(mins) + ":" + String(sec));
}

void drawP2Time(int time){

  displayTwo.clearDisplay();
  displayTwo.setCursor(0,0);

  int mins = time / 60;
  int sec = time - (mins * 60);
  
  displayTwo.drawBitmap(0, 0, whiteBMP, 32, 32, WHITE);
  displayTwo.setCursor(34,0);

  if(sec >= 10){
    displayTwo.print(String(mins) + ":" + String(sec));
  }else{
    displayTwo.print(String(mins) + ":0" + String(sec));
  }
  
  displayTwo.display();

  // printToScreenOne(String(mins) + ":" + String(sec));
}