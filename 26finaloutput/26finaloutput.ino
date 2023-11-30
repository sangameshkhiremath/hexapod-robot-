#define BLYNK_TEMPLATE_ID "TMPL3w0hiAWU5"
#define BLYNK_TEMPLATE_NAME "HEXAPOD"
#define BLYNK_AUTH_TOKEN "yce7PlJi6LSnClmAjjrNUGVRjMa6rs7G"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

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



#define HIP_MIN 150
#define HIP_MAX 617

int A = 0, B = 0, C = 0, D = 0, E = 0, F = 0, G = 0, H = 0;
bool Impair_start = false;


char ssid[] = "oppo";  // Replace with your Wi-Fi network name
char pass[] = "1213141516";  // Replace with your Wi-Fi network password

int FORWARD = 5;
int BACKWARD = 7;
int RIGHT = 9;
int LEFT = 11;
int ROTRIGHT = 13;
int ROTLEFT = 15;

int GOFORWARD = 6;
int GOBACKWARD = 8;
int GORIGHT = 10;
int GOLEFT = 12;
int GOROTRIGHT = 14;
int GOROTLEFT = 16;

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




BlynkTimer timer;

void setup() {
  Serial.begin(115200);

  pwm.begin();
  pwm.setPWMFreq(60);
  delay(10);

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
 Serial.println(" Connecting to blynk.cloud:80");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  
  // 6 TYPES MOTION FROM BLYNK AS INPUT AND PAIR PIN FOR CMND MOTION
  pinMode(FORWARD, OUTPUT);
  pinMode(GOFORWARD, INPUT);

  pinMode(BACKWARD, OUTPUT);
  pinMode(GOBACKWARD, INPUT);
  
  pinMode(RIGHT, OUTPUT);
  pinMode(GORIGHT, INPUT);

  pinMode(LEFT, OUTPUT);
  pinMode(GOLEFT, INPUT);

  pinMode(ROTRIGHT, OUTPUT);
  pinMode(GOROTRIGHT, INPUT);

  pinMode(ROTLEFT, OUTPUT);
  pinMode(GOROTLEFT, INPUT);

  
}



void loop() {
 int runMoveforward = digitalRead(GOFORWARD);
 int runMoveleft = digitalRead(GOLEFT);
 int runMoveright = digitalRead(GORIGHT);
 int runMovebackward = digitalRead(GOBACKWARD);
 int runMoverotleft = digitalRead(GOROTLEFT);

  if (runMoveforward==1) {
    move_frwd();  // Execute the move_left function
  }

  if (runMoveleft==1) {
    move_left();  // Execute the move_left function
  }

  if (runMoveright==1) {
    move_right();  // Execute the move_left function
  }

  if (runMovebackward==1) {
    move_bwd();  // Execute the move_left function
  }

  if (runMoverotleft==1) {
    rotate_left();  // Execute the move_left function
  }

  Blynk.run();
  timer.run();

  
}


void checkSwitchStates() {
  int forwardState1 = digitalRead(V0);
  int leftState1 = digitalRead(V1);
  int rightState1 = digitalRead(V2);
  int backwardState1 = digitalRead(V3);
  int rotateLeftState1 = digitalRead(V4);

  digitalWrite(FORWARD, forwardState1);
  digitalWrite(BACKWARD, backwardState1);
  digitalWrite(RIGHT, rightState1);
  digitalWrite(LEFT, leftState1);
  digitalWrite(ROTLEFT, rotateLeftState1);

  // Add any additional logic based on switch states here
}

BLYNK_WRITE(V0) {
  int forwardState = param.asInt();
  digitalWrite(FORWARD, forwardState); // Get the button state from the Blynk app
  // You can add an else condition if you want to perform some action when the button is released
}

BLYNK_WRITE(V1) {
  int leftState = param.asInt();
  digitalWrite(LEFT, leftState); // Get the button state from the Blynk app
  // You can add an else condition if you want to perform some action when the button is released
}

