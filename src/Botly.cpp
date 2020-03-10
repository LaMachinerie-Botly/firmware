#include "Botly.h"

Botly::Botly(){
	Steppers = new BotlySteppers();
}


void Botly::init()
{

	Serial.begin(9600);
	initIRcom();
	analogReference(INTERNAL); //reference analogique 2.56V

	crayon.attach(_pinBotlyServo);
	crayon.write(_botlyHaut);

	pinMode(_pinBotlyIrEmetteur, OUTPUT);
	digitalWrite(_pinBotlyIrEmetteur, LOW);

	getCalibration();
	_deltaArc = BOTLY_DELTA_ARC;

	// Detecte si une nouvelle version du code uploadé
	// Atention : ne fonctionne pas encore.
	// Utilisé uniquement à la recompilation de la library
	if(__TIME__[0] == '?') _buildSec = 99;
	else _buildSec = ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0') ;

   if (EEPROM.read(_timeAddress)!=_buildSec)
   {
		 EEPROM.update(_timeAddress, _buildSec);
		 musicNewProgramm();
   }
	else musicBegin();

	Steppers->setMaxSpeed(900.0);
	Steppers->setSpeed(300.0);
	Steppers->enable();

}

void Botly::run(){
  Steppers->run();
}

void Botly::setCalibration(int distance, int rotation){

	// Mise à jour des variables de calibration
	_mmToStep  = distance;
	_radToStep = rotation;

	// Sauvegarde des paramètres dans l'EEPROM
	EEPROM.update(_distanceAddress, distance >> 8);
	EEPROM.update(_distanceAddress+1, distance & 255);

	EEPROM.update(_rotationAddress, rotation >> 8);
	EEPROM.update(_rotationAddress+1, rotation & 255);

	// Sauvegarde du changement de la calibration
	EEPROM.update(_checkNewAddress, NEW_CALIBRATION);
}

void Botly::getCalibration(){

	// Si une nouvelle valeur a été chargée dans le robot prendre ces valeurs
	if (EEPROM.read(_checkNewAddress) == NEW_CALIBRATION)
	{
		_mmToStep  = EEPROM.read(_distanceAddress) << 8 | EEPROM.read(_distanceAddress+1);
		_radToStep = EEPROM.read(_rotationAddress) << 8 | EEPROM.read(_rotationAddress+1);
	}
	else
	{
		_mmToStep  = BOTLY_MM_TO_STEP 	;
		_radToStep = BOTLY_RAD_TO_STEP 	;
	}
}

void Botly::factoryCalibration(){
	// revenir à la calibration usine
	EEPROM.update(_checkNewAddress, 0);
	_mmToStep  = BOTLY_MM_TO_STEP 	;
	_radToStep = BOTLY_RAD_TO_STEP 	;
}

void Botly::setSpeed(float vitesse){
	Steppers->setSpeed(vitesse);
}

void Botly::setSpeed( float vitesseD, float vitesseG){
	Steppers->setSpeed(vitesseD, vitesseG);
}

void Botly::logSpeed(){
	Serial.print("Vitesse : "); Serial.print(Steppers->getSpeed(0)); Serial.print(" | "); Serial.println(Steppers->getSpeed(1));
	Serial.print("Vitesse max : "); Serial.print(Steppers->getMaxSpeed(0)); Serial.print(" | "); Serial.println(Steppers->getMaxSpeed(1));
}


void Botly::turnGoDegree(float angle, long ligne){
  angle = angle * DEG_TO_RAD ; // Passage en radians
  turnGo(angle, ligne);
}

void Botly::turnGo(float angle, long ligne){

  if(angle > 0 && angle < PI){
    gauche( int( (angle * _radToStep)) );
  }
  else if( angle >= PI ){
	  droite(int( ( (angle-PI) * _radToStep)) );
  }
  else if( angle < 0 ){
    droite(int( -( angle * _radToStep)) );
  }
  else{
    stop(100);
  }

  if( ligne > 0 ){
    avant( (ligne * _mmToStep)/10 );
  }
  else if( ligne < 0 ){
    arriere( -( ligne * _mmToStep)/10 );
  }
  else{
    stop(100);
  }
}

