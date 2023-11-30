#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define Leg1_Mot1 0
#define Leg1_Mot2 1
#define Leg1_Mot3 2
#define Leg2_Mot1 3
#define Leg2_Mot2 4
#define Leg2_Mot3 5
#define Leg3_Mot1 6
#define Leg3_Mot2 7
#define Leg3_Mot3 8
#define Leg4_Mot1 9
#define Leg4_Mot2 10
#define Leg4_Mot3 11
#define Leg5_Mot1 12
#define Leg5_Mot2 13
#define Leg5_Mot3 14
#define Leg6_Mot1 15
//#define Leg6_Mot2 16
//#define Leg6_Mot3 17

int home_Leg1_Mot1 = 90;
int home_Leg1_Mot2 = 80;
int home_Leg1_Mot3 = 70;

int home_Leg2_Mot1 = 90;
int home_Leg2_Mot2 = 80;
int home_Leg2_Mot3 = 70;

int home_Leg3_Mot1 = 90;
int home_Leg3_Mot2 = 80;
int home_Leg3_Mot3 = 70;

int home_Leg4_Mot1 = 90;
int home_Leg4_Mot2 = 100;
int home_Leg4_Mot3 = 110;

int home_Leg5_Mot1 = 90;
int home_Leg5_Mot2 = 100;
int home_Leg5_Mot3 = 110;

int home_Leg6_Mot1 = 90;
//int home_Leg6_Mot2 = 100;
//int home_Leg6_Mot3 = 110;

#define HIP_MIN 150
#define HIP_MAX 617

int A = 0, B = 0, C = 0, D = 0, E = 0, F = 0, G = 0, H = 0;
bool Impair_start = false;

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60);
  delay(10);
}

void setServoPosition(uint8_t servoNum, uint16_t position) {
  pwm.setPWM(servoNum, 0, position);
}

void setAllServoPositions(int pulse1, int pulse2, int pulse3, int pulse4, int pulse5, int pulse6, int pulse7, int pulse8, int pulse9, int pulse10, int pulse11, int pulse12, int pulse13, int pulse14, int pulse15, int pulse16) {
  setServoPosition(Leg1_Mot1, pulse1);
  setServoPosition(Leg1_Mot2, pulse2);
  setServoPosition(Leg1_Mot3, pulse3);

  setServoPosition(Leg2_Mot1, pulse4);
  setServoPosition(Leg2_Mot2, pulse5);
  setServoPosition(Leg2_Mot3, pulse6);

  setServoPosition(Leg3_Mot1, pulse7);
  setServoPosition(Leg3_Mot2, pulse8);
  setServoPosition(Leg3_Mot3, pulse9);

  setServoPosition(Leg4_Mot1, pulse10);
  setServoPosition(Leg4_Mot2, pulse11);
  setServoPosition(Leg4_Mot3, pulse12);

  setServoPosition(Leg5_Mot1, pulse13);
  setServoPosition(Leg5_Mot2, pulse14);
  setServoPosition(Leg5_Mot3, pulse15);

  setServoPosition(Leg6_Mot1, pulse16);
  //setServoPosition(Leg6_Mot2, pulse17);
  //setServoPosition(Leg6_Mot3, pulse18);
}

void initializeHexapod() {
  // Set initial positions for each servo in each leg
  setAllServoPositions(
    map(90, 0, 180, HIP_MIN, HIP_MAX), map(80, 0, 180, HIP_MIN, HIP_MAX), map(70, 0, 180, HIP_MIN, HIP_MAX),  // Leg1
    map(90, 0, 180, HIP_MIN, HIP_MAX), map(80, 0, 180, HIP_MIN, HIP_MAX), map(70, 0, 180, HIP_MIN, HIP_MAX),  // Leg2
    map(90, 0, 180, HIP_MIN, HIP_MAX), map(80, 0, 180, HIP_MIN, HIP_MAX), map(70, 0, 180, HIP_MIN, HIP_MAX),  // Leg3
    map(90, 0, 180, HIP_MIN, HIP_MAX), map(100, 0, 180, HIP_MIN, HIP_MAX), map(110, 0, 180, HIP_MIN, HIP_MAX),  // Leg4
    map(90, 0, 180, HIP_MIN, HIP_MAX), map(100, 0, 180, HIP_MIN, HIP_MAX), map(110, 0, 180, HIP_MIN, HIP_MAX),  // Leg5
    map(90, 0, 180, HIP_MIN, HIP_MAX)  // Leg6
  );
   
}
  void move_left() {
  // Impair Motor 2 -20 Motor 3 +20
  if (A <= 20) {
    setServoPosition(Leg4_Mot2, map(home_Leg4_Mot2 + A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg4_Mot3, map(home_Leg4_Mot3 + A, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg5_Mot2, map(home_Leg5_Mot2 + A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot3, map(home_Leg5_Mot3 - A * 2, 0, 180, HIP_MIN, HIP_MAX));

   // setServoPosition(Leg6_Mot2, map(home_Leg6_Mot2 + A, 0, 180, HIP_MIN, HIP_MAX));
    //setServoPosition(Leg6_Mot3, map(home_Leg6_Mot3 + A, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg3_Mot2, map(home_Leg4_Mot3 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot3, map(home_Leg4_Mot3 + A * 2, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg2_Mot2, map(home_Leg2_Mot2 + A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg2_Mot3, map(home_Leg2_Mot3 - A, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg1_Mot2, map(home_Leg1_Mot2 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg1_Mot3, map(home_Leg1_Mot3 + A * 2, 0, 180, HIP_MIN, HIP_MAX));

    A++;
  }

  if (A >= 20 && B <= 20) {
    setServoPosition(Leg5_Mot2, map(home_Leg5_Mot2 + A + B, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot3, map(home_Leg5_Mot3 - A * 2 + B * 2, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg3_Mot2, map(home_Leg3_Mot2 - A + B, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot3, map(home_Leg3_Mot3 + A * 2 - B * 2, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg1_Mot2, map(home_Leg1_Mot2 - A + B, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg1_Mot3, map(home_Leg1_Mot3 + A * 2 - B * 2, 0, 180, HIP_MIN, HIP_MAX));

    B++;
  }

  if (B >= 20 && C <= 20) {
    //setServoPosition(Leg6_Mot2, map(home_Leg6_Mot2 + A - C, 0, 180, HIP_MIN, HIP_MAX));
    //setServoPosition(Leg6_Mot3, map(home_Leg6_Mot3 + A - A, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg4_Mot2, map(home_Leg4_Mot2 + A - C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg4_Mot3, map(home_Leg4_Mot3 + A - C, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg2_Mot2, map(home_Leg2_Mot2 + A - C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg2_Mot3, map(home_Leg2_Mot3 - A + C, 0, 180, HIP_MIN, HIP_MAX));

    C++;
  }
  if (C >= 20) {
    A = 0;
    B = 0;
    C = 0;
  }
}





void loop() {
//  initializeHexapod(); // Initialize hexapod
// delay(500); // 2 seconds delay
// while (true) {
   move_left() ; // Continuous forward movement
//delay(500); // Adjust delay as needed
//}
}