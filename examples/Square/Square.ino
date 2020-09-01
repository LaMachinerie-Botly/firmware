#include <Botly.h>

Botly robot;

void setup(){
    robot.init();
}

void loop(){
    robot.avancer(10); //Move forward by 10 mm
    robot.tournerGauche(90); //Turn left by 90°
}