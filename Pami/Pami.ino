#include "arduino.h"
#include "LibPami.h"
#include "MemoryFree.h"

#define Guitare PAMI_GPIO_3
#define Head PAMI_GPIO_2

#define defaultAngle 90

LibPami pami;

//short angleChange[] = {90, 0, 0, -90};

bool zbeub = false;
bool coOverride = false;
bool awaitingDistance = false;
bool cc = true;
bool dance = false;

unsigned long timedAction = -1;

char timedActionSelector = 1;

char targetAngleOffset = 0;

constexpr char slopeMin = 0x08;
constexpr char slopeMax = 0x0f;  

char SINE = 0;

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
char axis = 0x01;


// Selection d'action lors que le
// Robot va finir l'ascention de
// La pente de la rampe de truc.
// VALEUR    RÉSULTAT
// 0x01      Lors que le robot
//           atteint la fin de la
//           pente, il va s'arrêter.
// 0x02      Lors que le robot
//           atteint la fin de la
//           pente, il va changer
//           l'angle vers lequel
//           s'orienter à 90°.
// 0x03      Lors que le robot
//           atteint la fin de la
//           pente, il va changer
//           l'angle vers lequel
//           s'orienter à -90°.
char actionSelector = 0x05;


// Sélection de direction.
// VALEUR    RÉSULTAT
// 0x01      Le robot va avancer
//           en avant.
//
// -0x01     Le robot va avancer
//           en arrière.
char direction = -0x01;


// Selection de vitessse initiale.
// V ∈ [-0x64 : 0x64]
// |!| 
// V < 0 possible mais pas recommandé.
// Si mauvaise direction, changer le
// paramètre "direction" [LIGNE 29]
// Recommandé:
// V ∈ [0x20 : 0x40]
char baseSpeed = 0x40;


// Selection du cooldown entre
// deux corrections, pour faciliter
// la lecture des messages de debug.
//
// Si pour de vrai:
// T ∈ [0x00 : 0x0a]
//
// Si pour un test:
// V ∈ [0x1e : 0x32]
short cooldown = 0x00;


// Facteur de division
// Sert à réduire la sensibilité
// Du robot pour les angles.
// Valeurs conseillées:
// X ∈ [-0x02 : 0x0a]
char divisionFactor = 1;


// Vitesse maximale
// Vitesse que la correction
// Ne pourra pas dépasser.
// Sert à éviter des drifts
// Et des crashs dans des
// Murs de merde.
char maxSpeed = 0xff;


float offset = 0;  // Corrected declaration
unsigned long startTime;


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
  //pami.chrono.begin();
  pami.gyro.begin();
  pami.gyro.selectAxis(GYROSCOPE_AXIS_X);
  pami.radio.begin(16);
  pami.moteur.setPwmMode(false);

  pami.afficheur.displayString("done");
  delay(200);
  pami.gyro.display(true);

  pami.chrono.display(true);

  pami.gpio.configure(Guitare, PAMI_GPIO_PWM, defaultAngle);
  pami.gpio.configure(Head, PAMI_GPIO_PWM, defaultAngle);


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
    Serial.println("Incorrect axis.");
    initialAngle = -1;
    return;
  }

    offset = 0 - initialAngle; // Calcule ne nombre à soustraire des angles mesurés pour avoir un écart de l'angle initial.

    startTime = millis();
}

void loop() {
  pami.gestion();

  //Serial.print("# Gyroscope #");
  //Serial.print("  Axe X=");
  //Serial.print(pami.gyro.getAngle(GYROSCOPE_AXIS_X));
  //Serial.print(", Axe Y=");
  //Serial.print(pami.gyro.getAngle(GYROSCOPE_AXIS_Y));
  //Serial.print(", Axe Z=");
  //Serial.println(pami.gyro.getAngle(GYROSCOPE_AXIS_Z));

  if (cc) {conditionnalCorection();}
  correctDependingOnAngle();
  onEdge();
  timedActionsManager();

  if (dance){
    nonMandatoryActions();
    SINE = (char)(sin(millis() / 1000.0) * 45);
  }

  //delay(cooldown);

  //short distance = 0; // OBTENSION DE LA DISTANCE AVEC LE ULTRASON ICI!
  //distance = pami.ultrason.getDistance();

  //Serial.print("Distance mesurée par le ultrason: ");
  //Serial.println(distance);
}

