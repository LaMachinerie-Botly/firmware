#include <Botly.h>

Botly robot;

void setup(){
    robot.init();
}

void loop(){
    polygone(4, 100);
}

void polygone(int nbCote, int longueurCote){
    for (int i = 0; i < nbCote; i++){
        robot.tournerGauche(360/nbCote);
        robot.avancer(longueurCote);
    }
}