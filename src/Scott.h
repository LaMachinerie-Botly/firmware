#ifndef Scott_h
#define Scott_h
#define LIBRARY_VERSION	0.36

#define ORIGINAL 1 // Version de base de Scott
#define DIY 2 // Version DIY de Scott

#include "ScottSteppers.h"
/*****************************************************
 *      	        Constantes utiles                *
 *            Attention calculé seulement            *
 *   pour les roues et les moteurs du Scott v0.35)   *
 *****************************************************/
  
#define MM_TO_STEP 26.076  // 13.038 ?
#define RAD_TO_STEP 1210//6175
#define DELTA_ARC 47.5


/*********************
	 Dépendance
*********************/
#include <Servo.h>
#include <Arduino.h>

#include "ScottSteppers.h"

class Scott{
public:

  Servo crayon;

  Scott();
  Scott(int version); //Contructeur

  void init();

  void run();
  
  void stop(long temps);


  void gauche(long pas);

  void tournerGauche(long angleDegree);


  void droite(long pas);

  void tournerDroite(long angleDegree);


  void avant(long pas);

  void avancer(long distanceMillimeter);


  void arriere(long pas);

  void reculer(long distanceMillimeter);



  void setSpeed(float vitesse);
  void setSpeed(float vitesseD, float vitesseG);

  void logSpeed();
  

  void turnGo(float angle, int ligne);

  void turnGoDegree(float angle, int ligne);

  //void turnGo();



  void polygone(unsigned int nbrCote, unsigned int longueur);

  void rectangle(unsigned int largeur, unsigned int longueur);

  void carre(unsigned int longueur);

  void cercle(unsigned int diametre);

  void arc(float rayon,float angle);

  

  void leverCrayon();

  void poserCrayon();

  void bougerCrayon(int angle);

  unsigned int lectureDistance();

  unsigned int lectureLigne();

  unsigned int lectureLumiere();

  unsigned char lectureContact();



private:

  // Pin pour les moteurs pas-à-pas par défaut

  int _pinSwitchDroite = 4 ;
  int _pinSwitchGauche = 5 ;
  int _pinLigneDroite = A1 ;
  int _pinLigneGauche = A0 ;
  int _pinLumiereDroite = A7 ;
  int _pinLumiereGauche = A6 ;
  int _pinDistDroite = A2 ;
  int _pinDistGauche = A3 ;
  int _pinIrEmetteur = 2 ;
  int _pinServo = 3 ;

   ScottSteppers *Steppers;

  

  //float DEG_TO_STEP(DEG_TO_RAD*RAD_TO_STEP)

  unsigned int _distD = 0;
  unsigned int _distG = 0;

  int tpsEcoule = 0 ;
  int tpsTop = 0 ;

  //Variable capteur de distance
  int _distDroite;
  int _distGauche;
  
  //Cst crayon
  int _bas = -35;
  int _haut = 10;

  int servoAction = 0;

};
#endif
