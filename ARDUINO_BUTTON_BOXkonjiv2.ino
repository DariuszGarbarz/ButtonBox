//BUTTON BOX + Hshifterlogitech + handbrake + throttle
//USE w Leonardo
//AMSTUDIO +konji + kutokonrad

#include <Keypad.h>
#include <Joystick.h>
#include <Encoder.h>

#define ENABLE_PULLUPS
#define NUMROTARIES 3
#define NUMBUTTONS 28
#define NUMROWS 4
#define NUMCOLS 4


byte buttons[NUMROWS][NUMCOLS] = {
  {0,7,9,10},
  {11,12,13,14},
  {15,16,17,18},
  {19,20,21,22},
};



byte rowPins[NUMROWS] = {13,12,11,10}; 
byte colPins[NUMCOLS] = {9,8,7,A4}; 

Keypad buttbx = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS); 
Encoder myEncA(0, 6);
Encoder myEncB(3, 4);
Encoder myEncC(1, 5);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_JOYSTICK, 32, 0,
  false, false, false, false, false, false,
  false, true, false, true, false);

// H-shifter mode analog axis thresholds
#define HS_XAXIS_12        300
#define HS_XAXIS_56        600
#define HS_YAXIS_135       800
#define HS_YAXIS_246       300

// Sequential shifter mode analog axis thresholds
#define SS_UPSHIFT_BEGIN   670
#define SS_UPSHIFT_END     600
#define SS_DOWNSHIFT_BEGIN 430
#define SS_DOWNSHIFT_END   500

// Handbrake mode analog axis limits
#define HB_MAXIMUM         530
#define HB_MINIMUM         400
#define HB_RANGE           (HB_MAXIMUM-HB_MINIMUM)

// Digital inputs definitions
#define DI_REVERSE         1
#define DI_MODE            3
#define DI_RED_CENTERRIGHT 4
#define DI_RED_CENTERLEFT  5
#define DI_RED_RIGHT       6
#define DI_RED_LEFT        7
#define DI_BLACK_TOP       8
#define DI_BLACK_RIGHT     9
#define DI_BLACK_LEFT      10
#define DI_BLACK_BOTTOM    11
#define DI_DPAD_RIGHT      12
#define DI_DPAD_LEFT       13
#define DI_DPAD_BOTTOM     14
#define DI_DPAD_TOP        15

// Shifter state
#define DOWN_SHIFT         -1
#define NO_SHIFT           0
#define UP_SHIFT           1

// Shifter mode
#define SHIFTER_MODE       0
#define HANDBRAKE_MODE     1

// LED blink counter
int led=0;

// Shifter state
int shift=NO_SHIFT;

// Handbrake mode
int mode=SHIFTER_MODE;

int b[16];

int gear=0;                          // Default value is neutral

// Constant that maps the phyical pin to the joystick button.
//const int pinToButtonMap = 9;  

void setup() {
  Joystick.begin();
     // G29 shifter analog inputs configuration 
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);   // X axis
  pinMode(A2, INPUT_PULLUP);   // Y axis
  pinMode(A1, INPUT_PULLUP);   // HandBrake
  pinMode(A3, INPUT_PULLUP);   // Throttle

  pinMode(2, INPUT); 
  
  for(int i=0; i<16; i++) b[i] = 0;
  b[DI_MODE] =0;
  // Initialize Joystick Library
  Joystick.begin();
  }

long oldPositionA  = -999;
long oldPositionB  = -999;
long oldPositionC  = -999;

void loop() { 

{int pot = analogRead(A3);
int mapped = map(pot,0,1023,0,255);
{Joystick.setThrottle(mapped);}}

{int pot = analogRead(A1);
int mapped = map(pot,0,1023,0,255);
{Joystick.setBrake(mapped);}}


  CheckAllButtons();
  int x=analogRead(0);                 // X axis
  int y=analogRead(2);                 // Y axis
  

  int _isreverse = digitalRead(2);
  int _gear_ = 0;

if( _isreverse == 1 ){

      _gear_ = 8;
      b[DI_REVERSE]= 1;

}else{ 
  

  if(b[DI_MODE]==0)                    // H-shifter mode?
  {
    if(x<HS_XAXIS_12)                  // Shifter on the left?
    {
      if(y>HS_YAXIS_135) _gear_=1;       // 1st gear
      if(y<HS_YAXIS_246) _gear_=2;       // 2nd gear
    }
    else if(x>HS_XAXIS_56)             // Shifter on the right?
    {
      if(y>HS_YAXIS_135) _gear_=5;       // 5th gear
      if(y<HS_YAXIS_246) _gear_=6;       // 6th gear
     
    }
    else                               // Shifter is in the middle
    {
      if(y>HS_YAXIS_135) _gear_=3;       // 3rd gear
      if(y<HS_YAXIS_246) _gear_=4;       // 4th gear
    }
   
  }

}
  
  
  if(gear!=6) b[DI_REVERSE]=0;         // Reverse gear is allowed only on 6th gear position
  
   if (_gear_ != gear ){
      gear = _gear_;
      desactivar();
      Joystick.setButton(gear-1, HIGH);
   }
   delay(50);

long newPositionA = myEncA.read();
  if (newPositionA > oldPositionA) {
    oldPositionA = newPositionA;
    Joystick.setButton(23, 1); delay(50); Joystick.setButton(23, 0);
  }
if (newPositionA < oldPositionA) {
    oldPositionA = newPositionA;
    Joystick.setButton(24, 1); delay(50); Joystick.setButton(24, 0);
  }
long newPositionB = myEncB.read();
  if (newPositionB > oldPositionB) {
    oldPositionB = newPositionB;
    Joystick.setButton(25, 1); delay(50); Joystick.setButton(25, 0);
  }
if (newPositionB < oldPositionB) {
    oldPositionB = newPositionB;
    Joystick.setButton(26, 1); delay(50); Joystick.setButton(26, 0);
  }  
long newPositionC = myEncC.read();
  if (newPositionC > oldPositionC) {
    oldPositionC = newPositionC;
    Joystick.setButton(27, 1); delay(50); Joystick.setButton(27, 0);
  }
if (newPositionC < oldPositionC) {
    oldPositionC = newPositionC;
    Joystick.setButton(28, 1); delay(50); Joystick.setButton(28, 0);
  }
    
}

void CheckAllButtons(void) {
      if (buttbx.getKeys())
    {
       for (int i=0; i<LIST_MAX; i++)   
        {
           if ( buttbx.key[i].stateChanged )   
            {
            switch (buttbx.key[i].kstate) {  
                    case PRESSED:
                    case HOLD:
                              Joystick.setButton(buttbx.key[i].kchar, 1);
                              break;
                    case RELEASED:
                    case IDLE:
                              Joystick.setButton(buttbx.key[i].kchar, 0);
                              break;
            }
           }   
         }
     }
}

void desactivar(){
  // Depress virtual button for current gear
  for(int i = 0; i <= 10 ; i++ )  Joystick.setButton(i, LOW);
}
