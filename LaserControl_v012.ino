//  This software is written by Philippe Kirsch, 01/04/2017
//  The CLI is based on Arduino Commandline v2.1.0 by Bas Stottelaar (basilfx)
//  https://github.com/basilfx/Arduino-CommandLine
//  Version 0.12
//  The used laser unit has 100mW on red, 50mW on green an 150mW on blue.

//  Hardware pins of the attachted safety relay to control the laser's power
#define relayPin01 7    
#define relayPin02 8
//  Hardware pins for the  laser diode TTL inputs
#define RLPin 10
#define GLPin 9
#define BLPin 11

#include "CommandLine.h"

typedef struct
{
  unsigned char color;
  int u_i;
  int r;
  int u_f;
} sweepmatrix;

// Keep track of the count, for the count command.
float fPmax = 0.25;
bool ModuleStatus = LOW;

unsigned char rainbow[9][3] =  {  {0,   0,    0},       //x-off   - 0
                                  {255, 0,    255},     //violett - 1
                                  {0,   0,    255},     //blue    - 2
                                  {0,   255,  255},     //cyan    - 3
                                  {0,   255,  0},       //green   - 4
                                  {255, 255,  0},       //yellow  - 5
                                  {255, 127,  0},       //orange  - 6
                                  {255, 0,    0},       //red     - 7
                                  {255, 255,  255}};    //white   - 8

                                  
// CommandLine instance.
CommandLine commandLine(Serial, "> ");

// Commands are simple structures that can be.
Command help        = Command("help", &handleHelp);
Command power       = Command("power", &handlePower);
Command color       = Command("color", &handleColor);
Command x           = Command("x", &handleX);
Command sweep       = Command("sweep", &handleSweep);

void setup()
{
  // Define the relay control pins as outputs and set them to HIGH = INACTIVE
  pinMode(relayPin01, OUTPUT);
  pinMode(relayPin02, OUTPUT);  
  digitalWrite(relayPin01, HIGH);
  digitalWrite(relayPin02, HIGH);

  pinMode(RLPin, OUTPUT);
  pinMode(GLPin, OUTPUT);
  pinMode(BLPin, OUTPUT);
  digitalWrite(RLPin, LOW);
  digitalWrite(GLPin, LOW);
  digitalWrite(BLPin, LOW);
  
  Serial.begin(9600);

  // Pre-defined commands
  commandLine.add(help);
  commandLine.add(power);
  commandLine.add(color);
  commandLine.add(x);
  commandLine.add(sweep);
  handleHelp(0);
}

void loop()
{
  commandLine.update();
}

//  Set the specified color referring to rainbow[][] with the given output power
void SetColor(unsigned char color, float power)
{
  unsigned char ucRed = 0;
  unsigned char ucGreen = 0;
  unsigned char ucBlue = 0;

  ucRed = char(rainbow[color][0]*power);
  ucGreen = char(rainbow[color][1]*power);
  ucBlue = char(rainbow[color][2]*power);
  
  if(ModuleStatus == LOW)
  {
     Serial.println("Power on the laser module before using this command.");
  }
  else
  { //All diodes off
    analogWrite(RLPin, rainbow[0][0]);
    analogWrite(GLPin, rainbow[0][1]);
    analogWrite(BLPin, rainbow[0][2]);
    delay(250);
    analogWrite(RLPin, ucRed);
    analogWrite(GLPin, ucGreen);
    analogWrite(BLPin, ucBlue);
    delay(250);
    /*
    Serial.print("The power dependent color code (RGB) is: ");
    Serial.print(ucRed);
    Serial.print(", ");
    Serial.print(ucGreen);
    Serial.print(", ");
    Serial.println(ucBlue);
    */
  }
}

void handleHelp(char* tokens)
{
  Serial.println(F("Laser Control Unit v0.12 - 01.04.2017"));
  Serial.println(F("Contact: philippe.kirsch@education.lu"));
  Serial.println(F("Always be careful when using a laser!"));
  Serial.println(F("This CLI (command line interface) enables you to control the RGB Laser Unit."));
  Serial.println(F("The following command are available:"));
  Serial.println(F("\t power on/off: \t Power ON  or OFF the laser module."));
  Serial.println(F("\t x: \t\t Turn OFF the laser beam."));
  Serial.println(F("\t sweep:\t\t Do a full spectrum color sweep."));
  Serial.println(F("\t help:\t\t Show these instructions."));
  Serial.println(F("\t Set the color with the following options:"));
  Serial.println(F("\t color [v]iolett, [b]lue, [c]yan, [g]reen, [y]ellow, [o]range, [r]ed, [w]hite"));
  
}

void handlePower(char* tokens)
{ char  option[8];
  char* token = strtok(NULL, " ");
  
  if (token == NULL)
  {
    Serial.println(F("Please specify an argument."));
  } 
  else 
  {
    strcpy(option, token);
    if (strcmp(option,"on") == 0)
    {
      ModuleStatus = HIGH;
      digitalWrite(relayPin01, LOW);
      digitalWrite(relayPin02, LOW);
      delay(250);
    }
    if (strcmp(option,"off") == 0)
    {
      ModuleStatus = LOW;
      digitalWrite(relayPin01, HIGH);
      digitalWrite(relayPin02, HIGH);
      delay(250);
    }
    if (strcmp(option,"low") == 0)
    {
      fPmax = 0.1;
      Serial.println(F("The laser module power has been set to 10%."));
    }
    if (strcmp(option,"normal") == 0)
    {
      fPmax = 0.25;
      Serial.println(F("The laser module power has been set to 25%."));
    }
    if (strcmp(option,"full") == 0)
    {
      fPmax = 1.0;
      Serial.println(F("The laser module power has been set to 100%."));
    }
  } 
}

