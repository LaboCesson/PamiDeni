/*!
 * \file 	Lib433Mhz.cpp
 * \brief 	Le fichier source de la classe Lib433Mhz
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "Lib433Mhz.h"


Lib433Mhz::Lib433Mhz(t_modeRadio modeRadio, unsigned short speed, unsigned char pin, char * p_pattern) {
  m_modeRadio = modeRadio;
  m_speed     = speed;
  m_pin       = pin;
  mp_pattern  = p_pattern;
  m_patternSize = strlen(p_pattern);
  m_begin = false;
}


void Lib433Mhz::begin(unsigned char msgSize) {
  m_msgSize = msgSize;
  switch(m_modeRadio) {
    case RADIO433MHZ_RECV :
      mp_radio = new RH_ASK(m_speed,m_pin,0,0);
      break;
    case RADIO433MHZ_SEND :
      mp_radio = new RH_ASK(m_speed,0,m_pin,0);
      break;
    default : return;
  }
  mp_radio->init();
  mp_buf = malloc(m_patternSize+m_msgSize);
  m_begin = true;
}


unsigned char Lib433Mhz::getMessage(char * p_msg, unsigned char length) {
  if( !m_begin ) return 0;

  unsigned char buflen = m_patternSize+m_msgSize;

  if( !mp_radio->recv(mp_buf,&buflen) ) return 0;

  mp_buf[buflen]='\0';
  if( buflen < m_patternSize ) return 0;
  if( memcmp(mp_buf,mp_pattern,m_patternSize) != 0 ) return 0;
  strcpy(p_msg,mp_buf+m_patternSize);
  return strlen(p_msg);
}


void Lib433Mhz::sendMessage( char * p_msg ) {
  if( !m_begin )return 0;
  strcpy(mp_buf,mp_pattern);
  strcat(mp_buf,p_msg);
  Serial.println(mp_buf);
  mp_radio->send(mp_buf, strlen(mp_buf));
  mp_radio->waitPacketSent();
}
