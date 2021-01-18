#include <Unistep2.h>
Unistep2 stepper(8, 9, 10, 11, 4096, 1000);// IN1, IN2, IN3, IN4, 總step數, 每步的延遲(in micros)

#define WaterDetPin A0
#define ThdCtlPin A5
#define LEDPin 13
int water_over = 0;
int ALL_stop = 0;
// #define BuzzerPin = 4;

// int duration = 200; // 200 miliseconds

// #define DO 264
// #define RE 297
// #define MI 330
// #define FA 352666

// int melody[] = {
//   DO, RE, MI, FA
// };

void setup() {
  Serial.begin(9600);
  pinMode(LEDPin, OUTPUT);
  pinMode(WaterDetPin, INPUT_PULLUP);
  pinMode(ThdCtlPin, INPUT_PULLUP);
  digitalWrite(LEDPin, LOW);
}

void checkStateChange() {
  int waterState = analogRead(WaterDetPin);
  int sensVal = analogRead(ThdCtlPin);
  sensVal = (sensVal - 28)*40;
  Serial.print(waterState);
  Serial.print("/");
  Serial.println(sensVal);

  if (waterState < sensVal) {
    digitalWrite(LEDPin, HIGH);
    // for (int thisNote = 0; thisNote < 4; thisNote++) {
    //  // tone(BuzzerPin, melody[thisNote], duration);
    //  delay(300);
    // }
    water_over = 1;
  } else {
    digitalWrite(LEDPin, LOW);
  }
  delay(300);
}

void loop() {
  if (water_over == 0){checkStateChange();}
  else if (ALL_stop == 0){
    stepper.run();
    if ( stepper.stepsToGo() == 0 ){ // 如果stepsToGo=0，表示步進馬達已轉完應走的step了
      delay(500);
      stepper.move(5*4096ul);
      ALL_stop = 1;
    }
  }
  if ( stepper.stepsToGo() != 0 ){stepper.run();}
  else if (ALL_stop){Serial.println("stop");delay(10*60*1000ul);}

}
