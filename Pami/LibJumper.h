/*!
 * \file 	LibJumper.h
 * \brief 	Le fichier de définition de la classe LibJumper
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#ifndef _LIB_JUMPER_H_
#define _LIB_JUMPER_H_

#include "arduino.h"

typedef enum {
      PAMI_TEAM_A = 0, ///< Jumper non positionné  :::
      PAMI_TEAM_B = 1  ///< Jumper positionne      |::
} t_pamiTeam;


typedef enum {
      PAMI_SING = 0, ///< Jumper :|| = Chanteur
      PAMI_NB_1 = 1, ///< Jumper ::: = Groupie 1
      PAMI_NB_2 = 2, ///< Jumper ::| = Groupie 2
      PAMI_NB_3 = 3  ///< Jumper :|: = Groupie 3
} t_pamiNumber;


/// \class LibJumper
/// \brief Gestion des Jumpers d'un PAMI
class LibJumper
{
  public:
    LibJumper(
		unsigned char pinJumperTeam, ///< Le numéro de la pin connectée au Jumper Team
		unsigned char pinJumperNbH,  ///< Le numéro de la pin connectée au Jumper Numéro de poids 2
		unsigned char pinJumperNbL   ///< Le numéro de la pin connectée au Jumper Numéro de poids 1
	);

	/// \details Retourne le numéro de l'équipe auquel appartient le PAMI
	/// - \ref PAMI_TEAM_A pour l'équipe A
	/// - \ref PAMI_TEAM_B pour l'équipe B
    t_pamiTeam getTeam(void);
	
	/// \details Retourne le numéro du PAMI
	/// - \ref PAMI_SING pour un chanteur
	/// - \ref PAMI_NB_1 pour la groupie 1
	/// - \ref PAMI_NB_1 pour la groupie 2
	/// - \ref PAMI_NB_1 pour la groupie 3
    t_pamiNumber getPami(void);

  private:
    unsigned char m_pinJumperTeam, m_pinJumperNbH, m_pinJumperNbL;

  protected:
};


#endif