#include <Arduino.h>
#include "Scott.h"


Scott::Scott(){
	Scott(DEFAULT);
}

Scott::Scott(int version){
	Steppers = new ScottSteppers();
}

void Scott::init(){
  tpsTop = millis();
  crayon.attach(_pinServo);
  crayon.write(_haut);
  
  Steppers->setMaxSpeed(900.0);
  Steppers->setSpeed(300.0);
  Steppers->enable();
}

void Scott::run(){
  Steppers->run();
}


void Scott::setSpeed(float vitesse){
	Steppers->setSpeed(vitesse);
}

void Scott::setSpeed( float vitesseD, float vitesseG){
	Steppers->setSpeed(vitesseD, vitesseG);
}

void Scott::logSpeed(){
	Serial.print("Vitesse : "); Serial.print(Steppers->getSpeed(0)); Serial.print(" | "); Serial.println(Steppers->getSpeed(1));
	Serial.print("Vitesse max : "); Serial.print(Steppers->getMaxSpeed(0)); Serial.print(" | "); Serial.println(Steppers->getMaxSpeed(1));
}


void Scott::turnGoDegree(float angle, int ligne){
  angle = angle * DEG_TO_RAD ; // Passage en radians
  turnGo(angle, ligne);
}

void Scott::turnGo(float angle, int ligne){

  if(angle > 0 && angle < PI){
    gauche( int( (angle * RAD_TO_STEP)) );
  }
  else if( angle >= PI ){
	  droite(int( ( (angle-PI) * RAD_TO_STEP)) );
  }
  else if( angle < 0 ){
    droite(int( -( angle * RAD_TO_STEP)) );
  }
  else{
    stop(100);
  }

  if( ligne > 0 ){
    avant( ligne * MM_TO_STEP );
  }
  else if( ligne < 0 ){
    arriere( -( ligne * MM_TO_STEP) );
  }
  else{
    stop(100);
  }
}


void Scott::avant(long pas){
	Steppers->moveTo(-pas, pas);
	Steppers->runSpeedToPosition(); //Blockling...
	Steppers->setPositions();
}

void Scott::arriere(long pas){
	Steppers->moveTo(pas, -pas);
	Steppers->runSpeedToPosition();//Blockling...
	Steppers->setPositions();
}

void Scott::gauche(long pas){
	Steppers->moveTo(-pas, -pas);
	Steppers->runSpeedToPosition();//Blockling...
	Steppers->setPositions();
}

void Scott::droite(long pas){
	Steppers->moveTo(pas, pas);
	Steppers->runSpeedToPosition();//Blockling...
	Steppers->setPositions();
}


//Battery Power save !!!!
void Scott::stop(long temps){
	if(temps > 40){
		delay(20);
		Steppers->disable();
		delay(temps-40);
		Steppers->enable();
		delay(20);
	}else{
		delay(temps);
	}
}




void Scott::tournerGauche(long angleDegree){
	gauche(long((angleDegree * DEG_TO_RAD * RAD_TO_STEP)));
}

void Scott::tournerDroite(long angleDegree){
	droite(long((angleDegree * DEG_TO_RAD *RAD_TO_STEP)));
}

void Scott::avancer(long distanceMillimeter){
	turnGoDegree(0, distanceMillimeter);
}

void Scott::reculer(long distanceMillimeter){
	turnGoDegree(0, -distanceMillimeter);
}

void Scott::polygone(unsigned int nbrCote, unsigned int longueur){
	if (nbrCote >=3)
	{
		float polyAngle = 360 / nbrCote;
		turnGoDegree(0,longueur);
		for (unsigned int i=1 ; i<nbrCote ; i++)
		{
			turnGoDegree(polyAngle,longueur);
		}
		turnGoDegree(polyAngle,0);
	}
}

void Scott::rectangle(unsigned int largeur, unsigned int longueur){
	turnGoDegree(0,largeur);
	turnGoDegree(90,longueur);
	turnGoDegree(90,largeur);
	turnGoDegree(90,longueur);
	turnGoDegree(90,0);
}

void Scott::carre(unsigned int longueur){
	rectangle(longueur,longueur);
}

void Scott::cercle(unsigned int diametre){
	float angleCercle = 11 * DEG_TO_RAD ; // Passage en radians
	unsigned int corde = (diametre * angleCercle )/2 ;
	polygone(32,corde * 1.25); // 33=arrondi de 360/11
}

void Scott::arc( float rayon,float angle){
	int pasD, pasG;
	if(angle > 0){
		pasD = ((rayon - DELTA_ARC) * angle*DEG_TO_RAD) * MM_TO_STEP;
		pasG = ((rayon + DELTA_ARC) * angle*DEG_TO_RAD) * MM_TO_STEP;
	}else{
		pasG = ((rayon - DELTA_ARC) * angle*DEG_TO_RAD) * MM_TO_STEP;
		pasD = ((rayon + DELTA_ARC) * angle*DEG_TO_RAD) * MM_TO_STEP;
	}
	Steppers->moveTo(pasD, pasG);
}

unsigned char Scott::lectureContact(){
	return (!digitalRead(_pinSwitchDroite) + 2*(!digitalRead(_pinSwitchGauche)));
	//  Gauche  |  Droit  ||  Resultat
	//----------|---------||----------
	//    0     +    0    ||     0
	//    0     +    1    ||     1
	// 2 (2*1)  +    0    ||     2
	// 2 (2*1)  +    1    ||     3
}

unsigned int Scott::lectureLumiere(){
	delayMicroseconds(180);

	unsigned int _LumiereDroite = analogRead(_pinLumiereDroite);
	unsigned int _LumiereGauche = analogRead(_pinLumiereGauche);

	return (_LumiereDroite*100)/(_LumiereGauche + _LumiereDroite);

}

unsigned int Scott::lectureDistance(){
	digitalWrite(_pinIrEmetteur,HIGH);
	delayMicroseconds(180);

	_distDroite = analogRead(_pinDistDroite);
	_distGauche = analogRead(_pinDistGauche);

	digitalWrite(_pinIrEmetteur,LOW);
	delayMicroseconds(180);
	_distDroite -= analogRead(_pinDistDroite);
	_distGauche -= analogRead(_pinDistGauche);

	return (_distDroite*100)/(_distGauche + _distDroite);
}

unsigned int Scott::lectureLigne(){
	digitalWrite(_pinIrEmetteur,HIGH);
	delayMicroseconds(180);
	unsigned int _irDroit = analogRead(_pinLigneDroite);
	unsigned int _irGauche = analogRead(_pinLigneGauche);

	digitalWrite(_pinIrEmetteur,LOW);
	delayMicroseconds(180);
	_irDroit -= analogRead(_pinLigneDroite);
	_irGauche -= analogRead(_pinLigneGauche);

	return (_irDroit*100)/(_irGauche + _irDroit);
}

void Scott::leverCrayon(){
	crayon.write(_haut);
}

void Scott::poserCrayon(){
	crayon.write(_bas);
}

void Scott::bougerCrayon(int angle){
	crayon.write(angle);
}


