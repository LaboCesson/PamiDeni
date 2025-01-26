/*!
 * \file 	LibMoteur.h
 * \brief 	Le fichier de définition de la classe LibMoteur
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */
//
// Librairie de gestion d'un L298N
//

#ifndef _LIB_MOTEUR_H_
#define _LIB_MOTEUR_H_

#include "arduino.h"

/// \class LibMoteur
/// \brief Gestion d'un L298N
/// \details
/// <a href="https://www.handsontec.com/dataspecs/L298N%2520Motor%2520Driver.pdf"> L298N Module documentation </a>
class LibMoteur
{
  public:
    LibMoteur(
		unsigned char pinEna, ///< Le numéro de la pin connectée à ENA
		unsigned char pinIn1, ///< Le numéro de la pin connectée à IN1
		unsigned char pinIn2, ///< Le numéro de la pin connectée à IN2
		unsigned char pinIn3, ///< Le numéro de la pin connectée à IN3
		unsigned char pinIn4, ///< Le numéro de la pin connectée à IN4
		unsigned char pinEnb, ///< Le numéro de la pin connectée à ENB
		bool pwmMode = true   ///< Indique si la vitesse est géré en mode PWM (true) ou via une gestion basée sur millis() (false)
	);

    /// \details Cette fonction doit être appelée régulièrement pour la gestion des moteurs
    void gestion(void);
	
    /// \details Permet de changer le mode de variation de la vitesse
    /// \details pwmMode=false doit être utilisé lorsque le timer2 est utilisé par une autre librairie
    void setPwmMode(
        bool pwmMode ///< Indique si la vitesse est géré en mode PWM (true) ou via une gestion basée sur millis() (false)
	);

	/// \details Permet de piloter le moteur gauche
    void moteurGauche(
		char vitesse  ///< vitesse à appliquer, valeurs possibles entre -100 et +100 (0=stop)
	);

	/// \details Permet de piloter le moteur droit
    void moteurDroit(
		char vitesse  ///< vitesse à appliquer, valeurs possibles entre -100 et +100 (0=stop)
	);

	/// \details Permet de piloter les deux moteurs
    void moteurs(
		char vitesse  ///< vitesse à appliquer, valeurs possibles entre -100 et +100 (0=stop)
	);

  private:
    unsigned char m_enableDroite, m_avantDroite, m_arriereDroite;
    unsigned char m_enableGauche, m_avantGauche, m_arriereGauche;
      #define DIRECTION_AUCUNE  0
      #define DIRECTION_AVANT   1
      #define DIRECTION_ARRIERE 2
    unsigned char m_directionDroite = DIRECTION_AUCUNE;
    unsigned char m_directionGauche = DIRECTION_AUCUNE;
    bool          m_pwmMode;
    unsigned char m_nbPasEnA,m_nbPasEnB;

    unsigned char getDirection(char vitesse);
    unsigned char getVitesse(char vitesse);
    unsigned char getNbPas(char vitesse);

  protected:
};


#endif