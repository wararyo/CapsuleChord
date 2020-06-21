#ifndef _MODIFIER_H_
#define _MODIFIER_H_

#include "Chord.h"

void thirdInvert(Chord *c){
  if(c->option & Chord::Minor) c->option &= ~(Chord::Minor);
  else c->option |= Chord::Minor;
}
void fifthFlat(Chord *c){
  c->option |= Chord::FifthFlat;
}
void augment(Chord *c){
  c->option |= Chord::Aug;
}
void sus4(Chord *c){
  c->option |= Chord::Sus4;
}
void seventhInvert(Chord *c){
  if(c->option & Chord::Seventh) {c->option &= ~(Chord::Seventh); c->option |= Chord::MajorSeventh;}
  else if(c->option & Chord::MajorSeventh) {c->option &= ~(Chord::MajorSeventh); c->option |= Chord::Seventh;}
}
void ninth(Chord *c){
  c->option |= Chord::Ninth;
}
void thirteenth(Chord *c){
  c->option |= Chord::Thirteenth;
}

#endif