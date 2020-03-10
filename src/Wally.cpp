#include "Wally.h"

/*********************************
 * 			Kinematics			 *
 *********************************/  
  
GVector2z forward(GVector2z polar){
  float La = polar.x;
  float Lb = polar.y;
  
  //Theorical
  //float alpha = acos((La*La+4*w*w-(Lb*Lb))/(2*La*2*-w));
  float beta = acos((Lb*Lb+4*w*w-(La*La))/(2*Lb*2*w));
  
  //Real
  return new GVector2z(Lb * cos(beta) - w, Lb * sin(beta), polar.z);
}

GVector2z backward(GVector2z pos){
 float x,y;
 
 x = pos.x;
 y = pos.y;
 
 //Theorical
 float alpha = atan(y / (w-x)); 
 float beta  = atan(y / (w+x)); 
 float dLa = (y / sin(alpha));
 float dLb = (y / sin(beta ));

 return new GVector2z(dLa, dLb, pos.z);
}

void Wally::origin(){
    polar.x = polar.y = 0;
    cartesian.z = polar.z = PI/2;
    cartesian.x = 0;
    cartesian.y = paperGap + paperSize/2;
    polar = backward(cartesian);
}

void Wally::avancer(long distance){
  turnGoDegree(0, distanceMillimeter);
}

void Wally::reculer(long distance){
  turnGoDegree(0, -distanceMillimeter);
}

void Wally::tournerDroite(float angleDegree){
  angle = GMath.degToRad * angle;
  polar.z -= angle;
  if(polar.z >= 2*PI){
    polar.z -= 2*PI;
  }
  if(polar.z <= -2*PI){
    polar.z += 2*PI;
  }
  
  cartesian.z = polar.z;
}

void Wally::tournerGauche(float angleDegree){
  angle = GMath.degToRad * angle;
  polar.z += angle;
    if(polar.z >= PI){
    polar.z -= 2*PI;
  }
  if(polar.z <= -PI){
    polar.z += 2*PI;
  }
  
  cartesian.z = polar.z;
}

void Wally::turnGoDegree(float angle, long ligne){
  angle = angle * DEG_TO_RAD ; // Passage en radians
  turnGo(angle, ligne);
}

void Wally::turnGo(float angle, long ligne){

  if(angle > 0 && angle < PI){
    tournerGauche( int( (angle * _radToStep)) );
  }
  else if( angle >= PI ){
	  tournerDroite(int( ( (angle-PI) * _radToStep)) );
  }
  else if( angle < 0 ){
    tournerDroite(int( -( angle * _radToStep)) );
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


void Wally::avant(long pas){
  //ABS
  cartesian.x -= cos(polar.z)*distance;
  cartesian.y += sin(polar.z)*distance;
  GVector2z temp = polar;
  polar = backward(cartesian);

  temp = polar - temp;

	Steppers->moveTo(-temp.x, temp.y);
	Steppers->runSpeedToPosition();//Blockling...
	Steppers->setPositions();
}

void Wally::arriere(long pas){
  //ABS
  cartesian.x -= cos(polar.z)*distance;
  cartesian.y += sin(polar.z)*distance;
  GVector2z temp = polar;
  polar = backward(cartesian);

  temp = polar - temp;

	Steppers->moveTo(temp.x, -temp.y);
	Steppers->runSpeedToPosition();//Blockling...
	Steppers->setPositions();
}

void Wally::gauche(long pas){
//Nothing to see here
}

void Wally::droite(long pas){
//Nothing to see here
}

//Battery Power save !!!!
void Wally::stop(long temps){
  delay(temps);
}