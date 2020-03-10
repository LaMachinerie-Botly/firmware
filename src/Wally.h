#ifndef Wally_h
#define Wally_h

#include "Botly.h"
#include "Geometry.h"

class Wally : public Botly{
  Wally(){}

  void origin();
  void avant(long pas);
  void arriere(long pas);
  void gauche(long pas);
  void droite(long pas);

  void avancer(float distance);
  void reculer(float distance);
  void tournerDroite(float angle);
  void tournerGauche(float angle);

  void Botly::turnGoDegree(float angle, long ligne);
  void Botly::turnGo(float angle, long ligne)
  
protected :
  GVector2z cartesian, polar;
};
#endif
