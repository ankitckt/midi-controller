// Pin Definitions
// Keyboard Rows are connected to
const int row1 = 5;
const int row2 = 6;
const int row3 = 7;
const int row4 = 8;

const unsigned char NOTE_ON_BASE = 0x90;//command format 0x90+channelNo(0-15),pitch,velocity(0-127)
const unsigned char PROGRAM_CHANGE_BASE = 0xC0;//command format 0xC0+channelNo(0-15),program(0-127)
const int PITCH_BEND_BASE=0xE0;//command format 0xE0+channelNo(0-15),LSB,MSB

// The 74HC595 uses a serial communication 
// link which has three pins
const int clock = 9;
const int latch = 10;
const int data = 11;


uint8_t keyToMidiMap[32];

boolean keyPressed[32];

boolean rPressed = false;
boolean rReleased = true;
boolean lPressed = false;
boolean lReleased = true;
boolean uPressed = false;
boolean uReleased = true;
boolean dPressed = false;
boolean dReleased = true;
boolean cPressed = false;
boolean cReleased = true;



boolean rClicked = false;
boolean lClicked = false;
boolean uClicked = false;
boolean dClicked = false;
boolean cClicked = false;

unsigned char midiChannel = 0;
unsigned char midiProgram = 0;

int noteVelocity = 127;


// use prepared bit vectors instead of shifting bit left everytime
int bits[] = { B11111110, B11111101, B11111011, B11110111, B11101111, B11011111, B10111111, B01111111 };

char buffer[64] = "";

//PS2 joystick pins
const int xPosPin = A2;
const int yPosPin = A3;

int xOffset = 511;
int yOffset = 511;

int xPos = 0;
int yPos = 0;
int coarseX = 0;
int coarseY = 0;

const int buttonPin = A1;//A1
int button = 512;
// 74HC595 shift to next column
void scanColumn(int value) {
  digitalWrite(latch, LOW); //Pulls the chips latch low
  shiftOut(data, clock, MSBFIRST, value); //Shifts out the 8 bits to the shift register
  digitalWrite(latch, HIGH); //Pulls the latch high displaying the data
}

void setup() {
  
  // Map scan matrix buttons/keys to actual Midi note number. Lowest num 41 corresponds to F MIDI note.
  keyToMidiMap[0] = 41;
  keyToMidiMap[1] = 42;
  keyToMidiMap[2] = 43;
  keyToMidiMap[3] = 44;
  keyToMidiMap[4] = 45;
  keyToMidiMap[5] = 46;
  keyToMidiMap[6] = 47;
  keyToMidiMap[7] = 48;

  keyToMidiMap[8] =     49;
  keyToMidiMap[1 + 8] = 50;
  keyToMidiMap[2 + 8] = 51;
  keyToMidiMap[3 + 8] = 52;
  keyToMidiMap[4 + 8] = 53;
  keyToMidiMap[5 + 8] = 54;
  keyToMidiMap[6 + 8] = 55;
  keyToMidiMap[7 + 8] = 56;

  keyToMidiMap[16] =     57;
  keyToMidiMap[1 + 16] = 58;
  keyToMidiMap[2 + 16] = 59;
  keyToMidiMap[3 + 16] = 60;
  keyToMidiMap[4 + 16] = 61;
  keyToMidiMap[5 + 16] = 62;
  keyToMidiMap[6 + 16] = 63;
  keyToMidiMap[7 + 16] = 64;

  keyToMidiMap[24] =     65;
  keyToMidiMap[1 + 24] = 66;
  keyToMidiMap[2 + 24] = 67;
  keyToMidiMap[3 + 24] = 68;
  keyToMidiMap[4 + 24] = 69;
  keyToMidiMap[5 + 24] = 70;
  keyToMidiMap[6 + 24] = 71;
  keyToMidiMap[7 + 24] = 72;

  // setup pins output/input mode
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);

  pinMode(row1, INPUT);
  pinMode(row2, INPUT);
  pinMode(row3, INPUT);
  pinMode(row4, INPUT);

//  pinMode(buttonPin,OUTPUT);
//  digitalWrite(buttonPin,HIGH);
//  pinMode(buttonPin,INPUT);
//    Serial.begin(31250);/
  Serial.begin(115200);
  
  delay(100);

}

