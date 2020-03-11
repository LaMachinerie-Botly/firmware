#ifndef Wally_h
#define Wally_h

#include "Botly.h"
#include "Geometry.h"

const float w = 500, 
            paperGap = 200,
            paperSize = 500;

extern bool _originSet;

class Wally : public Botly{
public:
  Wally() : cartesian(GVector2D(0,0,0)){}

  void init();
  void origin();
  void stop         (long temps = 0);
  void avant        (long);
  void arriere      (long);
  void gauche       (long);
  void droite       (long);

  void avancer      (float);
  void reculer      (float);
  void tournerDroite(float);
  void tournerGauche(float);

  void execRequest  (int);
  void isIRDataReceived();

  void turnGoDegree (float angle, float ligne);
  void turnGo       (float angle, float ligne);
  
  
protected :
  GVector2D cartesian, polar;
};
#endif