void Botly::musicBegin(){

	delay(500);
    tone(_pinBuzzer, 1364, 100);
	delay(110);
	tone(_pinBuzzer, 1535, 250);
	delay(300);
	tone(_pinBuzzer, 2060, 500);
	delay(500);

}

void Botly::musicNewProgramm(){

  tone(_pinBuzzer, 1364, 500);
  delay(400);
  tone(_pinBuzzer, 1364, 80);
  delay(100);
  tone(_pinBuzzer, 1364, 80);
  delay(100);
  tone(_pinBuzzer, 2060, 800);
  delay(800);
}

void Botly::musicEnd(){

	delay(500);
  tone(_pinBuzzer, 1976-33, 500);
	delay(500);
	tone(_pinBuzzer, 1480-33, 100);
	delay(110);
	tone(_pinBuzzer, 1568-33, 800);
	delay(800);
}

void Botly::avant(long pas){
	Steppers->moveTo(-pas, pas);
	Steppers->runSpeedToPosition(); //Blockling...
	Steppers->setPositions();
}

void Botly::arriere(long pas){
	Steppers->moveTo(pas, -pas);
	Steppers->runSpeedToPosition();//Blockling...
	Steppers->setPositions();
}

void Botly::gauche(long pas){
	Steppers->moveTo(-pas, -pas);
	Steppers->runSpeedToPosition();//Blockling...
	Steppers->setPositions();

}

void Botly::droite(long pas){
	Steppers->moveTo(pas, pas);
	Steppers->runSpeedToPosition();//Blockling...
	Steppers->setPositions();
}


