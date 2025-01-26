/*!
 * \file 	LibGyroscope.h
 * \brief 	Le fichier de définition de la classe LibGyroscope
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#ifndef _LIB_GYROSCOPE_H_
#define _LIB_GYROSCOPE_H_

#include "arduino.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "MPU6050.h"
#include "libAff1637.h"

typedef enum {
    GYROSCOPE_AXIS_X = 0, ///< Angle X
    GYROSCOPE_AXIS_Y = 1, ///< Angle Y
    GYROSCOPE_AXIS_Z = 2  ///< Angle Z
} t_gyroscopeAxis;


/// \class LibGyroscope
/// \brief Gestion d'un gyroscope basée sur un MPU6050 
/// \details
/// <a href="https://www.gotronic.fr/pj-956.pdf"> MPU6050 documentation </a>
/// <a href="https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf"> MPU6050 Register documentation </a>
/// <a href="https://www.sciallastore.com/wp-content/uploads/2020/10/MPU-6050-Module.pdf"> MPU6050 Module documentation </a>
class LibGyroscope
{
  public:
    LibGyroscope(uint8_t address = MPU6050_DEFAULT_ADDRESS, void *wireObj = 0);

    /// \details Permet d(initialiser et de valider le gyroscope
    void begin(void);

    /// \details Cette fonction doit être appelée régulièrement pour la gestion du gyroscope
    void gestion(void);

    /// \details Permet de recalibrer le gyroscope
    /// \return 0 si OK et 1 ou 2 sinon
    unsigned char calibrate(void);

    /// \details Permet de selectionner un axe à privilégier
    void selectAxis(
      t_gyroscopeAxis axis ///< Axe à privilégier
    );

    /// \details Permet de lire l'angle d'un axe
    /// \return La valeur en degré de l'angle
    int getAngle(
      t_gyroscopeAxis axis ///< Axe dont l'angle est à retourner
    );

    /// \details Permet de lire l'angle de l'axe sélectionné
    /// \return La valeur en degré de l'angle
    int getAngle();

    /// \details Permet de lire la valeur des 3 angles
    void getAngles(
      int * p_axisX, ///< Pointeur ou écrire la valeur de l'axe X
      int * p_axisY, ///< Pointeur ou écrire la valeur de l'axe Y
      int * p_axisZ  ///< Pointeur ou écrire la valeur de l'axe Z
    );

    /// \details Permet de valider l'affichage sur le display l'angle de l'axe sélectionné
    void display(bool status); ///< true or false

    /// \details Permet de donner les références de l'afficheur
    void setDisplay(LibAff1637 * p_display);  ///< Pointeur vers le gestionnaire d'affichage

  protected:

  private:
    bool  m_begin;
    short m_angleX;
    short m_angleY;
    short m_angleZ;

    t_gyroscopeAxis m_axis;

    bool m_displayStatus;
    LibAff1637 * mp_display;

    MPU6050     mpu;
    Quaternion  q;       // [w, x, y, z] Quaternion container
    VectorFloat gravity; // [x, y, z]    Gravity vector
    unsigned char m_FIFOBuffer[64]; // FIFO storage buffer

    void displayAngle(void);
};


#endif