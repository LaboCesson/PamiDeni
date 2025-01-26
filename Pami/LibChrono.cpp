/*!
 * \file 	LibChrono.cpp
 * \brief 	Le fichier source de la classe LibChrono
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibChrono.h"

LibChrono::LibChrono() {
  m_display_status = false;
  m_status_chrono  = false;
  m_status_alarm   = false;
  m_mode   = CHRONO_MODE_COMPTEUR;
  m_value  = 0;
  m_alarm  = 0;
  mp_display = NULL;
  m_lastValue = 0;
  m_nextTimeInc = 0;
  m_begin = false;
}

void LibChrono::begin(void) { m_begin = true; }

void LibChrono::setMode(t_ChronoMode mode) { m_mode = mode; }

void LibChrono::setAlarme(unsigned short alarmTime) {
  m_alarm = alarmTime; 
  m_status_alarm = false;
}

void LibChrono::start( unsigned short value) {
  m_status_chrono = true; 
  m_nextTimeInc = millis() + 1000; 
  m_value = value;
  displayChrono();
}

void LibChrono::stop(void) { m_status_chrono = false; }

bool LibChrono::isAlarm(void) { return m_status_alarm; }

void LibChrono::resetAlarm(void) { m_status_alarm = false;  }

unsigned int LibChrono::getValue() { return m_value; }

void LibChrono::setDisplay(LibAff1637 * p_display) { mp_display = p_display; }

void LibChrono::display(bool status) { m_display_status = status; }


void LibChrono::gestion(void) {
  if(m_begin == false ) return;

  unsigned long newTime = millis();

  while( (newTime > m_nextTimeInc) && m_status_chrono == true ) {
	if( m_mode == CHRONO_MODE_COMPTEUR ) {
		m_value++;
		if( m_value >= m_alarm ) {
		  m_status_alarm  = true;
		  m_status_chrono = false;
		}
	}
	if( m_mode == CHRONO_MODE_DECOMPTE ) {
		m_value--;
		if( m_value <= m_alarm ) {
		  m_status_alarm  = true;
		  m_status_chrono = false;
		}
	}
	m_nextTimeInc += 1000;
  }
  if( m_value != m_lastValue ) displayChrono();
}


void LibChrono::displayChrono(void) {
  if(m_begin == false ) return;
  if( m_display_status == false ) return;
  if( mp_display == NULL) return;
  mp_display->displayValue(m_value);
  m_lastValue = m_value;
}
