/*!
 * \file 	LibUltrason.h
 * \brief 	Le fichier de définition de la classe LibPami
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

// Librairies requises :
//    MPU6050 : https://github.com/electroniccats/mpu6050
//    Servo   : https://docs.arduino.cc/libraries/servo/
//    Wire
//

#ifndef _LIB_PAMI_H_
#define _LIB_PAMI_H_

#include "arduino.h"

#define LIB433MHZ_USED

#include "LibAff1637.h"
#include "LibJumper.h"
#include "LibMoteur.h"
#include "LibGpio.h"
#include "LibUltrason.h"
#include "LibGyroscope.h"
#include "Lib433Mhz.h"
#include "LibChrono.h"

typedef enum {
  PAMI_GPIO_1 = 0,
  PAMI_GPIO_2 = 1,
  PAMI_GPIO_3 = 2,
  PAMI_GPIO_4 = 3
} t_pami_gpio;


/// \class LibPami
/// \brief Librairie de gestion d'un PAMI
class LibPami
{
  public:
    LibPami();

    void gestion(void);

    LibAff1637   afficheur; ///< Librairie de gestion d'un afficheur
    LibJumper    jumper;
    LibGpio      gpio;
    LibUltrason  ultrason;
    LibGyroscope gyro;
    LibMoteur    moteur;
    Lib433Mhz    radio;
    LibChrono    chrono;

  private:

  protected:
};

#endif