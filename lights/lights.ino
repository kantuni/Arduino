#include <IRremote.h>

// set receiver pin
const int RECEIVER_PIN = 15;
IRrecv irrecv(RECEIVER_PIN);
decode_results results;


int p4 = 4, p5 = 5, p6 = 6, p7 = 7,
    p8 = 8, p9 = 9, p10 = 10, p11 = 11;
int aa1, aa2, aa3, aa4, aa5;
bool d = false;

int brightness = 255;
int fadeAmount = 5;
bool turnOff = false;


// fade out functionality (provide pin)
// start
void fadeOut(int pin) {
  // set the brightness of the pin
  analogWrite(pin, brightness);

  // reduce the brightness
  brightness -= fadeAmount;

  if (brightness <= 0) {
    // switch back to digital
    digitalWrite(p9, LOW);
    // finish turn off process
    turnOff = false;
  }

  // wait for 20 ms to see the dimming effect
  delay(20);
}
// end

void setup() {
  // enable receiver decoding
  irrecv.enableIRIn();

  aa1 = 0;
  aa2 = 0;
  aa3 = 0;
  aa4 = 0;
  aa5 = 0;

  pinMode(p4, OUTPUT);
  pinMode(p5, OUTPUT);
  pinMode(p6, OUTPUT);
  pinMode(p7, OUTPUT);
  pinMode(p8, OUTPUT);
  pinMode(p9, OUTPUT);
  pinMode(p10, OUTPUT);
  pinMode(p11, OUTPUT);
}

void loop() {
  // if turn off process is on
  if (turnOff) {
    fadeOut(p9);
  }

  if (irrecv.decode(&results)) {
    switch (results.value) {
      // on
      case 0xFD00FF: {
        // toggle lights
        digitalWrite(p5, !digitalRead(p5));
        digitalWrite(p6, !digitalRead(p6));
        break;
      }
      // func / stop
      case 0xFD40BF: {
        if (aa2 == 0) {
          digitalWrite(p4, HIGH);
          aa2 = 1;
        } else {
          digitalWrite(p4, LOW);
          aa2 = 0;
        }
        break;
      }
      // rewind
      case 0xFD20DF: {
        if (aa3 == 0) {
          digitalWrite(p8, HIGH);
          aa3 = 1;
        } else {
          digitalWrite(p8, LOW);
          aa3 = 0;
        }
        break;
      }
      // forward
      case 0xFD609F: {
        if (aa4 == 0) {
          digitalWrite(p9, HIGH);
          aa4 = 1;
        } else {
          digitalWrite(p9, LOW);
          aa4 = 0;
        }
        break;
      }
      // play
      case 0xFDA05F: {
        if (aa5 == 0) {
          digitalWrite(p10, HIGH);
          aa5 = 1;
        } else {
          digitalWrite(p10, LOW);
          aa5 = 0;
        }
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
          turnOff = true;
          brightness = 255;
        }
        break;
      }
    }

    irrecv.resume();
  }

  if (d == true) {
    digitalWrite(p7, HIGH);
    delay(500);
    digitalWrite(p7, LOW);
    delay(500);
  }
}
