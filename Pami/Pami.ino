#include "arduino.h"
#include "LibPami.h"
#include "MemoryFree.h"

LibPami pami;


// Selection d'axe de Gyroscope.
// VALEUR    RÉSULTAT
// 0x01      Mesure effectuée
//           Sur l'axe X.
//
// 0x02      Mesure effectuée
//           Sur l'axe Y.
//
// 0x03      Mesure effectuée
//           Sur l'axe Z.
char axis = 0x02;


// Sélection de direction.
// VALEUR    RÉSULTAT
// 0x01      Le robot va avancer
//           en avant.
//
// -0x01     Le robot va avancer
//           en arrière.
char direction = 0x01;


// Selection de vitesse initiale.
// V ∈ [-0x64 : 0x64]
// |!| 
// V < 0 possible mais pas recommandé.
// Si mauvaise direction, changer le
// paramètre "direction" [LIGNE 29]
// Recommandé:
// V ∈ [-0x20 : 0x40]
char baseSpeed = 0x20;


// Selection du cooldown entre
// deux corrections, pour faciliter
// la lecture des messages de debug.
//
// Si pour de vrai:
// T ∈ [0x00 : 0x0a]
//
// Si pour un test:
// V ∈ [0x1e : 0x32]
short cooldown = 0x32;


// Facteur de division
// Sert à réduire la sensibilité
// Du robot pour les angles.
// Valeurs conseillées:
// X ∈ [-0x02 : 0x0a]
char divisionFactor = 0x05;


float offset = 0;  // Corrected declaration


void setup() {
  Serial.begin(9600);

  Serial.print("freeMemory=");
  Serial.println(freeMemory());

  pami.afficheur.begin();
  pami.afficheur.displayString("init");
  pami.ultrason.begin();
  pami.ultrason.setMaxDistance(500);
  pami.gpio.configure(PAMI_GPIO_1, PAMI_GPIO_OUTPUT, 1);
  pami.gpio.configure(PAMI_GPIO_2, PAMI_GPIO_INPUT);
  pami.gpio.configure(PAMI_GPIO_3, PAMI_GPIO_PWM, 0);
  pami.chrono.begin();
  pami.gyro.begin();
  pami.gyro.selectAxis(GYROSCOPE_AXIS_X);
  pami.radio.begin(16);
  pami.moteur.setPwmMode(false);

  pami.afficheur.displayString("done");
  delay(2000);

  pami.chrono.display(true);


  // Définition de l'angle de départ,
  // Et calculs pour en déduire l'offset
  // Par rapport à 0 de celui-ci.

  float initialAngle;

  switch (axis) {
  case 0x01: // X AXIS
    initialAngle = pami.gyro.getAngle(GYROSCOPE_AXIS_X);
    break;

  case 0x02: // Y AXIS
    initialAngle = pami.gyro.getAngle(GYROSCOPE_AXIS_Y);
    break;
    
  case 0x03: // Z AXIS
    initialAngle = pami.gyro.getAngle(GYROSCOPE_AXIS_Z);
    break;
    
  default:
    Serial.println("Failed to identify the selected axis. Please refer to [LINE 18] to select the desired axis.");
    initialAngle = -1;
    return;
  }

    offset = 0 - initialAngle; // Calcule ne nombre à soustraire des angles mesurés pour avoir un écart de l'angle initial.
}

void loop() {
  pami.gestion();

  Serial.print("# Gyroscope #");
  Serial.print("  Axe X=");
  Serial.print(pami.gyro.getAngle(GYROSCOPE_AXIS_X));
  Serial.print(", Axe Y=");
  Serial.print(pami.gyro.getAngle(GYROSCOPE_AXIS_Y));
  Serial.print(", Axe Z=");
  Serial.println(pami.gyro.getAngle(GYROSCOPE_AXIS_Z));

  correctDependingOnAngle();

  delay(cooldown);
}

void correctDependingOnAngle() {
  float angle;

  switch (axis) {
    case 0x01: // X AXIS
      angle = pami.gyro.getAngle(GYROSCOPE_AXIS_X) - offset;
      break;

    case 0x02: // Y AXIS
      angle = pami.gyro.getAngle(GYROSCOPE_AXIS_Y) - offset;
      break;

    case 0x03: // Z AXIS
      angle = pami.gyro.getAngle(GYROSCOPE_AXIS_Z) - offset;
      break;

    default:
      Serial.println("Failed to identify the selected axis. Please refer to [LINE 19] to select the desired axis.");
      angle = -1;
      return;
  }

  Serial.print("Moteur Gauche : ");
  Serial.print(direction * (baseSpeed - angle / divisionFactor));

  Serial.print("       Moteur Droit : ");
  Serial.println(direction * (baseSpeed + angle / divisionFactor));

  pami.moteur.moteurGauche(direction * (baseSpeed - angle / divisionFactor));
  pami.moteur.moteurDroit(direction * (baseSpeed + angle / divisionFactor));
}
