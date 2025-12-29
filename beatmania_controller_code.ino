#include <Keyboard.h>

// Button Config
const int nBut = 9;
const int nLED = 9;
int buttonPins[nBut] = {3, 4, 6, 8, 10, 12, A5, A3, A1};
int buttonState[nBut] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int buttonPState[nBut] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int ledPins[nLED] = {2, 5, 7, 9, 11, 13, A4, A2, A0}; 
uint8_t butKeys[nBut] = {'f', 'g', 't', 'y', 'u', 'h', 'j', KEY_RETURN, KEY_LEFT_SHIFT};  // Modify values to change which key press each button maps to

unsigned long lastDebounceTime[nBut] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned long debounceTimer[nBut] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int debounceDelay = 10;



// Rotary Encoder Config
#define ENC_A 0
#define ENC_B 1
unsigned long _lastIncReadTime = micros(); 
unsigned long _lastDecReadTime = micros(); 
int _pauseLength = 25000;
int _fastIncrement = 1;
static boolean ttRight = false;
static boolean ttLeft = false;
volatile int counter = 0;
int mCount = 0;
int numInt = 0;
int pNumInt = 0;
uint8_t ttRightKey = '1';
uint8_t ttLeftKey = '`';
boolean stop = false;



void setup() {
  Serial.begin(115200);
  Keyboard.begin();
  
  // For Buttons
  for (int i = 0; i < nBut; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  
  // For LEDs
  for (int i = 0; i < nBut; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Set encoder pins and attach interrupts
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), read_encoder, CHANGE);

}

void loop() {
  // Serial.println(digitalRead(12));
  // delay(50);
  //Tester
  // if (!stop) {
  //   // Serial.println(mCount);
  //   //Serial.println(numInt);

  // }

  // For Buttons
  for (int i = 0; i < nBut; i++) {
    buttonState[i] = digitalRead(buttonPins[i]);
    debounceTimer[i] = millis() - lastDebounceTime[i];
    if (buttonState[i] != buttonPState[i]) {
      lastDebounceTime[i] = millis();

      if (debounceTimer[i] > debounceDelay) {
        if (buttonState[i] == LOW) {
          Keyboard.press(butKeys[i]);
          digitalWrite(ledPins[i], HIGH);
        } else {
          Keyboard.release(butKeys[i]);
          digitalWrite(ledPins[i], LOW);
        }
      }
 
    }

    buttonPState[i] = buttonState[i];
  }


  //For Rotary Encoder
  if (pNumInt != numInt) {
    mCount = 1;
  }

  if ((pNumInt == numInt) && (mCount > 0)) {
    mCount++;
  }

  if ((mCount > 70)) {
    // If greater than 70 cycles of no interupt, then release the keys
      Keyboard.release(ttRightKey);
      Keyboard.release(ttLeftKey);
      ttRight = false;
      ttLeft = false;
      mCount = 0;
  }
  pNumInt = numInt;
}

void read_encoder() {
  // Encoder interrupt routine for both pins. Updates counter
  // if they are valid and have rotated a full indent
  static int lastCounter = 0;
  static uint8_t old_AB = 3;  // Lookup table index
  static int8_t encval = 0;   // Encoder value  
  static const int8_t enc_states[]  = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Lookup table

  old_AB <<=2;  // Remember previous state

  if (digitalRead(ENC_A)) old_AB |= 0x02; // Add current state of pin A
  if (digitalRead(ENC_B)) old_AB |= 0x01; // Add current state of pin B
  
  encval += enc_states[( old_AB & 0x0f )];

  // Update counter if encoder has rotated a full indent, that is at least 100 steps
  if( encval > 100 ) {         //Case for clockwise rotation, change this value to adjust turntable sensitivity
    int changevalue = 1;
    if((micros() - _lastIncReadTime) < _pauseLength) {
      changevalue = _fastIncrement * changevalue; 
    }
    _lastIncReadTime = micros();
    counter = counter + changevalue;              // Update counter
    encval = 0;
    
    if (ttLeft) {
      ttLeft = false;
    }

    if ((lastCounter < counter) && !ttRight) {
      // Press "1" if encoder is turning clockwise
      Keyboard.press(ttRightKey);
      Keyboard.release(ttLeftKey);
      ttRight = true;
    } else if ((lastCounter < counter) && ttLeft && !ttRight) {
      // Case for immediate swapping from counter-clockwise to clockwise
      ttLeft = false;
      ttRight = true;
      Keyboard.press(ttRightKey);
      Keyboard.release(ttLeftKey);
    }
    lastCounter = counter;

  }
  else if( encval < -100 ) {        // Case for counter-clockwise rotation, change this value to adjust turntable sensitivity
    int changevalue = -1;
    if((micros() - _lastDecReadTime) < _pauseLength) {
      changevalue = _fastIncrement * changevalue; 
    }
    _lastDecReadTime = micros();
    counter = counter + changevalue;              // Update counter
    encval = 0;
    
    if (ttRight) {
      ttRight = false;
    }

  

    if ((lastCounter > counter) && !ttLeft) {
      // Press "`" if encoder is turning counter-clockwise
      Keyboard.press(ttLeftKey);
      Keyboard.release(ttRightKey);
      ttLeft = true;
    } else if ((lastCounter > counter) && ttRight && !ttLeft) {
      // Case for immediate swapping from clockwise to counter-clockwise
      ttLeft = true;
      ttRight = false;
      Keyboard.press(ttLeftKey);
      Keyboard.release(ttRightKey);
    }
    lastCounter = counter;

  } 

  numInt++;
} 
