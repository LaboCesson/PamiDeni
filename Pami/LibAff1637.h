/*!
 * \file 	LibAff1637.h
 * \brief 	Le fichier de définition de la classe LibAff1637
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#ifndef _LIB_AFF1637_H_
#define _LIB_AFF1637_H_

#include "arduino.h"

//
//      HH
//     ----
// GH |    | DH
//     -MM-
// GB |    | DB
//     ---
//      BB
// Tous les segments : SEG_GH|SEG_GB|SEG_HH|SEG_MM|SEG_BB|SEG_DH|SEG_DB
//
#define SEG_GH 0x20
#define SEG_GB 0x10
#define SEG_DH 0x02
#define SEG_DB 0x04
#define SEG_HH 0x01
#define SEG_MM 0x40
#define SEG_BB 0x08
#define SEG_PP 0x80


/// \class LibAff1637
/// \brief Gestion d'un afficheur 7 Segments avec un TM1637. 
/// \details
/// <a href="https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/unit/digi_clock/TM1637.pdf">TM1637 documentation</a>
class LibAff1637
{
  public:
    LibAff1637( 
      unsigned char pinClk, ///< Le numéro de la pin connectée à l'entrée Clk
      unsigned char pinData ///< Le numéro de la pin connectée à l'entrée DIO
    );

    /// \details Permet de valider l'utilisation de l'afficheur
    void begin(void);

    /// \details Cette fonction doit être appelée régulièrement pour la gestion de l'afficheur
    void gestion(void);

    /// \details Permet de controler l'état de l'affichage
    void setStatus(
      bool status  ///< Etat de l'afficheur :  true= allumé, false= éteint
    );

    /// \details Permet de controler le cligbotement
    void setBlinking(
      bool blinking  ///< true : clignotement , false : pas de clignotement
    );

    /// \details Permet de régler la luminosité
    void setBrightness(
      unsigned char brightness  ///< valeur de la luminosité de 0 à 7
    );
    
    /// \details Permet d'effacer le contenu de l'afficheur
    void displayClear(void);

    /// \details Affiche le contenu d'un buffer
    void displayBuffer(
      char * buffer,        ///< Pointeur vers le buffer contenant les caractères ASCII à afficher
      unsigned char length, ///< Nombre de caractères à afficher
      unsigned char pos = 0 ///< Premier digit où afficher le contenu du buffer (de 0 à 3)
    );

    /// \details Affiche une chaine de caractère terminée par un 0
    void displayString(
      char * str,            ///< Pointeur vers une chaine de caractère à afficher
      unsigned char pos = 0  ///< Premier digit où afficher la chaine de caractère (de 0 à 3)
    );

    /// \details Permet de controler directement l'état des segments
    void displaySegments(
      unsigned char * segments, ///< Pointeurs vers la définition des segments
      unsigned char length,     ///< Nombre de digits à afficher
      unsigned char pos = 0     ///< Premier digit concerné (de 0 à 3)
    );

    void displayValue(         char value); ///< \param value Valeur à afficher \details Permet d'afficher un char
	void displayValue(unsigned char value); ///< \param value Valeur à afficher \details Permet d'afficher un unsigned char
    void displayValue(         int  value); ///< \param value Valeur à afficher \details Permet d'afficher un int
    void displayValue(unsigned int  value); ///< \param value Valeur à afficher \details Permet d'afficher un unsigned int

  private:
	bool m_begin;
	bool m_blinking;
	bool m_status;
    unsigned char m_pinClk, m_pinDIO;
    unsigned char m_brightness;
    unsigned char m_bitDelay;

    void clear();
    void writeBrightness(unsigned char brightness);
    void writeDigits(const unsigned char digits[], unsigned char length = 4, unsigned char pos = 0);
    unsigned char numToDigit(unsigned int value, unsigned char digits[], unsigned char nbDigit);

    bool writeByte(unsigned char byte);
    void start();
    void stop();
    void bitDelay();

  protected:
};


#endif