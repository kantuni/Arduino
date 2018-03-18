#include <IRremote.h>

// set IR receiver pin
const int RECEIVER_PIN = 15;
IRrecv irrecv(RECEIVER_PIN);
decode_results results;

const int pins[8] = {4, 5, 6, 7, 8, 9, 10, 11};

int brightness = 255;
int fadeAmount = 5;
bool turnOff = false;

unsigned long previousMillis = 0;
const unsigned long interval = 500;
bool blink = false;


void fadeOut(int pin) {
  // set the brightness of the pin
  analogWrite(pin, brightness);

  // reduce the brightness
  brightness -= fadeAmount;

  if (brightness <= 0) {
    // switch back to digital
    digitalWrite(pins[5], LOW);
    turnOff = false;
  }

  // wait for 20 ms to see the dimming effect
  delay(20);
}

void setup() {
  // enable IR receiver decoding
  irrecv.enableIRIn();

  // configure the pins to behave as OUTPUTs
  for (int i = 0; i < 8; i++) {
    pinMode(pins[i], OUTPUT);
  }
}

void loop() {
  if (turnOff) {
    fadeOut(pins[5]);
  }

  if (irrecv.decode(&results)) {
    switch (results.value) {
      // on
      case 0xFD00FF: {
        digitalWrite(pins[1], !digitalRead(pins[1]));
        digitalWrite(pins[2], !digitalRead(pins[2]));
        break;
      }
      // volume up
      case 0xFD807F: {
        blink = !blink;
        if (!blink) {
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
        if (digitalRead(pins[5]) == LOW) {
          digitalWrite(pins[5], HIGH);
        } else {
          turnOff = true;
          brightness = 255;
        }
        break;
      }
    }

    // resume recieving commands
    irrecv.resume();
  }

  if (blink) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      digitalWrite(pins[3], !digitalRead(pins[3]));
    }
  }
}