void loop() {


  rClicked = false;
  lClicked = false;
  uClicked = false;
  dClicked = false;
  cClicked = false;
//  char buffer[32];
  xPos = analogRead(xPosPin) - xOffset;
  yPos = analogRead(yPosPin) - yOffset;
  button = analogRead(buttonPin);
  coarseX = map(xPos,-512,511,0,7);
  coarseY = map(yPos,-512,511,0,7);

  if(coarseX<2)
  {
   lPressed = true; 
  }
  else if(coarseX>=6)
  {
    rPressed = true;
  }
  else if(lPressed)
  {
    lPressed = false;
    lClicked = true;
  }
  else if(rPressed)
  {
    rPressed = false;
    rClicked = true;
  }

  if(coarseY >= 6)
  {
    uPressed = true;
  }
  else if(coarseY <2)
  {
    dPressed = true;
  }
  else if(uPressed)
  {
    uPressed = false;
    uClicked = true;
  }
  else if(dPressed)
  {
    dPressed = false;
    dClicked = true;
  }
//FIXME - Center button click not registered properly, debug HW
//  if(button < 2)
//  {
//    cPressed = true;
//  }
//  else if(true == cPressed);
//  {
//    cPressed = false;
//    cClicked = true;
//  }

//sprintf(buffer,"x %d r=%d rc=%d l=%d lc=%d",xPos,rPressed,rClicked,lPressed,lClicked);
//Serial.println(buffer);
  if(true == rClicked)
  {
//    sprintf(buffer,"rClicked x %d r=%d rc=%d l=%d lc=%d",xPos,rPressed,rClicked,lPressed,lClicked);
//Serial.println(buffer);
    midiProgram ++;
    if(midiProgram>127)
    {
      midiProgram = 0;
      
    }
    midiProg(PROGRAM_CHANGE_BASE+midiChannel,midiProgram);
  }
  else if(true == lClicked)
  {
//    sprintf(buffer,"lClicked x %d r=%d rc=%d l=%d lc=%d",xPos,rPressed,rClicked,lPressed,lClicked);
//Serial.println(buffer);
    
    if(midiProgram==0)
    {
      midiProgram = 127;
    }
    else
    {
      midiProgram--;
    }
    midiProg(PROGRAM_CHANGE_BASE+midiChannel,midiProgram);
  }

  if(true == cClicked)
  {
//    sprintf(buffer,"cClicked x %d r=%d rc=%d l=%d lc=%d, button %d",xPos,rPressed,rClicked,lPressed,lClicked,button);
//Serial.println(buffer);
    midiChannel++;
    if(midiChannel>15)
    midiChannel = 0;
  }

  if(yPos > 3|| yPos<-2)
  {
    int val = 0x2000 + (map(yPos,-512,511,-8192,8191));
    unsigned char LSB = val&0x7F;
    unsigned char MSB = (val >> 7) & 0x7F;
    pitchBend(PITCH_BEND_BASE+midiChannel,LSB,MSB);
  }
  
//  sprintf(buffer,"x=%4d,%d, y=%4d,%d sw = %d\n",xPos,coarseX,yPos, coarseY,button);
//  Serial.println(buffer);

//Serial.println("scanning...");/
  for (int col = 0; col < 8; col++) {
    
    // shift scan matrix to following column
    scanColumn(bits[col]);

    // check if any keys were pressed - rows will have HIGH output in this case corresponding
    int groupValue1 = digitalRead(row1);
    int groupValue2 = digitalRead(row2);
    int groupValue3 = digitalRead(row3);
    int groupValue4 = digitalRead(row4);

    // process if any combination of keys pressed
    if (groupValue1 == 0 || groupValue2 == 0 || groupValue3 == 0
        || groupValue4 == 0) {

      if (groupValue1 == 0 && !keyPressed[col]) {
        keyPressed[col] = true;
        noteOn(NOTE_ON_BASE+midiChannel, keyToMidiMap[col], noteVelocity);
      }

      if (groupValue2 == 0 && !keyPressed[col + 8]) {
        keyPressed[col + 8] = true;
        noteOn(NOTE_ON_BASE+midiChannel, keyToMidiMap[col + 8], noteVelocity);
      }

      if (groupValue3 == 0 && !keyPressed[col + 16]) {
        keyPressed[col + 16] = true;
        noteOn(NOTE_ON_BASE+midiChannel, keyToMidiMap[col + 16], noteVelocity);
      }

      if (groupValue4 == 0 && !keyPressed[col + 24]) {
        keyPressed[col + 24] = true;
        noteOn(NOTE_ON_BASE+midiChannel, keyToMidiMap[col + 24], noteVelocity);
      }

    }

    //  process if any combination of keys released
    if (groupValue1 != 0 && keyPressed[col]) {
      keyPressed[col] = false;
      noteOn(NOTE_ON_BASE+midiChannel, keyToMidiMap[col], 0);
    }

    if (groupValue2 != 0 && keyPressed[col + 8]) {
      keyPressed[col + 8] = false;
      noteOn(NOTE_ON_BASE+midiChannel, keyToMidiMap[col + 8], 0);
    }

    if (groupValue3 != 0 && keyPressed[col + 16]) {
      keyPressed[col + 16] = false;
      noteOn(NOTE_ON_BASE+midiChannel, keyToMidiMap[col + 16], 0);
    }

    if (groupValue4 != 0 && keyPressed[col + 24]) {
      keyPressed[col + 24] = false;
      noteOn(NOTE_ON_BASE+midiChannel, keyToMidiMap[col + 24], 0);
    }

  }

//
//  for(int counter = 0;counter<32;counter++)
//  {
//    if(keyPressed[counter])
//    {
//      Serial.println(counter);  
//    }
//  }
 // Serial.println("----------------------"); 
//  delay(1000);

}

void midiProg(unsigned char status, unsigned char data ) {
//  sprintf(buffer,"midiprog %x %x",status,data);
//  Serial.println(buffer);
  Serial.write(status);
  Serial.write(data);
}

void noteOn(int cmd, int pitch, int velocity) {
//  sprintf(buffer,"note %x %d %d",cmd,pitch,velocity);
//  Serial.println(buffer);
    Serial.write(cmd);
   Serial.write(pitch);
  Serial.write(velocity);
}

void pitchBend(int cmd, int LSB, int MSB) {
//  sprintf(buffer,"pitchbend %x %d %d",cmd,LSB,MSB);
//  Serial.println(buffer);
    Serial.write(cmd);
   Serial.write(LSB);
  Serial.write(MSB);
}
