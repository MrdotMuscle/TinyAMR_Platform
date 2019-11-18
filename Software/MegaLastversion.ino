#include <avr/interrupt.h>
long system_clock = 16000000;

long prescaler = 1;

#define DIR_1 44

#define STEP_1 5

#define CS1 40

#define DIR_2 48

#define STEP_2 46

#define CS2 53

#define DIR_3 30

#define STEP_3 11

#define CS3 34

#define DIR_4 28

#define STEP_4 6

#define CS4 24

#define MOSI 51

#define SCK 52

#define MS3_State LOW

#define SCALER_1 B00000001

#define SCALER_8 B00000010

#define SCALER_64 B00000011

#define SCALER_256 B00000100

#define SCALER_1024 B00000101

int scaler[] = {SCALER_1, SCALER_8, SCALER_64, SCALER_256, SCALER_1024};

int FREQ = 60;

int data[5] = {0,0,0,0,0};

int timernumber = 252;



void setup() {

  Serial.begin(115200);

  pinMode(MOSI, OUTPUT);

  digitalWrite(MOSI, HIGH);

  pinMode(SCK, OUTPUT);

  digitalWrite(SCK, HIGH);

  pinMode(CS1, OUTPUT);

  digitalWrite(CS1, MS3_State);

  pinMode(CS2, OUTPUT);

  digitalWrite(CS2, MS3_State);

  pinMode(CS3, OUTPUT);

  digitalWrite(CS3, MS3_State);

  pinMode(CS4, OUTPUT);

  digitalWrite(CS4, MS3_State);

  pinMode(12, OUTPUT);

  noInterrupts();

  TCCR2A = 0x00; // overflow mode

  TCCR2B = 0x05; // 128 prescaler

  TCNT2 = TimerInterruptFrequency(802);

  TIFR2 = 0x00;

  TIMSK2 = 0x01;  // enable timer compare interrupt A

  interrupts();             // enable all interrupts   

}

ISR(TIMER2_OVF_vect)          // timer compare interrupt service routine
{
  // motor frequency ++
  digitalWrite(12, digitalRead(12) ^ 1);
  TCNT2 = TimerInterruptFrequency(802);
  TIFR2 = 0x00;
}

long int TimerInterruptFrequency(long int Fqec){
  return 256 - 125000 / Fqec;
}




void motorFrequency(int motorNum, int frequency, int direct){

  //motorNum = 1  pin 5   

  //motorNum = 2  pin 46

  //motorNum = 3  pin 11

  //motorNum = 4  pin 6

  

  long Top = (system_clock /( frequency * 2 * prescaler)) - 1;   // TOP  frequency=(system clock)/ [2 * prescaler * (TOP +1)]

  switch(motorNum){

    case 1:
      pinMode(STEP_1, OUTPUT);

      TCCR3A = B01010111;    // Fast PWM

      TCCR3B = TCCR3B & B11100000 | B00011001;         // Prescaler = 1

      OCR3A = Top;

      pinMode(DIR_1, OUTPUT);

      if(direct){ 

        digitalWrite(DIR_1, HIGH);

      }else{

        digitalWrite(DIR_1, LOW);

      }

      break;

    case 2:

      pinMode(46, OUTPUT);

      TCCR4A = B01010111;    // Fast PWM

      TCCR4B = TCCR4B & B11100000 | B00011001;         // Prescaler = 1

      OCR4A = Top;

      pinMode(DIR_2, OUTPUT);

      if(direct){ 

        digitalWrite(DIR_2, HIGH);

      }else{

        digitalWrite(DIR_2, LOW);

      }

      break;

    case 3:

      pinMode(11, OUTPUT);

      TCCR1A = B01010111;    // Fast PWM

      TCCR1B = TCCR1B & B11100000 | B00011001;         // Prescaler = 1

      OCR1A = Top;

      pinMode(DIR_3, OUTPUT);

      if(direct){ 

        digitalWrite(DIR_3, HIGH);

      }else{

        digitalWrite(DIR_3, LOW);

      }

      break;

    case 4:

      pinMode(6, OUTPUT);

      TCCR5A = B01010111;    // Fast PWM

      TCCR5B = TCCR5B & B11100000 | B00011001;         // Prescaler = 1

      OCR5A = Top;

      pinMode(DIR_4, OUTPUT);

      if(direct){ 

        digitalWrite(DIR_4, HIGH);

      }else{

        digitalWrite(DIR_4, LOW);

      }

      break;

  }



}



int acc_dir = 1;

int mode_select = 1;

int F_Lim = 9000;

String sep = "=======================";

  void loop() {   
  char buff[9];
  if(Serial.available()){
//    digitalWrite(12, HIGH);
    Serial.readBytesUntil('\n', buff, 10);
    data[0] = (((int)buff[0]) << 8) | buff[1];
    data[1] = (((int)buff[2]) << 8) | buff[3];
    data[2] = (((int)buff[4]) << 8) | buff[5];
    data[3] = (((int)buff[6])<< 8) | buff[7];     
    data[4] = buff[8];    
//    digitalWrite(12, LOW);   
 }
  for(int i = 0; i < 5; i ++){
    Serial.println(data[i]);
    delayMicroseconds(300);
  }
  Serial.println(sep);


  if(mode_select){
    F_Lim = 12000;
    motorFrequency(1, FREQ, 1);  // motor number: 1, 2, 3, 4  frquency(speed) direction: 1, 0
    motorFrequency(2, FREQ, 1);
    motorFrequency(3, FREQ, 0);
    motorFrequency(4, FREQ, 0); 
  }
  else{
    F_Lim = 7500;
    motorFrequency(1, FREQ, 0);  // motor number: 1, 2, 3, 4  frquency(speed) direction: 1, 0
    motorFrequency(2, FREQ, 0);
    motorFrequency(3, FREQ, 0);  
    motorFrequency(4, FREQ, 0); 

  }

  

}
