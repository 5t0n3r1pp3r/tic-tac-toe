/*
 * Tic Tac Toe Board
 * by John L and Tim W
 * march 2018
 * one wire kepad 3x3 on A5
 * button resistances 
 *A5 
 *~1/5k 
 *negative 0 button 
 *~200ohm
 *button 1
 *~20ohm
 *button 2
 *~220ohm
 * button 3
 *~220ohm
 * button 4
 *~220ohm
 *button 5
 *~220ohm
 *button 6
 *~220ohm
 *button 7
 *~220ohm
 *positive button 8
 *
 * 3x3 rgb led with 74HC02 xor gates for green  
 *Leds and gates ~30ohm gnd circuit
 *Dip, Switch,or jumper to pin 1 to ground for normal mode  
 */


/*
 * ------------------------------------Variables 
 */
int bval = 0;
int serialenable = 0;
bool tstdone = false;
int keypadPin = A5;
/*
 * ------------------------------------Led Pins 
 */
int redled[9] =
{ 2, 4, 6,
  8, 0, 10,
  12, 14, 16
};

int bluled[9] =
{ 3, 5, 7,
  9, 18, 11,
  13, 15, 17
};
/*
 * ------------------------------------Led on off State 
 */
int ledstate[9] =
{ 0, 0, 0,
  0, 0, 0,
  0, 0, 0
};

/*
 * ------------------------------------Led Color State and turn counter
 */
int ledclr[9] = //0= red 1= blue
{ 0, 0, 0,
  0, 0, 0,
  0, 0, 0
};

int turn = 0; //0= red 1= blue
/*
 * ------------------------------------Led Status Test Variables 
 */
int R_led_status[9] =
{ 0, 0, 0,
  0, 0, 0,
  0, 0, 0
};

int B_led_status[9] =
{ 0, 0, 0,
  0, 0, 0,
  0, 0, 0
};

/*
 * ------------------------------------Led Status Test Winning Moves
 */
int winstate[8][9] =
{
  { 1, 1, 1,
    0, 0, 0,
    0, 0, 0
  },

  { 0, 0, 0,
    1, 1, 1,
    0, 0, 0
  },

  { 0, 0, 0,
    0, 0, 0,
    1, 1, 1
  },

  { 0, 1, 0,
    0, 1, 0,
    0, 1, 0
  },

  { 1, 0, 0,
    1, 0, 0,
    1, 0, 0
  },

  { 0, 0, 1,
    0, 0, 1,
    0, 0, 1
  },

  { 1, 0, 0,
    0, 1, 0,
    0, 0, 1
  },

  { 0, 0, 1,
    0, 1, 0,
    1, 0, 0
  }
};
/*
  111000000
  000111000
  000000111
  010010010
  100100100
  001001001
  100010001
  001010100
*/
/*Button Resistances
  0
  133
  146
  292
  438
  585
  731
  877
  1023
*/

/*
 * ------------------------------------Led Win sequence 
 */
void redwins() {
  turn = 0;
  for (int pin = 0; pin <= 8; pin ++) {
    ledstate[pin] = 1;
    ledclr[pin] = 0;
    ledDisp();
    delay(150);
  } delay(500);
  for (int pin = 0; pin <= 8; pin ++) {
    ledstate[pin] = 0;
    ledclr[pin] = 0;
    ledDisp();
    delay(150);
  } delay(500);
  for (int pin = 0; pin <= 8; pin ++) {
    ledstate[pin] = 1;
    ledclr[pin] = 0;
    ledDisp();
    delay(150);
  } delay(2000);
  for (int pin = 0; pin <= 8; pin ++) {
    ledstate[pin] = 0;
    ledclr[pin] = 0;
    ledDisp();
    delay(150);
  } delay(500);
}
void bluwins() {
  turn = 1;
 for (int pin = 0; pin <= 8; pin ++) {
    ledstate[pin] = 1;
    ledclr[pin] = 1;
    ledDisp();
    delay(150);
  } delay(500);
  for (int pin = 0; pin <= 8; pin ++) {
    ledstate[pin] = 0;
    ledclr[pin] = 1;
    ledDisp();
    delay(150);
  } delay(500);
  for (int pin = 0; pin <= 8; pin ++) {
    ledstate[pin] = 1;
    ledclr[pin] = 1;
    ledDisp();
    delay(150);
  } delay(1000);
  for (int pin = 0; pin <= 8; pin ++) {
    ledstate[pin] = 0;
    ledclr[pin] = 1;
    ledDisp();
    delay(150);
  } delay(500);
}

