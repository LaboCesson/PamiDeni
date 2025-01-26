/*!
 * \file 	Lib433Mhz.h
 * \brief 	Le fichier de définition de la classe Lib433Mhz
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#ifndef _LIB_433MHZ_H_
#define _LIB_433MHZ_H_

#include "arduino.h"
#include "RH_ASK.h"


typedef enum {
  RADIO433MHZ_RECV = 0, ///< Radio 433Mh en mode réception
  RADIO433MHZ_SEND = 1  ///< Radio 433Mh en mode émetteur
} t_modeRadio;


/// \class Lib433Mhz
/// \brief Gestion des d'un récepteur 433Mhz
/// \details * <a href="http://www.eeant.com/datasheet/et-rxb-12.pdf"> Documentation du Récepteur RXB-12 0 </a>
/// \details * <a href="https://dl.keyhanelc.com/B6/A307.pdf"> HowTo 1 </a>
/// \details * <a href="https://content.instructables.com/FUT/M44H/KYR9VTKC/FUTM44HKYR9VTKC.pdf"> HowTo 2 </a>
/// \details La librairie a utilisée est 
/// <a href="https://www.airspayce.com/mikem/arduino/RadioHead/RadioHead-1.142.zip"> La librairie RadioHead </a>
/// \attention Il est nécessaire de modifier le fichier RH_ASK.cpp et decommenter la ligne
/// #define RH_ASK_ARDUINO_USE_TIMER2
/// pour éviter les incompatibilité avec la librairie 'servo'
class Lib433Mhz
{
  public:
    Lib433Mhz(
      t_modeRadio     modeRadio, ///< Indique si l'on est en mode émission ou reception
      unsigned short  speed,     ///< Vitesse de transmission
      unsigned char   pin,       ///< Numéro du pin associé au récepteur ou au transmetteur
               char * p_pattern  ///< Pointeur vers le pattern permettant de filtrer les messages (doit être terminé par un 0)
    );

    /// \details Permet d'initialiser et de valider le module radio
    void begin(
      unsigned char msgSize ///< Taille maximum des messages échangées
    );

    /// \details Permet de lire un message reçu par radio
    /// \return Retourne la longueur du message lu
    unsigned char getMessage(
               char * p_msg, ///< Pointeur vers ou écrire le message lu
      unsigned char length   ///< longueur maximum du message à retourner
    );

    /// \details Permet de'envoyer un message par radio
    void sendMessage(
      char * p_msg ///< Pointeur vers ou écrire le message lu (doit se terminer par un 0)
    );
 
  private:
    bool m_begin;
    unsigned short m_speed;
    unsigned char  m_pin;
    unsigned char  m_msgSize;
    unsigned char  m_patternSize;
    t_modeRadio    m_modeRadio;
    RH_ASK *       mp_radio;
    char * mp_buf;
    char * mp_pattern;
  protected:
};


#endif