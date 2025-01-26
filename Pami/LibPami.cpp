/*!
 * \file 	LibPami.cpp
 * \brief 	Le fichier source de la classe LibPami
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibPami.h"

#define PAMI_JUMPER_TEAM 16
#define PAMI_JUMPER_NB_L A6
#define PAMI_JUMPER_NB_H A7

#define PAMI_MOTEUR_ENA 11
#define PAMI_MOTEUR_IN1 13
#define PAMI_MOTEUR_IN2  4
#define PAMI_MOTEUR_IN3  8
#define PAMI_MOTEUR_IN4  7
#define PAMI_MOTEUR_ENB  6

#define PAMI_PIN_GPIO_1  3
#define PAMI_PIN_GPIO_2  5
#define PAMI_PIN_GPIO_3  9
#define PAMI_PIN_GPIO_4 10

#define PAMI_ULTRASON_TRIG 15
#define PAMI_ULTRASON_ECHO 14

#define PAMI_AFF1637_DATA 12
#define PAMI_AFF1637_CLK  2

#define PAMI_433MHZ_PIN         17
#define PAMI_433MHZ_SPEED     2000
#define PAMI_433MHZ_PATTERN "LaboCesson"

unsigned char gpioPinList[] = {PAMI_PIN_GPIO_1,PAMI_PIN_GPIO_2,PAMI_PIN_GPIO_3,PAMI_PIN_GPIO_4};

LibPami::LibPami() :
  afficheur(PAMI_AFF1637_CLK,PAMI_AFF1637_DATA),
  jumper(PAMI_JUMPER_TEAM, PAMI_JUMPER_NB_H, PAMI_JUMPER_NB_L),
  moteur(PAMI_MOTEUR_ENA, PAMI_MOTEUR_IN1, PAMI_MOTEUR_IN2, PAMI_MOTEUR_IN3, PAMI_MOTEUR_IN4, PAMI_MOTEUR_ENB),
  gpio(gpioPinList,4),
  ultrason(PAMI_ULTRASON_TRIG,PAMI_ULTRASON_ECHO,1000),
  gyro(),
  radio(RADIO433MHZ_RECV, PAMI_433MHZ_SPEED, PAMI_433MHZ_PIN, PAMI_433MHZ_PATTERN),
  chrono()
{
  // LibChrono::setDisplay(dynamic_cast<LibAff1637*>(this));
  gyro.setDisplay(&afficheur);
  chrono.setDisplay(&afficheur);
}


void LibPami::gestion(void) {
  moteur.gestion();
  afficheur.gestion();
  gyro.gestion();
  chrono.gestion();
}

