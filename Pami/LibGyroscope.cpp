/*!
 * \file 	LibGyroscope.cpp
 * \brief 	Le fichier source de la classe LibGyroscope
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

//#include "I2Cdev.h"
#include "LibGyroscope.h"

#define PERIODE_GESTION         50 // Période de gestion du gyroscope
#define PERIODE_UPDATE_DISPLAY 200 // Période de mise à jour de l'affichage d'un angle


LibGyroscope::LibGyroscope(uint8_t address = MPU6050_DEFAULT_ADDRESS, void *wireObj = 0) :
  mpu() {
    m_begin = false;
  if( wireObj == 0) Wire.begin(); // Si Wire n'a pas été initialisé plus haut, on le fait ici
}


void LibGyroscope::begin(void) {
//  Wire.setClock(400000); // 400kHz I2C clock

  mpu.initialize(); /*Initialize device*/

  /*Verify connection*/
  if(mpu.testConnection() == false) {
    Serial.println("MPU6050 connection failed");
    m_begin = false;
    return;
  }
  Serial.println("MPU6050 connection successful");
  m_begin = true;

  /* Initializate and configure the DMP*/
  unsigned char dmpStatus = calibrate();

  if (dmpStatus != 0) {
    Serial.print(F("DMP Initialization failed (code ")); //Print the error code
    Serial.print(dmpStatus);
    Serial.println(F(")"));
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    return;
  }

}


void LibGyroscope::gestion(void) {
  if(m_begin == false ) return;

  static unsigned long nextTimeGestion = millis(); // Variable persistante
  static unsigned long nextTimeDisplay = millis(); // Variable persistante

  float ypr[3];           // [yaw, pitch, roll]   Yaw/Pitch/Roll container and gravity vector

  unsigned long newTime = millis();

  if( newTime > nextTimeGestion ) {
    if (mpu.dmpGetCurrentFIFOPacket(m_FIFOBuffer)) { // Get the Latest packet
      mpu.dmpGetQuaternion(&q, m_FIFOBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

      m_angleX = (ypr[0] * 180/M_PI);
      m_angleY = (ypr[1] * 180/M_PI);
      m_angleZ = (ypr[2] * 180/M_PI);

      nextTimeGestion += PERIODE_GESTION;
    }
  }

  if( newTime > nextTimeDisplay ) {
    displayAngle();
    nextTimeDisplay += PERIODE_UPDATE_DISPLAY;
  }

  // On évite l'accumulation de traitements le cas écheant
  while(newTime > nextTimeGestion) nextTimeGestion += PERIODE_GESTION;
  while(newTime > nextTimeDisplay) nextTimeDisplay += PERIODE_UPDATE_DISPLAY;
}


unsigned char LibGyroscope::calibrate(void) {
  if (m_begin == false) return 2;

  /* Initializate and configure the DMP*/
  unsigned char dmpStatus = mpu.dmpInitialize();

  /* Supply your gyro offsets here, scaled for min sensitivity */
  mpu.setXGyroOffset(0);
  mpu.setYGyroOffset(0);
  mpu.setZGyroOffset(0);
  mpu.setXAccelOffset(0);
  mpu.setYAccelOffset(0);
  mpu.setZAccelOffset(0);

  if (dmpStatus != 0) return dmpStatus;

  mpu.CalibrateAccel(6);  // Calibration Time: generate offsets and calibrate our MPU6050
  mpu.CalibrateGyro(6);
  mpu.setDMPEnabled(true); //Turning ON DMP
  return 0;
 }


int LibGyroscope::getAngle(t_gyroscopeAxis axis) {
  switch(axis) {
    case GYROSCOPE_AXIS_X : return m_angleX;
    case GYROSCOPE_AXIS_Y : return m_angleY;
    case GYROSCOPE_AXIS_Z : return m_angleZ;
    default : return 0;
  }
}


void LibGyroscope::selectAxis(t_gyroscopeAxis axis) { m_axis = axis;}
int  LibGyroscope::getAngle() { return getAngle(m_axis); }


void LibGyroscope::getAngles(int * p_axisX, int * p_axisY, int * p_axisZ ) {
  p_axisX = m_angleX;
  p_axisY = m_angleY;
  p_axisZ = m_angleZ;
}


void LibGyroscope::display(bool status) { m_displayStatus = status;}
void LibGyroscope::setDisplay(LibAff1637 * p_display) { mp_display = p_display; }

void LibGyroscope::displayAngle(void) {
  if( m_displayStatus == false ) return;
  if( mp_display == NULL) return;
  mp_display->displayValue(getAngle(m_axis));
}


