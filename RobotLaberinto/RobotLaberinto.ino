#define enB 9
#define enA 10

#define lm1 5
#define lm2 4
#define rm1 7
#define rm2 6
#define offsetEnA 12

#define tCenter A3
#define eCenter A2
#define tRight A5
#define eRight A4
#define tLeft A1
#define eLeft A0

#define TURN_SPEED 100   // Velocidad de giro
#define FORWARD_SPEED 150 // Velocidad de avance
#define WALL_DISTANCE 20  // Distancia objetivo para seguir la pared


uint8_t maxLimitFront = 15;

uint16_t leftSensor, oldLeftSensor = 0;
uint16_t rightSensor, oldRightSensor = 0;
uint16_t centerSensor, oldCenterSensor = 0;


bool isFront = true;

void setup() {
  Serial.begin(9600);
  initPins();
  delay(1000);
}

void loop() {
  readSensors();
   // Verifica si hay un obstáculo en el frente
  if (centerSensor < maxLimitFront) {
    // Detiene el carro
    stop();

    // Verifica si hay espacio a la derecha para girar
    if (rightSensor > WALL_DISTANCE) {
      // Gira hacia la derecha
      turnRight();
    } else {
      // Si no hay espacio a la derecha, gira a la izquierda
      turnLeft();
    }
  } else {
    // Si no hay obstáculos en el frente, sigue la pared
    followWall();
  }
}




void followWall() {
  // Ajusta la velocidad de los motores para seguir la pared
  run();

  // Calcula la diferencia entre la distancia actual y la distancia objetivo
  int error = centerSensor - WALL_DISTANCE;

  // Ajusta la velocidad de los motores para corregir el error
  int leftSpeed = FORWARD_SPEED - error;
  int rightSpeed = FORWARD_SPEED + error;

  // Aplica las velocidades ajustadas a los motores
  analogWrite(enA, leftSpeed + offsetEnA);
  analogWrite(enB, rightSpeed);
}

void turnLeft() {
  // Realiza una maniobra de giro a la izquierda
  analogWrite(enA, TURN_SPEED + offsetEnA);
  analogWrite(enB, TURN_SPEED);
  digitalWrite(rm1, 1);
  digitalWrite(rm2, 0);
  digitalWrite(lm1, 0);
  digitalWrite(lm2, 1);
}



void speed(int vel) {
  analogWrite(enA, vel+offsetEnA);
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

  leftSensor = (lSensor + oldLeftSensor) / 2;
  rightSensor = (rSensor + oldRightSensor) / 2;
  centerSensor = (cSensor + oldCenterSensor) / 2;


  oldLeftSensor = leftSensor;
  oldRightSensor = rightSensor;
  oldCenterSensor = centerSensor;

  

}


void run() {
  analogWrite(enA, 150 + offsetEnA);
  analogWrite(enB, 150);
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

void turnRight() {
  analogWrite(enA, 100 + offsetEnA);
  analogWrite(enB, 100);
  digitalWrite(rm1, 0);
  digitalWrite(rm2, 1);
  digitalWrite(lm1, 1);
  digitalWrite(lm2, 0);
}
