#include <IRremote.h>

// set IR receiver pin
const int RECEIVER_PIN = 15;
IRrecv irrecv(RECEIVER_PIN);
decode_results results;

const int p3 = 3,        // high beam
          p4 = 4,        // low beam
          p5 = 5,        // right turn signal
          p6 = 6,        // parking
          p7 = 7,        // left turn signal
          p8 = 8,        // fog
          p9 = 9,        // cabin
          p10 = 10,      // reverse
          p16 = 16;      // stop

bool onIsActive = false,
     volumeUpIsActive = false,
     downIsActive = false,
     upIsActive = false;

bool isLeftTurnSignalOn = false,
     isRightTurnSignalOn = false,
     areCabinLightsOn = false,
     areHazardsOn = false;

int brightness = 250;

void setup() {
  // enable IR receiver
  irrecv.enableIRIn();
  
  // set pin modes
  pinMode(p3, OUTPUT);
  pinMode(p4, OUTPUT);
  pinMode(p5, OUTPUT);
  pinMode(p6, OUTPUT);
  pinMode(p7, OUTPUT);
  pinMode(p8, OUTPUT);
  pinMode(p9, OUTPUT);
  pinMode(p10, OUTPUT);
  pinMode(p16, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  // reed switches
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
}

void loop() {
  if (irrecv.decode(&results)) {
    switch (results.value) {
      // on
      case 0xFD00FF: {
        if (onIsActive) {
          analogWrite(p6, 0); // parking
          //analogWrite(A0, 0); // tail
          //analogWrite(A1, 0); // tag
          //analogWrite(A2, 0); // dashboard
        } else {
          analogWrite(p6, 255); // parking
          //analogWrite(A0, 128); // tail
          //analogWrite(A1, 128); // tag
          //analogWrite(A2, 128); // dashboard
        }
        onIsActive = !onIsActive;
        break;
      }
      // volume up
      case 0xFD807F: {
        // low beam
        if (volumeUpIsActive) {
          analogWrite(p4, 0);
        } else {
          analogWrite(p4, 255);
        }
        volumeUpIsActive = !volumeUpIsActive;
        break;
      }
      // func / stop
      case 0xFD40BF: {
        // high beam
        digitalWrite(p3, !digitalRead(p3));
        break;
      }
      // rewind
      case 0xFD20DF: {
        // left turn signal (p5)
        if (areHazardsOn) {
          break;
        }
        isRightTurnSignalOn = false;
        isLeftTurnSignalOn = !isLeftTurnSignalOn;
        break;
      }
      // play
      case 0xFDA05F: {
        // hazard lights
        isLeftTurnSignalOn = false;
        isRightTurnSignalOn = false;
        areHazardsOn = !areHazardsOn;
        break;
      }
      // forward
      case 0xFD609F: {
        // right turn signal
        if (areHazardsOn) {
          break;
        }
        isLeftTurnSignalOn = false;
        isRightTurnSignalOn = !isRightTurnSignalOn;
        break;
      }
      // down
      case 0xFD10EF: {
        // reverse
        if (downIsActive) {
          analogWrite(p10, 0);
        } else {
          analogWrite(p10, 255);
        }
        downIsActive = !downIsActive;
        break;
      }
      // volume down
      case 0xFD906F: {
        // stop
        digitalWrite(p16, !digitalRead(p16));
        break;
      }
      // up
      case 0xFD50AF: {
        // fog
        if (upIsActive) {
          analogWrite(p8, 0);
        } else {
          analogWrite(p8, 255);
        }
        upIsActive = !upIsActive;
        break;
      }
    }

    // resume recieving commands
    irrecv.resume();
  }

  if (isLeftTurnSignalOn) {
    digitalWrite(p7, HIGH);
    delay(500);
    digitalWrite(p7, LOW);
    delay(500);
  }

  if (isRightTurnSignalOn) {
    digitalWrite(p5, HIGH);
    delay(500);
    digitalWrite(p5, LOW);
    delay(500);
  }

  if (areHazardsOn) {
    digitalWrite(p5, HIGH);
    digitalWrite(p7, HIGH);
    delay(500);
    digitalWrite(p5, LOW);
    digitalWrite(p7, LOW);
    delay(500);
  }
  
  // door lights
  digitalWrite(A1, !digitalRead(A2));
  digitalWrite(A0, !digitalRead(A3));

  // cabin lights
  if (digitalRead(A2) and digitalRead(A3)) {
    if (areCabinLightsOn) {
      delay(1000);
      brightness = 250;
    }
    areCabinLightsOn = false;
  } else {
    if (!areCabinLightsOn) {
      brightness = 0;
    }
    areCabinLightsOn = true;
  }

  if (areCabinLightsOn) {
    analogWrite(p9, 255);
  } else {
    if (brightness > 0) {
      brightness -= 10;
      analogWrite(p9, brightness);
      delay(30);
    }
  }
}
