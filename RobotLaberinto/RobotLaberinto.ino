#define enB 9   //<Motor Izquierda
#define enA 10  //<Motor Derecha

#define lm1 5
#define lm2 4
#define rm1 7
#define rm2 6
#define offsetEnA 12

#define FORWARD_SPEED 200
#define tCenter A3
#define eCenter A2
#define tRight A5
#define eRight A4
#define tLeft A1
#define eLeft A0


uint8_t maxLimitFront = 20;

uint16_t leftSensor, oldLeftSensor = 0;
uint16_t rightSensor, oldRightSensor = 0;
uint16_t centerSensor, oldCenterSensor = 0;

bool bturnRight = false;
bool bturnLeft = false;

void setup() {
  Serial.begin(9600);
  initPins();
  delay(1000);
}

void loop() {
  readSensors();

  //Verifica condicion de parada.
  if (centerSensor < maxLimitFront) {
    stop();
    //Bandera que permite activar la funcion de doblar hacia la derecha
    if (rightSensor > 20) {
      bturnRight = true;
    }

    //Bandear que me permite activar la funcion de doblar hacia la izquierda.
    if (leftSensor > 20) {
      bturnLeft = true;
    }
  }

  //Si el sensor central detecta una distancia mayor a la limite, seguirá hacia adelante
  if (centerSensor > maxLimitFront) {
    run();
    followWall();
  }

  // Si la bandera esta en true, permite doblar hacia la derecha
  if (bturnRight) {
    fTurnRight();
    bturnRight = false;
  }
  //Si la bandera esta en true, permite doblar hacia la izquierda.
  if (bturnLeft) {
    fTurnLeft();
    bturnLeft = false;
  }
}


void speed(int vel) {
  analogWrite(enA, vel + offsetEnA);
  analogWrite(enB, vel);
}

void initPins() {
  for (int i = 4; i <= 10; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(A0, INPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, INPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, INPUT);
  pinMode(A5, OUTPUT);
}


uint16_t readSensor(uint8_t trigger, uint8_t echo) {
  uint16_t t;  //timepo que demora en llegar el eco
  uint16_t d;  //distancia en centimetros

  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);  //Enviamos un pulso de 10us
  digitalWrite(trigger, LOW);

  t = pulseIn(echo, HIGH);  //obtenemos el ancho del pulso
  d = t / 59;               //escalamos el tiempo a una distancia en cm
  return d;
}

void readSensors() {
  uint16_t lSensor = readSensor(tLeft, eLeft);
  uint16_t rSensor = readSensor(tRight, eRight);
  uint16_t cSensor = readSensor(tCenter, eCenter);

  leftSensor = ((lSensor > 700 ? 0 : lSensor) + oldLeftSensor) / 2;
  rightSensor = ((rSensor > 700 ? 0 : rSensor) + oldRightSensor) / 2;
  centerSensor = ((cSensor > 700 ? 0 : cSensor) + oldCenterSensor) / 2;
  // Serial.print("left: ");
  //Serial.print(leftSensor);
  //Serial.print(" right: ");
  //Serial.print(rightSensor);
  //Serial.print(" center: ");
  //Serial.println(centerSensor);

  oldLeftSensor = leftSensor;
  oldRightSensor = rightSensor;
  oldCenterSensor = centerSensor;
}

//funcion que me permite doblar hacia la derecha
void fTurnRight() {
  //Controlar por medio del sensor del centro hasta cuando doblar hacia la derecha
  analogWrite(enA, 90 + offsetEnA);
  analogWrite(enB, 90);

  digitalWrite(rm1, 0);
  digitalWrite(rm2, 1);
  digitalWrite(lm1, 1);
  digitalWrite(lm2, 0);
}


void fTurnLeft() {
  //Controlar por medio del sensor del centro hasta cuando doblar hacia la derecha
  analogWrite(enA, 90 + offsetEnA);
  analogWrite(enB, 90);

  digitalWrite(rm1, 1);
  digitalWrite(rm2, 0);
  digitalWrite(lm1, 0);
  digitalWrite(lm2, 1);
}
void followWall() {


  int error = (leftSensor - rightSensor);

  int leftSpeed = FORWARD_SPEED;
  int rightSpeed = FORWARD_SPEED;
  if (error >= 0) {

    leftSpeed = FORWARD_SPEED - error * 5;

  } else {

    rightSpeed = FORWARD_SPEED + error * 5;
  }
  Serial.print("left: ");
  Serial.print(leftSpeed);
  Serial.print(" right: ");
  Serial.println(rightSpeed);


  analogWrite(enA, rightSpeed + offsetEnA);
  analogWrite(enB, leftSpeed);
}



void run() {
  digitalWrite(rm1, 1);
  digitalWrite(rm2, 0);
  digitalWrite(lm1, 1);
  digitalWrite(lm2, 0);
}
void stop() {

  digitalWrite(rm1, 0);
  digitalWrite(rm2, 0);
  digitalWrite(lm1, 0);
  digitalWrite(lm2, 0);
}