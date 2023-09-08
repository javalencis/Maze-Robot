#define ENB 9
#define ENA 10

#define LM1 5
#define LM2 4
#define DM1 7
#define DM2 6

#define tCenter A3  
#define eCenter A2 
#define tRight  A5  
#define eRight  A4 
#define tLeft   A1  
#define eLeft   A0 

uint32_t leftSensor, oldLeftSensor=0;
uint32_t rightSensor,oldRightSensor=0;
uint32_t centerSensor,oldCenterSensor=0;

void setup() {
  Serial.begin(9600);
  initPins();

}

void loop() {
  readSensors();
  Serial.print("left: ");
  Serial.println(leftSensor);


}


void speed(int vel){
  analogWrite(ENB, vel);
  digitalWrite(LM1,1);
  digitalWrite(LM2,0);

}

void initPins(){
  for(int i = 4; i<=10;i++ ){
    pinMode(i,OUTPUT);
  }
  pinMode(A0,INPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,INPUT);
  pinMode(A3,OUTPUT);
  pinMode(A4,INPUT);
  pinMode(A5,OUTPUT);
}


uint32_t readSensor(uint8_t trigger, uint8_t echo){
  uint32_t t; //timepo que demora en llegar el eco
  uint32_t d; //distancia en centimetros

  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(trigger, LOW);
  
  t = pulseIn(echo, HIGH); //obtenemos el ancho del pulso
  d = t/59;             //escalamos el tiempo a una distancia en cm
  return d;     
}

void readSensors(){
  uint32_t lSensor = readSensor(tLeft, eLeft);
  uint32_t rSensor = readSensor(tRight, eRight);
  uint32_t cSensor = readSensor(tCenter, eCenter);

  leftSensor = (lSensor + oldLeftSensor)/2;
  rightSensor = (rSensor + oldRightSensor)/2;
  centerSensor = (cSensor + oldCenterSensor)/2;


  oldLeftSensor= leftSensor;
  oldRightSensor = rightSensor;
  oldCenterSensor = centerSensor;
}



