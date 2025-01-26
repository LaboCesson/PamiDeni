/*!
 * \file 	LibGpio.h
 * \brief 	Le fichier de définition de la classe LibGpio
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#ifndef _LIB_GPIO_H_
#define _LIB_GPIO_H_

#define SERVO_USED
//#define PWMSERVO_USED
//#define SERVOTIMER2_USED

#include "arduino.h"
#ifdef SERVO_USED
  #include "Servo.h"
#endif
#ifdef PWMSERVO_USED
  #include "PWMServo.h"
#endif
#ifdef SERVOTIMER2_USED
  #include "ServoTimer2.h"
#endif

typedef enum {
    PAMI_GPIO_UNUSED = 0, ///< GPIO not used
    PAMI_GPIO_INPUT  = 1, ///< GPIO used as Input
    PAMI_GPIO_OUTPUT = 2, ///< GPIO used as Output
    PAMI_GPIO_PWM    = 3  ///< GPIO used as PWM
} t_gpioMode;

#define LIB_GPIO_ERROR 10000 ///< Valeur retournée lorsqu'une erreur est rencontrée


/// \class LibGpio
/// \brief Gestion des Entrées/Sorties d'un PAMI
class LibGpio
{
  public:
    LibGpio(
		unsigned char * pinList,  ///< Pointeur vers la liste de pin de type GPIO
		unsigned char nbGpioInTab ///< Nombre de pins dans la liste
	);

	/// \details Permet de configurer le mode associé à un GPIO. Les modes disponibles sont les suivants :
	/// - \ref PAMI_GPIO_UNUSED  pour un GPIO non utilisé
	/// - \ref PAMI_GPIO_INPUT   pour un GPIO utilisé en entrée
	/// - \ref PAMI_GPIO_OUTPUT  pour un GPIO utilisé en sortie
	/// - \ref PAMI_GPIO_PWM     pour un GPIO utilisé en mode PWM
 	/// \return true ou false si le mode est inconnu ou si le GPIO n'existe pas
	/// \attention Le GPIO utilisant la pin 3 ne peut pas être utilisé en mode PWM
   bool configure(
		unsigned char  gpioIdx,         ///< Numéro d'index du GPIO à configurer (à partir de 0)
		t_gpioMode gpioMode,            ///< Type de configuration du pin
		unsigned short defaultValue = 0 ///< Valeur par défaut à appliquer
	);

	/// \details Permet de piloter la sortie d'un GPIO
 	/// \return true ou \ref LIB_GPIO_ERROR si le GPIO n'existe pas
    bool set(
		unsigned char gpioIdx, ///< Numéro d'index du GPIO à piloter(à partir de 0)
		unsigned short value   ///< Valeur à appliquer
	);

 	/// \details Permet de lire l'entrée d'un GPIO
 	/// \return la valeur lue ou \ref LIB_GPIO_ERROR si le GPIO n'existe pas	
	unsigned short get(
		unsigned char gpioIdx  ///< Numéro d'index du GPIO à lire (à partir de 0)
	);

  private:
    unsigned char m_nbGpio;
  
    typedef struct {
      unsigned char  pin;
      unsigned char  type;
      unsigned short value;
      #ifdef SERVO_USED
		Servo* servo;
	  #endif
      #ifdef PWMSERVO_USED
        PWMServo* servo;
	  #endif
      #ifdef SERVOTIMER2_USED
		ServoTimer2 * servo;
	  #endif
    } t_gpio;
    t_gpio * m_gpio;
 
    void trace( unsigned char gpioIdx, char * msg );

  protected:
};

#endif