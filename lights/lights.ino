#include <IRremote.h>

// set IR receiver pin
const int RECEIVER_PIN = 15;
IRrecv irrecv(RECEIVER_PIN);
decode_results results;

// pin numbers
const int pins[8] = {4, 5, 6, 7, 8, 9, 10, 11};

// blink
unsigned long previous_blink_ms = 0;
const unsigned long blink_interval = 500;
bool blink = false;

// fade out
unsigned long previous_fade_out_ms = 0;
const unsigned long fade_out_interval = 20;
int brightness = 255;
const int fade_amount = 5;
bool fade_out_now = false;


void fade_out(int pin) {
  const unsigned long current_ms = millis();
  if (fade_out_now and current_ms - previous_fade_out_ms >= fade_out_interval) {
    previous_fade_out_ms = current_ms;
    // set the brightness of the pin
    analogWrite(pin, brightness);
  
    // reduce the brightness
    brightness -= fade_amount;
  
    if (brightness <= 0) {
      analogWrite(pin, 0);
      fade_out_now = false;
    }
  }
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
  if (irrecv.decode(&results)) {
    switch (results.value) {
      // on
      case 0xFD00FF: {
        // TODO:
        digitalWrite(pins[2], !digitalRead(pins[2]));
        break;
      }
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
          fade_out_now = true;
        }
        break;
      }
    }

    // resume recieving commands
    irrecv.resume();
  }

  if (blink) {
    unsigned long current_ms = millis();
    if (current_ms - previous_blink_ms >= blink_interval) {
      previous_blink_ms = current_ms;
      digitalWrite(pins[3], !digitalRead(pins[3]));
    }
  }

  if (fade_out_now) {
    fade_out(pins[5]);
  }
}