/*
 * ------------------------------------Winning Status Test 
 */
void whoWon(int array1[9], int array2[9], int array3[8][9]) {
  for (int slot = 0; slot <= 7; slot ++) {
    int red = 0;
    int blue = 0 ;
    for (int i = 0; i <= 8; i = i + 1) {
      if (array1[i] & array3[slot][i]) {
        red ++;
        Serial.print ("red element= ");
        Serial.println (red);
        if (red >= 3) {
          redwins();
        }
        else {
          Serial.print ("red element= ");

          Serial.println (red);
        }
      }

    } for (int n = 0; n <= 8; n = n + 1) {
      if (array2[n] & array3[slot][n]) {
        Serial.print ("blu element= ");
        blue ++;
        Serial.println (blue);
        if (blue >= 3) {
          bluwins();
        }
      }
      else {
        Serial.print ("blu element= ");
        Serial.println (blue);
      }
    }
  }
}

/*
 * ------------------------------------Led Status Test  
 */
void CHECK_PIN_STATE() {
  for (int pin = 0; pin <= 8; pin ++) {
    R_led_status[pin] = digitalRead(redled[pin]);
    //  Serial.print("Red LED # ");Serial.print(pin);Serial.print(" = ");Serial.println(R_led_status[pin]);
  }
  for (int pin = 0; pin <= 8; pin ++) {
    B_led_status[pin] = digitalRead(bluled[pin]);
    //Serial.print("Blue LED # ");Serial.print(pin);Serial.print(" = ");Serial.println(B_led_status[pin]);
  }
}

/*
 * ------------------------------------Led Status Display Sequence
 */
void ledDisp() {
  if (ledstate[0] == 1) {
    if (ledclr[0] == 0) {
      digitalWrite(redled[0], HIGH);
    } if (ledclr[0] == 1) {
      digitalWrite(bluled[0], HIGH);
    }
  }else{
      digitalWrite(redled[0], LOW);
      digitalWrite(bluled[0], LOW);
    }
  if (ledstate[1] == 1) {
    if (ledclr[1] == 0) {
      digitalWrite(redled[1], HIGH);
    } if (ledclr[1] == 1) {
      digitalWrite(bluled[1], HIGH);
    }
  }else{
      digitalWrite(redled[1], LOW);
      digitalWrite(bluled[1], LOW);
    }
  if (ledstate[2] == 1) {
    if (ledclr[2] == 0) {
      digitalWrite(redled[2], HIGH);
    } if (ledclr[2] == 1) {
      digitalWrite(bluled[2], HIGH);
    }
  }else{
      digitalWrite(redled[2], LOW);
      digitalWrite(bluled[2], LOW);
    }
  if (ledstate[3] == 1) {
    if (ledclr[3] == 0) {
      digitalWrite(redled[3], HIGH);
    } if (ledclr[3]  == 1) {
      digitalWrite(bluled[3], HIGH);
    }
  }else{
      digitalWrite(redled[3], LOW);
      digitalWrite(bluled[3], LOW);
    }
  if (ledstate[4] == 1) {
    if (ledclr[4] == 0) {
      digitalWrite(redled[4], HIGH);
    } if (ledclr[4] == 1) {
      digitalWrite(bluled[4], HIGH);
    }
  }else{
      digitalWrite(redled[4], LOW);
      digitalWrite(bluled[4], LOW);
    }
  if (ledstate[5] == 1) {
    if (ledclr[5] == 0) {
      digitalWrite(redled[5], HIGH);
    } if (ledclr[5] == 1) {
      digitalWrite(bluled[5], HIGH);
    }
  }else{
      digitalWrite(redled[5], LOW);
      digitalWrite(bluled[5], LOW);
    }
  if (ledstate[6] == 1) {
    if (ledclr[6] == 0) {
      digitalWrite(redled[6], HIGH);
    } if (ledclr[6] == 1) {
      digitalWrite(bluled[6], HIGH);
    }
  }else{
      digitalWrite(redled[6], LOW);
      digitalWrite(bluled[6], LOW);
    }
  if (ledstate[7] == 1) {
    if (ledclr[7] == 0) {
      digitalWrite(redled[7], HIGH);
    } if (ledclr[7] == 1) {
      digitalWrite(bluled[7], HIGH);
    }
  } else{
      digitalWrite(redled[7], LOW);
      digitalWrite(bluled[7], LOW);
    }
  if (ledstate[8] == 1) {
    if (ledclr[8] == 0) {
      digitalWrite(redled[8], HIGH);
    } if (ledclr[8] == 1) {
      digitalWrite(bluled[8], HIGH);
    }
  }else{
      digitalWrite(redled[8], LOW);
      digitalWrite(bluled[8], LOW);
    }
}
/*
 * ------------------------------------Turn Status Test 
 */
