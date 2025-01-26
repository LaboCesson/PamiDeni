/*!
 * \file 	LibUltrason.h
 * \brief 	Le fichier de définition de la classe LibUltrason
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#ifndef _LIB_ULTRASO_H_
#define _LIB_ULTRASON_H_

#include "arduino.h"

/// \class LibUltrason
/// \brief Gestion d'un capteur Ultrason HC-SR04. 
/// <a href="https://www.gotronic.fr/pj2-hc-sr04-utilisation-avec-picaxe-1343.pdf">HC-SR04 documentation</a>
class LibUltrason
{
  public: 
    /// \details La distance maximum indiqué permet de limiter le temps d'analyse de la distance dans le cas
    ///          ou aucun obstacle n'est présent
    LibUltrason(
			unsigned char  pinTrig,    ///< Le numéro de la pin connectée à l'entrée Trig
      unsigned char  pinEcho,    ///< Le numéro de la pin connectée à l'entrée Echo
      unsigned short distanceMax ///< La distance maximum à mesurer en millimètre
    );
	
    /// \details Permet de valider l'utilisation du capteur à Ultrason
    void begin(void);
 
    /// \details Permet de changer la valeur maximum à mesurer.
    ///          Pour 1000mm le temps de mesure est de 5,8ms
    void setMaxDistance(
      unsigned short distanceMax ///< La distance maximum à mesurer en millimètre
    );

    /// \details Mesure la distance avec un objet
    /// \return Retourne la distance en millimetre (La distance maximum si aucun objet n'est détecté)
    /// \attention Pour une distance quasi nulle, la distance retournée est erronée et peut égaler la distance maximum
    unsigned short getDistance(void);

  private:
    bool          m_begin;
    unsigned char m_pinTrig;
    unsigned char m_pinEcho;
    unsigned long m_timeOut;

    unsigned long getPulseDuration(void);
    long newPulseIn(unsigned char pin, unsigned char state, unsigned long timeout);

  protected:
};


#endif