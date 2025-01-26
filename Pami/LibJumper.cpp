/*!
 * \file 	LibJumper.cpp
 * \brief 	Le fichier source de la classe LibJumper
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibJumper.h"


LibJumper::LibJumper(unsigned char pinJumperTeam, unsigned char pinJumperNbH, unsigned char pinJumperNbL) {
  pinMode(m_pinJumperTeam = pinJumperTeam, INPUT_PULLUP);  
  pinMode(m_pinJumperNbH  = pinJumperNbH,  INPUT);
  pinMode(m_pinJumperNbL  = pinJumperNbL,  INPUT);
}


t_pamiTeam LibJumper::getTeam(void) {
  return( ( digitalRead(m_pinJumperTeam) == 0 ? PAMI_TEAM_B : PAMI_TEAM_A ) ) ;
}


t_pamiNumber LibJumper::getPami(void) {
  if( analogRead(m_pinJumperNbH) > 100 ) {
    return( ( analogRead(m_pinJumperNbL) > 100 ? PAMI_NB_1 : PAMI_NB_2 ) ) ;
  } else {
    return( ( analogRead(m_pinJumperNbL) > 100 ? PAMI_NB_3 : PAMI_SING ) ) ;
  }
}
