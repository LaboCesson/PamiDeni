/*!
 * \file 	LibUltrason.cpp
 * \brief 	Le fichier source de la classe LibUltrason
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibUltrason.h"



LibUltrason::LibUltrason(unsigned char pinTrig, unsigned char pinEcho, unsigned short distanceMax) {
  pinMode(m_pinTrig = pinTrig, OUTPUT);
  pinMode(m_pinEcho = pinEcho, INPUT);
  setMaxDistance(distanceMax); // On transforme la distance en une durée
  m_begin = false;
}


void LibUltrason::begin(void) { m_begin = true; }


void LibUltrason::setMaxDistance(unsigned short distanceMax) {
  // On traduit le timeout en multipliant par 5.8
  if(distanceMax < 500) distanceMax = 500;
  m_timeOut = distanceMax;
  m_timeOut *=58;
  m_timeOut /=10;
}


unsigned short LibUltrason::getDistance(void) {
  if(m_begin == false ) return 0;
  
  // On mesure le temps de réponse que l'on divise par 5.8
  unsigned long distance = getPulseDuration();
  distance *= 10;
  distance /= 58;

  return (unsigned short) distance;
}

unsigned long LibUltrason::getPulseDuration() {
  // cache the port and bit of the pin in order to speed up the
  // pulse width measuring loop and achieve finer resolution.  
  // calling digitalRead() instead yields much coarser resolution.
  uint8_t bit = digitalPinToBitMask(m_pinEcho);
  uint8_t port = digitalPinToPort(m_pinEcho);

  unsigned long timeStart;

  // wait for any previous pulse to end
  timeStart = micros();
  while ((*portInputRegister(port) & bit) != 0) {
    if((micros()-timeStart)>m_timeOut) return m_timeOut;
  }

  // On genère un pulse de 10µs sur la pin de Trigger
  digitalWrite(m_pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(m_pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(m_pinTrig, LOW);

  // wait for the pulse to start
  while ((*portInputRegister(port) & bit) == 0) {
    if((micros()-timeStart)>m_timeOut) return m_timeOut;
  }

  // Now measure the echo pulse
  timeStart = micros(); // Reinit the time reference
  while ((*portInputRegister(port) & bit) != 0) {
    if((micros()-timeStart)>m_timeOut) return m_timeOut;
  }

  return micros()-timeStart;
}
