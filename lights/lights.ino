#include <IRremote.h>

// set IR receiver pin
const int RECEIVER_PIN = 15;
IRrecv irrecv(RECEIVER_PIN);
decode_results results;

const int p2 = 2, p3 = 3, p4 = 4, 
          p5 = 5, p6 = 6, p7 = 7,
          p8 = 8, p9 = 9, p10 = 10;
const int ap0 = A0, ap1 = A1;
bool onIsActive = false;

// blink
unsigned long previousBlinkMillis = 0;
const unsigned long blinkInterval = 500;
bool blink = false;

// fade out
unsigned long previousFadeOutMillis = 0;
const unsigned long fadeOutInterval = 20;
int brightness = 255;
const int fadeAmount = 5;
bool fadeOutNow = false;

/*
void fadeOut(int pin) {
  const unsigned long currentMillis = millis();
  if (fadeOutNow and currentMillis - previousFadeOutMillis >= fadeOutInterval) {
    previousFadeOutMillis = currentMillis;
    analogWrite(pin, brightness);
    
    // reduce the brightness
    brightness -= fadeAmount;
    
    if (brightness <= 0) {
      analogWrite(pin, 0);
      fadeOutNow = false;
    }
  }
}
*/

void setup() {
  // enable IR receiver
  irrecv.enableIRIn();

  // as output pins
  pinMode(p2, OUTPUT);
  pinMode(p3, OUTPUT);
  pinMode(p4, OUTPUT);
  pinMode(p5, OUTPUT);
  pinMode(p6, OUTPUT);
  pinMode(p7, OUTPUT);
  pinMode(p8, OUTPUT);
  pinMode(p9, OUTPUT);
  pinMode(p10, OUTPUT);
  pinMode(ap0, OUTPUT);
  pinMode(ap1, OUTPUT);
}

void loop() {
  if (irrecv.decode(&results)) {
    switch (results.value) {
      // on
      case 0xFD00FF: {
        if (onIsActive) {
          analogWrite(p6, 0);
          analogWrite(ap0, 0);
          // analogWrite(ap1, 0);
          // analogWrite(ap2, 0);
        } else {
          analogWrite(p6, 255);
          analogWrite(ap0, 100);
          // analogWrite(ap1, 100);
          // analogWrite(ap2, 100);
        }
        onIsActive = !onIsActive;
        
        /*
        if (a1) {
          analogWrite(pins[1], 0);
        } else {
          analogWrite(pins[1], 10);
        }
        a1 = !a1;
        digitalWrite(pins[2], !digitalRead(pins[2]));
        */
        break;
      }
      /*
      // volume up
      case 0xFD807F: {
        blink = !blink;
        if (!blink) {
          // turn off blinking immediately
          digitalWrite(pins[3], LOW);
        }
        break;
      }
      // func / stop
      case 0xFD40BF: {
        digitalWrite(pins[0], !digitalRead(pins[0]));
        break;
      }
      // rewind
      case 0xFD20DF: {
        digitalWrite(pins[4], !digitalRead(pins[4]));
        break;
      }
      // play
      case 0xFDA05F: {
        digitalWrite(pins[6], !digitalRead(pins[6]));
        break;
      }
      // forward
      case 0xFD609F: {
        digitalWrite(pins[5], !digitalRead(pins[5]));
        break;
      }
      // equalizer
      case 0xFDB04F: {
        if (analogRead(pins[5]) == 0) {
          brightness = 255;
          analogWrite(pins[5], brightness);
        } else {
          fadeOutNow = true;
        }
        break;
      }
    }
    */

    // resume recieving commands
    irrecv.resume();
  }

  /*
  if (blink) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousBlinkMillis >= blinkInterval) {
      previousBlinkMillis = currentMillis;
      digitalWrite(pins[3], !digitalRead(pins[3]));
    }
  }

  if (fadeOutNow) {
    fadeOut(pins[5]);
  }
  */
}