void next() {

  if (turn == 0) {
    turn = 1;
  } else if (turn == 1) {
    turn = 0;
  }
}
/*
 * -----------------------------------Button Status Test 
 */
void CHECK_BUTTON() {
  bval = analogRead(keypadPin);
  delay(10);
  if (bval == 0 && ledstate[0] == 0) { //0
    Serial.println("B1");
    ledstate[0] = 1;
    ledclr[0] = turn;
    next();
  } if (bval >= 20 && bval <= 139 && ledstate[3] == 0) { //133
    Serial.println("B2");
    ledstate[3] = 1;
    ledclr[3] = turn;
    next();
  } if (bval >= 140 && bval <= 219 && ledstate[6] == 0) { //146
    Serial.println("B3");
    ledstate[6] = 1;
    ledclr[6] = turn;
    next();
  } if (bval >= 220 && bval <= 365 && ledstate[1] == 0) { //292
    Serial.println("B4");
    ledstate[1] = 1;
    ledclr[1] = turn;
    next();
  } if (bval >= 366 && bval <= 512 && bval != 469 && ledstate[4] == 0) { // 438
    Serial.println("B5");
    ledstate[4] = 1;
    ledclr[4] = turn;
    next();
  } if (bval >= 513 && bval <= 658 && ledstate[7] == 0) { //585
    Serial.println("B6");
    ledstate[7] = 1;
    ledclr[7] = turn;
    next();
  } if (bval >= 659 && bval <= 804 && ledstate[2] == 0) { //731
    Serial.println("B7");
    ledstate[2] = 1;
    ledclr[2] = turn;
    next();
  } if (bval >= 805 && bval <= 951 && ledstate[5] == 0) { //877
    Serial.println("B8");
    ledstate[5] = 1;
    ledclr[5] = turn;
    next();
  } if (bval >= 952 && bval <= 1024 && ledstate[8] == 0) { //1024
    Serial.println("B9");
    ledstate[8] = 1;
    ledclr[8] = turn;
    next();
  }

  //  Serial.println("val = ");
  //  Serial.println(bval);
}
/*
 * ------------------------------------Led Test 
 * 
 */
void Debug() {
  if (serialenable == HIGH && tstdone == false) {
    Serial.println("Light Test Start");

    for (int pin = 0; pin <= 18; pin ++) {
      digitalWrite(pin, HIGH);
      delay(450);
      digitalWrite(pin, LOW);

    } tstdone = true;
    Serial.println("Light Test Completed");
    delay(450);
  }
}
/*
 * ------------------------------------Setup/Startup 
 */
void setup() {
  pinMode(1, INPUT_PULLUP);
  serialenable = digitalRead(1);

  if (serialenable == HIGH) {
    pinMode(1, OUTPUT);
    Serial.begin(9600);
    Serial.println("Serial Enabled");
    delay(700); // Wait for 1000 millisecond(s)
  }
  if (serialenable == LOW) {
    pinMode(0, OUTPUT);
  }
  for (int pin = 2; pin <= 18; pin ++) {
    Serial.print("Pin ");
    Serial.print(pin);
    Serial.println(" Activated!");
    pinMode(pin, OUTPUT);
  } Serial.println("BOOT Finished Run Program.....");
  delay(1000); // Wait for 1000 millisecond(s)
}
/*
 * ------------------------------------Main Program
 */
void loop()
{ Debug();
  CHECK_PIN_STATE();
  CHECK_BUTTON();
  ledDisp();
  delay(5);
  //bluwins();
  whoWon(R_led_status, B_led_status, winstate);
  //redwins();
  // Serial.println("_____________cycle______________");
}
