#include <BleKeyboard.h>   
BleKeyboard bleKeyboard("SplitKeyboard","Ashish",100);
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
const int ROWS = 4;     
const int COLS = 5;    

int rowPins[ROWS] = {32, 33, 12, 35};        
int colPins[COLS] = {27, 26, 25, 21, 22};    

const int EXTRA_PIN_A = 15;   
const int EXTRA_PIN_B = 22;

const char EXTRA_KEY = '\n';   

char keymap[ROWS][COLS] = {
  {'q','w','e','r','t'},
  {'a','s','d','f','g'},
  {'z','x','c','v','b'},
  {'1','2','3','4','5'}
};

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    for(;;);

  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Split keyboard on!!");
  display.display();
  for (int r = 0; r < ROWS; r++) {
    pinMode(rowPins[r], OUTPUT);
    digitalWrite(rowPins[r], HIGH); 
  }

  for (int c = 0; c < COLS; c++) {
    pinMode(colPins[c], INPUT_PULLUP);
  }

  pinMode(EXTRA_PIN_A, INPUT_PULLUP);

  bleKeyboard.begin(); // start 
}

void loop() {
  if (bleKeyboard.isConnected()) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("bluetooth connected");
    display.display();
    // Scan matrix
    for (int r = 0; r < ROWS; r++) {
      digitalWrite(rowPins[r], LOW);
      for (int c = 0; c < COLS; c++) {
        if (digitalRead(colPins[c]) == LOW) {
          bleKeyboard.press(keymap[r][c]);
        } else {
          bleKeyboard.release(keymap[r][c]);
        }
      }
      digitalWrite(rowPins[r], HIGH);
    }

    if (digitalRead(EXTRA_PIN_A) == LOW && digitalRead(EXTRA_PIN_B) == LOW) {
      bleKeyboard.press(EXTRA_KEY);
    } else {
      bleKeyboard.release(EXTRA_KEY);
    }
  } else{
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("waiting");
    display.display();
  }
  delay(10); 
}