void handleX(char* tokens)
{
  char* token = strtok(NULL, " ");

  SetColor(0, fPmax);
}

void handleColor(char* tokens)
{
  char* token = strtok(NULL, " ");
  unsigned char ucColor = 0;  //  default is off

  if (token == NULL)
  {
    Serial.println(F("Please specify a color."));
  } 
  else 
  {
    switch (*token)
    {
      case 'v': ucColor = 1;
              break;
      case 'b': ucColor = 2;
              break;
      case 'c': ucColor = 3;
              break;
      case 'g': ucColor = 4;
              break;
      case 'y': ucColor = 5;
              break;
      case 'o': ucColor = 6;
              break;
      case 'r': ucColor = 7;
              break;
      case 'w': ucColor = 8;
              break;
      default:  ucColor = 0;
    }
    /*
    Serial.print("The chosen color code (RGB) is: ");
    Serial.print(rainbow[ucColor][0]);
    Serial.print(", ");
    Serial.print(rainbow[ucColor][1]);
    Serial.print(", ");
    Serial.println(rainbow[ucColor][2]);
    */
    SetColor(ucColor, fPmax);
  }
}


void handleSweep(char* tokens)
{
  char* token = strtok(NULL, " ");
  unsigned char fadeTime = 100;
  unsigned char fadeTimeFactor = 10;
  const int Imin = 0;
  const int Imax = 255;
  const int Istep = 5;
  unsigned char phase;
  int wait = fadeTime * fadeTimeFactor;

  sweepmatrix matrix[8];

  matrix[0].color = BLPin;
  matrix[0].u_i   = Imin;
  matrix[0].r     = Istep;
  matrix[0].u_f   = Imax;

  matrix[1].color = GLPin;
  matrix[1].u_i   = Imin;
  matrix[1].r     = Istep;
  matrix[1].u_f   = Imax;

  matrix[2].color = BLPin;
  matrix[2].u_i   = Imax;
  matrix[2].r     = -Istep;
  matrix[2].u_f   = Imin;

  matrix[3].color  = RLPin;
  matrix[3].u_i   = Imin;
  matrix[3].r     = Istep;
  matrix[3].u_f   = Imax;

  matrix[4].color = GLPin;
  matrix[4].u_i   = Imax;
  matrix[4].r     = -Istep;
  matrix[4].u_f   = Imin;

  matrix[5].color = BLPin;
  matrix[5].u_i   = Imin;
  matrix[5].r     = Istep;
  matrix[5].u_f   = Imax;

  matrix[6].color = RLPin;
  matrix[6].u_i   = Imax;
  matrix[6].r     = -Istep;
  matrix[6].u_f   = Imin;

  matrix[7].color = BLPin;
  matrix[7].u_i   = Imax;
  matrix[7].r     = -Istep;
  matrix[7].u_f   = Imin;
                          
  if(ModuleStatus == HIGH)
  {
    Serial.println(F("Rainbow color sweep"));
    for(phase = 0; phase < 8; phase++)
    {
      Serial.print(F("Phase "));
      Serial.println(phase, DEC);
      while(matrix[phase].u_f - matrix[phase].u_i != 0)
      {
        matrix[phase].u_i = matrix[phase].u_i + matrix[phase].r;
        analogWrite(matrix[phase].color, matrix[phase].u_i);
         
        Serial.print("Pin: ");
        Serial.print(matrix[phase].color, DEC);
        Serial.print(", ");
        Serial.print("Intensity: ");
        Serial.print(matrix[phase].u_i, DEC);
         Serial.print(", ");
        Serial.print("Step: ");
        Serial.println(matrix[phase].r, DEC);
        
        delay(fadeTime);
      }
      delay(wait);
    }
    /*
    
    Serial.println(F("Phase 1"));
    for(i = 0; i < Imax; i++)    // Fade in Blue 
    {
      analogWrite(BLPin, Imax*i);
      delay(fadeTime);
    }
    delay(wait);
    Serial.println(F("Phase 2"));
    for(i = 0; i < Imax; i++)    // Fade in Green
    {
      analogWrite(GLPin, Imax*i);
      delay(fadeTime);
    }
    delay(wait);
    Serial.println(F("Phase 3"));
    for(i = Imax; i >= 0; i--)    // Fade out Blue
    {
      analogWrite(BLPin, Imax*i);
      delay(fadeTime);
    }
    delay(wait);
    Serial.println(F("Phase 4"));
    for(i = 0; i < Imax; i++)    // Fade in Red
    {
      analogWrite(RLPin, Imax*i);
      delay(fadeTime);
    }
    delay(wait);
    Serial.println(F("Phase 5"));
    for(i = Imax; i >= 0; i--)    // Fade out Green
    {
      analogWrite(GLPin, Imax*i);
      delay(fadeTime);
    }
    delay(wait);
    Serial.println(F("Phase 6"));
    for(i = 0; i < Imax; i++)    // Fade in Blue
    {
      analogWrite(BLPin, Imax*i);
      delay(fadeTime);
    }
    delay(wait);
    Serial.println(F("Phase 7"));
    for(i = Imax; i >= 0; i--)    // Fade out Red
    {
      analogWrite(RLPin, Imax*i);
      delay(fadeTime);
    }
    delay(wait);
    Serial.println(F("Phase 8"));
    for(i = Imax; i >= 0; i--)    // Fade out Blue
    {
      analogWrite(BLPin, Imax*i);
      delay(fadeTime);
    }
    */
  }
}
