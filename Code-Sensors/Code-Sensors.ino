#include <NewPing.h>

#define enB 9
#define enA 10

#define lm1 5
#define lm2 4
#define rm1 7
#define rm2 6
#define offsetEnA 12

#define tCenter   A3 // Arduino pin tied to trigger pin on ping sensor.
#define eCenter   A2 // Arduino pin tied to echo pin on ping sensor.
#define tRight A5
#define eRight A4
#define tLeft A1
#define eLeft A0

#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.



uint16_t leftSensor, oldLeftSensor = 0;
uint16_t rightSensor, oldRightSensor = 0;
uint16_t centerSensor, oldCenterSensor = 0;
uint8_t maxLimitFront = 15;       //Limite maximo de distancia del sensor frontal
bool turnRight = false;   //Doblar derecha

NewPing sonar[3] = {   // Sensor object array.
  NewPing(tCenter, eCenter, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(tRight, eRight, MAX_DISTANCE),
  NewPing(tLeft, eLeft, MAX_DISTANCE)
};

unsigned int pingSpeed = 50; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;     // Holds the next ping time.

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  pingTimer = millis(); // Start now.
}

void loop() {
  // Notice how there's no delays in this sketch to allow you to do other processing in-line while doing distance pings.
  if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;      // Set the next ping time.
    sonar[0].ping_timer(echoCheck); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }
  // Do other stuff here, really. Think of it as multi-tasking.

  if(turnRight){
    fTurnRight();
  }
}

void echoCheck() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  if (sonar[0].check_timer()) { // This is how you check to see if the ping was received.
    // Here's where you can add code.
    centerSensor = sonar[0].ping_result / US_ROUNDTRIP_CM;
    Serial.print("Ping: ");
    Serial.print(centerSensor); // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
    Serial.println("cm");

    if (  (sonar[0].ping_result / US_ROUNDTRIP_CM) < maxLimitFront) {
      stop();
      turnRight = true;
    } else {
      run();
    }

  }
  // Don't do anything here!
}

//funcion que me permite doblar hacia la derecha
void fTurnRight(){
  //Controlar por medio del sensor del centro hasta cuando doblar hacia la derecha
  while(centerSensor < 12){
    analogWrite(enA, 200 + offsetEnA);
    analogWrite(enB, 200);
    digitalWrite(rm1, 0);
    digitalWrite(rm2, 1);
    digitalWrite(lm1, 1);
    digitalWrite(lm2, 0);
    delay(300);
    centerSensor = sonar[0].ping_cm();      //Para medir la distancia actual 

  }

  stop();
  delay(100);
  turnRight = false;

  
}


void run() {
  analogWrite(enA, 200 + offsetEnA);
  analogWrite(enB, 200);
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