//Battery Power save !!!!
void Botly::stop(long temps){
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

//Battery Power save !!!!
void Botly::stop(){
	Steppers->disable();
	while(true);
}

void Botly::tournerGauche(long angleDegree){
	gauche(long((angleDegree * DEG_TO_RAD * _radToStep)));
}

void Botly::tournerDroite(long angleDegree){
	droite(long((angleDegree * DEG_TO_RAD *_radToStep)));
}

void Botly::avancer(long distanceMillimeter){
	turnGoDegree(0, distanceMillimeter);
}

void Botly::reculer(long distanceMillimeter){
	turnGoDegree(0, -distanceMillimeter);
}

void Botly::polygone(unsigned int nbrCote, unsigned int longueur, unsigned int direction){
	if (nbrCote >=3)
	{
		float polyAngle = 360 / nbrCote;
		if(direction != DIR_RIGHT) polyAngle = -polyAngle;
		turnGoDegree(0,longueur);
		for (unsigned int i=1 ; i<nbrCote ; i++)
		{
			turnGoDegree(polyAngle,longueur);
		}
		turnGoDegree(polyAngle,0);
	}
}

void Botly::rectangle(unsigned int largeur, unsigned int longueur){
	turnGoDegree(0,largeur);
	turnGoDegree(90,longueur);
	turnGoDegree(90,largeur);
	turnGoDegree(90,longueur);
	turnGoDegree(90,0);
}

void Botly::carre(unsigned int longueur){
	rectangle(longueur,longueur);
}

void Botly::cercle(unsigned int diametre, unsigned int direction){
	float angleCercle = 11 * DEG_TO_RAD ; // Passage en radians
	unsigned int corde = (diametre * angleCercle )/2 ;
	polygone(36,corde * 1.25, direction); // 33=arrondi de 360/11
}

void Botly::arc( float rayon,float angle){
	int pasD, pasG;
	if(angle > 0){
		pasD = ((rayon - _deltaArc) * angle*DEG_TO_RAD) * (_mmToStep/10);
		pasG = ((rayon + _deltaArc) * angle*DEG_TO_RAD) * (_mmToStep/10);
	}else{
		pasG = ((rayon - _deltaArc) * angle*DEG_TO_RAD) * (_mmToStep/10);
		pasD = ((rayon + _deltaArc) * angle*DEG_TO_RAD) * (_mmToStep/10);
	}
	Steppers->moveTo(pasD, pasG);
}

void Botly::leverCrayon(){
	crayon.write(_botlyHaut);
}

void Botly::poserCrayon(){
	crayon.write(_botlyBas);
}

void Botly::bougerCrayon(int angle)
{
	crayon.write(angle);
}

void Botly::finProgramme()
{
	stop();
	musicEnd();
	while(true);
}

void Botly::isIRDataReceived(){
	if (irrecv.decode(&results)) {
		Serial.println(results.value, HEX);
		irrecv.resume(); // Receive the next value
    }
}

void Botly::initIRcom(){
	irrecv.enableIRIn(); // Start the receiver
}

void Botly::sonyCode(byte data){
	irsend.sendSony(data, 8);
}


bool Botly::proximite(int ite, int trigger)
{
	int validDetection = 0;
	trigger = (trigger > ite) ? ite : trigger ;

	for (int k = 0; k<= ite; k++)
	{
		delay(20);						// Attendre avant une lecture

		// Generation des pulsation à 38kHz
		/* Le temps à l'etat haut est diminué afin de limiter
		la portée de la detection du capteur */

		for(int i = 0; i <= 31; i++)
		{
			digitalWrite(_pinBotlyIrEmetteur, HIGH);
			delayMicroseconds(8);
			digitalWrite(_pinBotlyIrEmetteur, LOW);
			delayMicroseconds(13);
			if(digitalRead(_pinTsop)==LOW)
			{
				validDetection++;
				break;
			}
		}
	}
  return (validDetection>=trigger);
}

/*Cette fonction mesure la valeur analogique
La référence interne est de 2.56V peu importe la tension
d'alimentation du controleur
Mesure sur 1024 valeurs (10bits)
Méthode de recalcul: (2.56/1024)*mesureAnalogique
Ne pas oublier le rapport de transformation du pont diviseur
en hardware
*/
int Botly::mesureBatterie()
{
	int mesureAnalogique=analogRead(_pinMesureBatterie);
	return mesureAnalogique;
}

void Botly::sleepNow()
{
	/* In the Atmega32u4 datasheet on page 62
	 * there is a list of sleep modes which explains which clocks and
	 * wake up sources are available for each sleep mode.
	 *
	 * In the avr/sleep.h file, the call names of these sleep modus are to be found:
	 *
	 * The 5 different modes are:
	 * SLEEP_MODE_IDLE -the least power savings, wake up on usart
	 * SLEEP_MODE_ADC
	 * SLEEP_MODE_PWR_SAVE
	 * SLEEP_MODE_STANDBY
	 * SLEEP_MODE_PWR_DOWN -the most power savings
	 */
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
	sleep_enable(); // enables the sleep in mcucr register,safety bit
	power_all_disable(); //low power consumption
	sleep_mode(); // here the device is actually put to sleep!!
	// THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
	sleep_disable(); //disable sleep, safety bit
	power_all_enable(); //restore power on peripherals
}

void Botly::sleepWakeup()
{
	/* In the Atmega32u4 datasheet on page 62
	 * there is a list of sleep modes which explains which clocks and
	 * wake up sources are available for each sleep mode.
	 *
	 * In the avr/sleep.h file, the call names of these sleep modus are to be found:
	 *
	 * The 5 different modes are:
	 * SLEEP_MODE_IDLE -the least power savings, wake up on usart
	 * SLEEP_MODE_ADC
	 * SLEEP_MODE_PWR_SAVE
	 * SLEEP_MODE_STANDBY
	 * SLEEP_MODE_PWR_DOWN -the most power savings
	 */

	attachInterrupt(0, pin2_isr,LOW );//wake up on interrupt pin 2 or 3
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode
	cli(); //clear interrupts
	sleep_enable(); // enables sleep bit in mcucr register, safety pin
	sei();//to allow wake up by interrupt
	//power_adc_disable();
	//power_usart0_disable();
	//power_spi_disable();
	//power_twi_disable();
	//power_timer0_disable();
	//power_timer1_disable();
	//power_timer2_disable();
	//power_timer3_disable();
	//power_usart1_disable();
	//power_usb_disable();

	power_all_disable(); //low power consumption
	sleep_mode(); // here the device is actually put to sleep!!
	// THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
	sleep_disable(); //disable sleep,safety bit
	power_all_enable(); //restore power on peripherals
}


// !!!! Pas utilisé pour le moment
//external interrupt routine to wake up controller
// void pin2_isr()
// {
// 	if (_version==SCOTT_V4) return; // annule la fonction si mauvaise version
//   sleep_disable();
//   detachInterrupt(0);
// }