BLYNK_WRITE(V2) {
  int rightState = param.asInt();
  digitalWrite(RIGHT, rightState); // Get the button state from the Blynk app
  // You can add an else condition if you want to perform some action when the button is released
}

BLYNK_WRITE(V3) {
  int backwardState = param.asInt();
  digitalWrite(BACKWARD, backwardState); // Get the button state from the Blynk app
  // You can add an else condition if you want to perform some action when the button is released
}

BLYNK_WRITE(V4) {
  int rotateLeftState = param.asInt();
  digitalWrite(ROTLEFT, rotateLeftState); // Get the button state from the Blynk app
  // You can add an else condition if you want to perform some action when the button is released
}


void setServoPosition(uint8_t servoNum, uint16_t position) {
  pwm.setPWM(servoNum, 0, position);
}




void move_frwd() {
  // Impair Lift 10 deg
  if (A <= 10) {
    setServoPosition(Leg1_Mot2, map(home_Leg1_Mot2 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg1_Mot3, map(home_Leg1_Mot3 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot2, map(home_Leg3_Mot2 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot3, map(home_Leg3_Mot3 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot2, map(home_Leg5_Mot2 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot3, map(home_Leg5_Mot3 - A, 0, 180, HIP_MIN, HIP_MAX));
    A++;
  }

  // Impair rote fwd 20 deg
  if (B <= 20) {
    setServoPosition(Leg1_Mot1, map(home_Leg1_Mot1 - B, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot1, map(home_Leg3_Mot1 - B, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot1, map(home_Leg5_Mot1 + B, 0, 180, HIP_MIN, HIP_MAX));
    B++ ;
  }

  // Impair touch ground -10 deg
  if (B > 20 && C <= 10) {
    setServoPosition(Leg1_Mot2, map(home_Leg1_Mot2 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg1_Mot3, map(home_Leg1_Mot3 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot2, map(home_Leg3_Mot2 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot3, map(home_Leg3_Mot3 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot2, map(home_Leg5_Mot2 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot3, map(home_Leg5_Mot3 + C, 0, 180, HIP_MIN, HIP_MAX));
    C++;
  }

  // Impair rotate bwd -20 deg
  if (B >= 20) {
    setServoPosition(Leg1_Mot1, map(home_Leg1_Mot1 + D, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot1, map(home_Leg3_Mot1 + D, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot1, map(home_Leg5_Mot1 - D, 0, 180, HIP_MIN, HIP_MAX));
    D++;

    Impair_start = true;
  }
  if (D >= 20) {
    A = 0;
    B = 0;
    C = 0;
    D = 0;
  }


  if (Impair_start) {
    // Pair Lift 10 deg
    if (E <= 10) {
      setServoPosition(Leg2_Mot2, map(home_Leg2_Mot2 - E, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg2_Mot3, map(home_Leg2_Mot3 - E, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot2, map(home_Leg4_Mot2 - E, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot3, map(home_Leg4_Mot3 - E, 0, 180, HIP_MIN, HIP_MAX));
      //setServoPosition(Leg6_Mot2, home_Leg6_Mot2 - E);
      //setServoPosition(Leg6_Mot3, home_Leg6_Mot3 - E);
      E++;
    }
    // Pair rote fwd 20 deg
    if (F <= 20) {
      setServoPosition(Leg2_Mot1, map(home_Leg2_Mot1 - F, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot1, map(home_Leg4_Mot1 + F, 0, 180, HIP_MIN, HIP_MAX));
      //setServoPosition(Leg6_Mot1, home_Leg6_Mot1 + F, 0, 180, HIP_MIN, HIP_MAX);
      F++;
    }
    // Pair touch ground -10 deg
    if (F > 20 && G <= 10) {
      setServoPosition(Leg2_Mot2, map(home_Leg2_Mot2 + G, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg2_Mot3, map(home_Leg2_Mot3 + G, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot2, map(home_Leg4_Mot2 + G, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot3, map(home_Leg4_Mot3 + G, 0, 180, HIP_MIN, HIP_MAX));
      //setServoPosition(Leg6_Mot2, home_Leg6_Mot2 + G, 0, 180, HIP_MIN, HIP_MAX);
      //setServoPosition(Leg6_Mot3, home_Leg6_Mot3 + G, 0, 180, HIP_MIN, HIP_MAX);
      G++;
    }
    // Pair rotate bwd -20 deg
    if (F >= 20) {
      setServoPosition(Leg2_Mot1, map(home_Leg2_Mot1 + H, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot1, map(home_Leg4_Mot1 - H, 0, 180, HIP_MIN, HIP_MAX));
      //setServoPosition(Leg6_Mot1, map(home_Leg6_Mot1 - H, 0, 180, HIP_MIN, HIP_MAX);
      H++;
    }
    if (H >= 20) {
      Impair_start = false;
      E = 0;
      F = 0;
      H = 0;
    }
  }
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



void move_right() {
  // Impair Motor 2 -20 Motor 3 +20
  if (A <= 20) {
    setServoPosition(Leg1_Mot2, map(home_Leg1_Mot2 + A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg1_Mot3, map(home_Leg1_Mot3 + A, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg2_Mot2, map(home_Leg2_Mot2 + A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg2_Mot3, map(home_Leg2_Mot3 - A * 2, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg3_Mot2, map(home_Leg3_Mot2 + A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot3, map(home_Leg3_Mot3 + A, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg4_Mot2, map(home_Leg4_Mot2 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg4_Mot3, map(home_Leg4_Mot3 + A * 2, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg5_Mot2, map(home_Leg5_Mot2 + A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot3, map(home_Leg5_Mot3 - A, 0, 180, HIP_MIN, HIP_MAX));

    //setServoPosition(Leg6_Mot2, map(home_Leg6_Mot2 - A, 0, 180, HIP_MIN, HIP_MAX));
    //setServoPosition(Leg6_Mot3, map(home_Leg6_Mot3 + A * 2, 0, 180, HIP_MIN, HIP_MAX));

    A++;
  }

  if (A >= 20 && B <= 20) {
    setServoPosition(Leg2_Mot2, map(home_Leg2_Mot2 + A + B, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg2_Mot3, map(home_Leg2_Mot3 - A * 2 + B * 2, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg4_Mot2, map(home_Leg4_Mot2 - A + B, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg4_Mot3, map(home_Leg4_Mot3 + A * 2 - B * 2, 0, 180, HIP_MIN, HIP_MAX));

    //setServoPosition(Leg6_Mot2, map(home_Leg6_Mot2 - A + B, 0, 180, HIP_MIN, HIP_MAX));
    //setServoPosition(Leg6_Mot3, map(home_Leg6_Mot3 + A * 2 - B * 2, 0, 180, HIP_MIN, HIP_MAX));
    B++;
  }

  if (B >= 20 && C <= 20) {
    setServoPosition(Leg1_Mot2, map(home_Leg1_Mot2 + A - C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg1_Mot3, map(home_Leg1_Mot3 + A - A, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg3_Mot2, map(home_Leg3_Mot2 + A - C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot3, map(home_Leg3_Mot3 + A - C, 0, 180, HIP_MIN, HIP_MAX));

    setServoPosition(Leg5_Mot2, map(home_Leg5_Mot2 + A - C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot3, map(home_Leg5_Mot3 - A + C, 0, 180, HIP_MIN, HIP_MAX));

    C++;
  }

  if (C >= 20) {
    A = 0;
    B = 0;
    C = 0;
  }
}

void move_bwd() {
  // Impair Lift 10 deg
  if (A <= 10) {
    setServoPosition(Leg1_Mot2, map(home_Leg1_Mot2 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg1_Mot3, map(home_Leg1_Mot3 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot2, map(home_Leg3_Mot2 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot3, map(home_Leg3_Mot3 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot2, map(home_Leg5_Mot2 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot3, map(home_Leg5_Mot3 - A, 0, 180, HIP_MIN, HIP_MAX));
    A++;
  }

  // Impair rote fwd 20 deg
  if (B <= 20) {
    setServoPosition(Leg1_Mot1, map(home_Leg1_Mot1 + B, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot1, map(home_Leg3_Mot1 + B, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot1, map(home_Leg5_Mot1 - B, 0, 180, HIP_MIN, HIP_MAX));
    B++;
  }

  // Impair touch ground -10 deg
  if (B > 20 && C <= 10) {
    setServoPosition(Leg1_Mot2, map(home_Leg1_Mot2 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg1_Mot3, map(home_Leg1_Mot3 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot2, map(home_Leg3_Mot2 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot3, map(home_Leg3_Mot3 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot2, map(home_Leg5_Mot2 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot3, map(home_Leg5_Mot3 + C, 0, 180, HIP_MIN, HIP_MAX));
    C++;
  }

  // Impair rotate bwd -20 deg
  if (B >= 20) {
    setServoPosition(Leg1_Mot1, map(home_Leg1_Mot1 - D, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot1, map(home_Leg3_Mot1 - D, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot1, map(home_Leg5_Mot1 + D, 0, 180, HIP_MIN, HIP_MAX));
    D++;
    Impair_start = true;
  }
  if (D >= 20) {
    A = 0;
    B = 0;
    C = 0;
    D = 0;
  }

  //////////////////////////////////

  if (Impair_start) {
    // Pair Lift 10 deg
    if (E <= 10) {
      setServoPosition(Leg2_Mot2, map(home_Leg2_Mot2 - E, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg2_Mot3, map(home_Leg2_Mot3 - E, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot2, map(home_Leg4_Mot2 - E, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot3, map(home_Leg4_Mot3 - E, 0, 180, HIP_MIN, HIP_MAX));
     // setServoPosition(Leg6_Mot2, map(home_Leg6_Mot2 - E, 0, 180, HIP_MIN, HIP_MAX));
      //setServoPosition(Leg6_Mot3, map(home_Leg6_Mot3 - E, 0, 180, HIP_MIN, HIP_MAX));
      E++;
    }
    // Pair rote fwd 20 deg
    if (F <= 20) {
      setServoPosition(Leg2_Mot1, map(home_Leg2_Mot1 + F, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot1, map(home_Leg4_Mot1 - F, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg6_Mot1, map(home_Leg6_Mot1 - F, 0, 180, HIP_MIN, HIP_MAX));
      F++;
    }

    // Pair touch ground -10 deg
    if (F > 20 && G <= 10) {
      setServoPosition(Leg2_Mot2, map(home_Leg2_Mot2 + G, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg2_Mot3, map(home_Leg2_Mot3 + G, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot2, map(home_Leg4_Mot2 + G, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot3, map(home_Leg4_Mot3 + G, 0, 180, HIP_MIN, HIP_MAX));
      //setServoPosition(Leg6_Mot2, map(home_Leg6_Mot2 + G, 0, 180, HIP_MIN, HIP_MAX));
      //setServoPosition(Leg6_Mot3, map(home_Leg6_Mot3 + G, 0, 180, HIP_MIN, HIP_MAX));
      G++;
    }

    // Pair rotate bwd -20 deg
    if (F >= 20) {
      setServoPosition(Leg2_Mot1, map(home_Leg2_Mot1 - H, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot1, map(home_Leg4_Mot1 + H, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg6_Mot1, map(home_Leg6_Mot1 + H, 0, 180, HIP_MIN, HIP_MAX));
      H++;
    }
    if (H >= 20) {
      Impair_start = false;
      E = 0;
      F = 0;
      G = 0;
      H = 0;
    }
  }
}  // End Move Backward


void rotate_left() {
  // Impair Lift 10 deg
  if (A <= 10) {
    setServoPosition(Leg1_Mot2, map(home_Leg1_Mot2 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg1_Mot3, map(home_Leg1_Mot3 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot2, map(home_Leg3_Mot2 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot3, map(home_Leg3_Mot3 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot2, map(home_Leg5_Mot2 - A, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot3, map(home_Leg5_Mot3 - A, 0, 180, HIP_MIN, HIP_MAX));
    A++;
  }

  // Impair rote fwd 30 deg
  if (B <= 20) {
    setServoPosition(Leg1_Mot1, map(home_Leg1_Mot1 - B, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot1, map(home_Leg3_Mot1 - B, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot1, map(home_Leg5_Mot1 - B, 0, 180, HIP_MIN, HIP_MAX));
    B++;
  }

  // Impair touch ground -10 deg
  if (B > 20 && C <= 10) {
    setServoPosition(Leg1_Mot2, map(home_Leg1_Mot2 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg1_Mot3, map(home_Leg1_Mot3 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot2, map(home_Leg3_Mot2 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot3, map(home_Leg3_Mot3 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot2, map(home_Leg5_Mot2 + C, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot3, map(home_Leg5_Mot3 + C, 0, 180, HIP_MIN, HIP_MAX));
    C++;
  }

  // Impair rotate bwd -30 deg
  if (B >= 20) {
    setServoPosition(Leg1_Mot1, map(home_Leg1_Mot1 + D, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg3_Mot1, map(home_Leg3_Mot1 + D, 0, 180, HIP_MIN, HIP_MAX));
    setServoPosition(Leg5_Mot1, map(home_Leg5_Mot1 + D, 0, 180, HIP_MIN, HIP_MAX));
    D++;
    Impair_start = true;
  }
  if (D >= 20) {
    A = 0;
    B = 0;
    C = 0;
    D = 0;
  }

  //////////////////////////////////

  if (Impair_start) {
    // Pair Lift 10 deg
    if (E <= 10) {
      setServoPosition(Leg2_Mot2, map(home_Leg2_Mot2 - E, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg2_Mot3, map(home_Leg2_Mot3 - E, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot2, map(home_Leg4_Mot2 - E, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot3, map(home_Leg4_Mot3 - E, 0, 180, HIP_MIN, HIP_MAX));
      //setServoPosition(Leg6_Mot2, map(home_Leg6_Mot2 - E, 0, 180, HIP_MIN, HIP_MAX));
     // setServoPosition(Leg6_Mot3, map(home_Leg6_Mot3 - E, 0, 180, HIP_MIN, HIP_MAX));
      E++;
    }
    // Pair rote fwd 30 deg
    if (F <= 20) {
      setServoPosition(Leg2_Mot1, map(home_Leg2_Mot1 - F, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot1, map(home_Leg4_Mot1 - F, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg6_Mot1, map(home_Leg6_Mot1 - F, 0, 180, HIP_MIN, HIP_MAX));
      F++;
    }

    // Pair touch ground -10 deg
    if (F > 20 && G <= 10) {
      setServoPosition(Leg2_Mot2, map(home_Leg2_Mot2 + G, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg2_Mot3, map(home_Leg2_Mot3 + G, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot2, map(home_Leg4_Mot2 + G, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot3, map(home_Leg4_Mot3 + G, 0, 180, HIP_MIN, HIP_MAX));
      //setServoPosition(Leg6_Mot2, map(home_Leg6_Mot2 + G, 0, 180, HIP_MIN, HIP_MAX));
      //setServoPosition(Leg6_Mot3, map(home_Leg6_Mot3 + G, 0, 180, HIP_MIN, HIP_MAX));
      G++;
    }

    // Pair rotate bwd -30 deg
    if (F >= 20) {
      setServoPosition(Leg2_Mot1, map(home_Leg2_Mot1 + H, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg4_Mot1, map(home_Leg4_Mot1 + H, 0, 180, HIP_MIN, HIP_MAX));
      setServoPosition(Leg6_Mot1, map(home_Leg6_Mot1 + H, 0, 180, HIP_MIN, HIP_MAX));
      H++;
    }
    if (H >= 20) {
      Impair_start = false;
      E = 0;
      F = 0;
      G = 0;
      H = 0;
    }
  }
}  // End Rotate Left





