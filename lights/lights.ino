#include <IRremote.h>

// set receiver pin
const int RECEIVER_PIN = 15;
IRrecv irrecv(RECEIVER_PIN);
decode_results results;

int pins[8] = {4, 5, 6, 7, 8, 9, 10, 11};
bool d = false;

int brightness = 255;
int fadeAmount = 5;
bool turnOn = false;


void fadeOut(int pin) {
  // set the brightness of the pin
  analogWrite(pin, brightness);

  // reduce the brightness
  brightness -= fadeAmount;

  if (brightness <= 0) {
    // switch back to digital
    digitalWrite(pins[5], LOW);
    // finish turn off process
    turnOn = false;
  }

  // wait for 20 ms to see the dimming effect
  delay(20);
}

void setup() {
  // enable receiver decoding
  irrecv.enableIRIn();

  // configure the pins to behave as OUTPUTs
  for (int i = 0; i < 8; i++) {
    pinMode(pins[i], OUTPUT);
  }
}

void loop() {
  // if turn off process is on
  if (turnOn) {
    fadeOut(p9);
  }

  if (irrecv.decode(&results)) {
    switch (results.value) {
      // on
      case 0xFD00FF: {
        // toggle lights
        digitalWrite(pins[1], !digitalRead(pins[1]));
        digitalWrite(pins[2], !digitalRead(pins[2]));
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
      // forward
      case 0xFD609F: {
        digitalWrite(pins[5], !digitalRead(pins[5]));
        break;
      }
      // play
      case 0xFDA05F: {
        digitalWrite(pins[6], !digitalRead(pins[6]));
        break;
      }
      // volume up
      case 0xFD807F: {
        d = !d;
        break;
      }
      // equalizer
      case 0xFDB04F: {
        if (digitalRead(p9) == LOW) {
          digitalWrite(p9, HIGH);
        } else {
          turnOn = true;
          brightness = 255;
        }
        break;
      }
    }

    // resume recieving commands
    irrecv.resume();
  }

  if (d) {
    digitalWrite(p7, HIGH);
    delay(500);
    digitalWrite(p7, LOW);
    delay(500);
  }
}
