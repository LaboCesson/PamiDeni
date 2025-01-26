/*!
 * \file 	LibChrono.h
 * \brief 	Le fichier de définition de la classe LibChrono
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#ifndef _LIB_CHRONO_H_
#define _LIB_CHRONO_H_

#include "arduino.h"
#include "libAff1637.h"


typedef enum {
    CHRONO_MODE_COMPTEUR = 0, ///< Mode compteur
    CHRONO_MODE_DECOMPTE = 1  ///< Mode décompteur
} t_ChronoMode;


/// \class LibChrono
/// \brief Gestion d'un chronometre
class LibChrono
{
  public:
    LibChrono();
	
    /// \details Permet de valider l'utilisation du chrono
	  void begin(void);

    /// \details Permet de définir le mode du chrono
	  /// - \ref CHRONO_MODE_COMPTEUR  Le chronomètre est incrémenté
	  /// - \ref CHRONO_MODE_DECOMPTE  Le chronomètre est décrémenté
    void setMode(t_ChronoMode mode); ///<  \param mode Mode de comptage du chronomètre

    /// \details Permet de définir une alarme.
    ///          Lorsque l'alarme est échue la valeur du chronomètre est figée
    void setAlarme(unsigned short alarmTime); ///< \param alarmTime Valeur a laquelle l'alarme doit être déclenchée

    /// \details Teste si l'alarme est échue
    /// \return  Retourne true si l'alarme est échue et false sinon
    bool isAlarm(void);

    /// \details Permet de réinitialiser le flag indiquant une alarme
    void resetAlarm(void);

    /// \details Retourne la valeur du chronomètre
    /// \return la valeur courante du chronomètre
    unsigned int getValue(void);

    /// \details Permet de démarrer le chronomètre
    void start(unsigned short value); ///< \param value Valeur du chronomètre au démarrage

    /// \details Permet d'arreter le chronomètre
    void stop(void);

    /// \details Permet de valider l'affichage de la valeur du chronomètre sur le display
    void display(bool status); ///< \param  status true or false

    /// \details Permet de donner un pointeur vers la classe en charge de la gestion de l'afficheur
    void setDisplay(LibAff1637 * p_display); ///< \param Pointeur vers l'objet en charge de l'afficheur

    /// \details Cette fonction doit être appelée régulièrement pour la gestion du chronomètre
    void gestion(void);

  protected:

  private:
    bool m_begin;
    bool m_display_status;
    bool m_status_chrono;
    bool m_status_alarm;
    unsigned char  m_mode;
    unsigned short m_value;
    unsigned short m_alarm;
	unsigned int   m_lastValue;
	unsigned long  m_nextTimeInc;
    LibAff1637 * mp_display;

    void displayChrono(void);
};

#endif