void timedActionsManager(){
  if (timedAction == -1) {
    return;
  }

  if (millis() > timedAction) {
      if (timedActionSelector == 1) {
        divisionFactor = 1;
        baseSpeed = 30;
        timedAction = -1;
        cc = false;

        Serial.println("timeA exec");
      }
  }
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
      Serial.println("Invalid Axis");
      angle = -1;
      return;
  }

  //unsigned long time = millis() - startTime;
  //unsigned short step = (time / 1000) % 4;


  //Serial.print("Moteur Gauche : ");
  //Serial.print(max(-100, min(100, direction * (baseSpeed - (angle + angleChange[step]) / divisionFactor))));

  //Serial.print("       Moteur Droit : ");
  //Serial.println(max(-100, min(100, direction * (baseSpeed + (angle + angleChange[step]) / divisionFactor))));

  if (!coOverride){
    //pami.moteur.moteurGauche(max(-100, min(100, direction * (baseSpeed - (angle + angleChange[step]) / divisionFactor))));
    //pami.moteur.moteurDroit(max(-100, min(100,direction * (baseSpeed + (angle + angleChange[step]) / divisionFactor))));
    
    //Serial.print("Moteur Gauche : ");
    //Serial.print(max(-100, min(100, direction * (baseSpeed - (angle + angleChange[step]) / divisionFactor))));
    //Serial.print("       Moteur Droit : ");
    //Serial.println(max(-100, min(100, direction * (baseSpeed + (angle + angleChange[step]) / divisionFactor))));

    char mg = direction * (baseSpeed - (angle + targetAngleOffset));
    char md = direction * (baseSpeed + (angle + targetAngleOffset));

    //Serial.print("Moteur Gauche : ");
    //Serial.print((short)(mg / divisionFactor));
    //Serial.print("       Moteur Droit : ");
    //Serial.println((short)(mg / divisionFactor));
    
    pami.moteur.moteurGauche(mg / divisionFactor);
    pami.moteur.moteurDroit(md / divisionFactor);

    //pami.moteur.moteurGauche(clampValues(mg, maxSpeed * -1, maxSpeed));
    //pami.moteur.moteurDroit(clampValues(md, maxSpeed * -1, maxSpeed));

    
  }
}


void conditionnalCorection(){

  char angle = abs(pami.gyro.getAngle(GYROSCOPE_AXIS_Y));

  if (slopeMin < angle && angle < slopeMax && !zbeub){
    Serial.print("cc slope: ");
    Serial.print(angle);
    Serial.println("°.");
    zbeub = true;
  }

  if (zbeub && angle < slopeMin) {

    switch (actionSelector) {
      case 0x01:
        // Action 0x01 correspond à l'arrêt du robot.
        pami.moteur.moteurGauche(0);
        pami.moteur.moteurDroit(0);
        if (!coOverride){
          Serial.println("Overriden speed controls.");
          coOverride = true;
        }
        break;


      case 0x02:
        // Action 0x02 correspond à une rotation de 90°

        targetAngleOffset = 90;
        Serial.println("targetAngle = 90");
        zbeub = false;
        break;


      case 0x03:
        // Action 0x03 correspond à une rotation de -90°

        targetAngleOffset = -90;
        Serial.println("targetAngle = -90.");
        zbeub = false;
        break;


      case 0x04:
        // Action 0x02 correspond à une rotation de 90°, puis de l'arrêt sur le bord de la table.

        targetAngleOffset = 90;
        Serial.println("target angle = 90.");
        zbeub = false;

        //Serial.println("conditionnalCorrection has set awaitingDistance to TRUE. Robot will now stop when it reaches an edge of the table.");
        awaitingDistance = true;

        baseSpeed = 20;
        divisionFactor = 2;

        timedAction = millis() + 1000;
        timedActionSelector = 1;

        break;


      case 0x05:
        // Action 0x03 correspond à une rotation de -90°, puis de l'arrêt sur le bord de la table.

        targetAngleOffset = -90;
        Serial.println("target angle = -90.");
        zbeub = false;

        //Serial.println(F"conditionnalCorrection has set awaitingDistance to TRUE. Robot will now stop when it reaches an edge of the table.");
        awaitingDistance = true;

        baseSpeed = 20;
        divisionFactor = 2;

        timedAction = millis() + 1000;
        timedActionSelector = 1;

        break;
  

      default:
        //Serial.println(F"Failed to identify the selected action. Please refer to [LINE ---] to select the desired action.");
        zbeub = false;
        return;
    }
  }
}

void onEdge() {
  if (awaitingDistance) {
    short distance = 0; // OBTENSION DE LA DISTANCE AVEC LE ULTRASON ICI!
    distance = pami.ultrason.getDistance();

    if (distance == 500) {
      return;
    }

    Serial.print("Distance mesurée par le ultrason: ");
    Serial.println(distance);

    if ((short)distance >= (short)(65)) { // Hauteur réelle: 55mm (5.5cm). Changer si besoin pour correspondre aux millimètres!!
      pami.moteur.moteurGauche(0);
      pami.moteur.moteurDroit(0);
      coOverride = true;
      awaitingDistance = false;
      zbeub = false;

      Serial.println("Robot stopped.");

      dance = true;

      //Serial.println(F"onEdge has overriten the speeds, and taken the lead of speed control.");
      //Serial.println(F"onEdge has stopped the robot. It will now longer move until it is restarted.");
    }
  }
} 

void nonMandatoryActions(){
  pami.gpio.set(Head   ,   defaultAngle+SINE);
  pami.gpio.set(Guitare,   defaultAngle-SINE);
}

//char clampValues(char v, char mn, char mx) {
//  return min(max(v, mn), mx);
//}