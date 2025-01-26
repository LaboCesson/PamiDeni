/*!
 * \file 	LibAff1637.cpp
 * \brief 	Le fichier source de la classe LibAff1637
 * \author  LaboCesson
 * \version 1.0
 * \date    Janvier 2025
 */

#include "LibAff1637.h"
#include "AsciiChar.h"

#define TM1637_I2C_COMM1  0x40  // Data Command Set
#define TM1637_I2C_COMM2  0xC0  // Address command set
#define TM1637_I2C_COMM3  0x80  // Display Control Command Set

#define AFF1637_BIT_DELAY 5     // Delai en microsecond

#define PERIOD_CLIGNOTEMENT 500 // en millisecondes


LibAff1637::LibAff1637(uint8_t pinClk, uint8_t pinDIO)
{
  m_pinClk = pinClk;
  m_pinDIO = pinDIO;
  m_bitDelay = AFF1637_BIT_DELAY;
  m_brightness =0x0F;
  m_begin = false;
  m_status = true;
  m_blinking = false;

  // Set the pin direction and default value.
  // Both pins are set as inputs, allowing the pull-up resistors to pull them up
  pinMode(m_pinClk, INPUT); digitalWrite(m_pinClk, LOW);
  pinMode(m_pinDIO,INPUT);  digitalWrite(m_pinDIO, LOW);
  displayClear();
  writeBrightness(m_brightness);
}

void LibAff1637::begin(void) { m_begin = true; }

void LibAff1637::gestion(void) {
  static bool status = false;
  static unsigned long nextTimeGestion = millis();
         unsigned long newTime  = millis();
  
  if( newTime < nextTimeGestion ) return;
  
  if(m_begin == true ) {
    if( m_blinking == false ) {
		writeBrightness((m_brightness & 0x7) | (m_status ? 0x08 : 0x00));
    } else {
	  status = ( status ? false : true );
	  if( status == false ) {
		writeBrightness(0x00);
	  } else {
		writeBrightness((m_brightness & 0x7) | (m_status ? 0x08 : 0x00));
	  }
	}
  }
  nextTimeGestion += PERIOD_CLIGNOTEMENT;
  
  // On évite l'accumulation de traitements le cas écheant
  while(newTime > nextTimeGestion) nextTimeGestion += PERIOD_CLIGNOTEMENT;
}

//void LibAff1637::setStatus( bool status )     { m_status = status; }
void LibAff1637::setBlinking( bool blinking ) { m_blinking = blinking; }


void LibAff1637::setBrightness(unsigned char brightness) {
  m_brightness = brightness;
  writeBrightness((m_brightness & 0x7) | (m_status ? 0x08 : 0x00));
}


void LibAff1637::setStatus(bool status) {
  m_status = status;
  writeBrightness((m_brightness & 0x7) | (m_status ? 0x08 : 0x00));
}


void LibAff1637::displayClear() {
  uint8_t data[] = { 0, 0, 0, 0 };
  writeDigits(data);
}


void LibAff1637::displayBuffer( char * buffer, unsigned char length, unsigned char pos = 0) {
  unsigned char digits[4];

  for( unsigned char i; i<length; i++ ) {
    digits[i] = asciiDef[(*buffer++)-OFFSET_ASCII];
  }
  
  writeDigits(digits, length, pos);
}


void LibAff1637::displayString( char * str, unsigned char pos = 0) {
  displayBuffer( str, min(strlen(str),4), pos );
}


void LibAff1637::displayValue(char value) {
  unsigned char digits[4]={0,0,0,0};
  unsigned char positif = ( value >= 0  ? true : false );
  unsigned char position = numToDigit( (unsigned int) (positif== true ? value : -value ), digits, 4);
  digits[position]=asciiDef[(positif== true ? ' ' : '-' )-OFFSET_ASCII];
  displaySegments(digits, 4);
}


void LibAff1637::displayValue(unsigned char value) {
  unsigned char digits[4]={0,0,0,0};
  numToDigit((unsigned int)value, digits, 4);
  displaySegments(digits, 4);
}


void LibAff1637::displayValue(int value) {
  displayClear();
  if((value>9999) or (value<-999)) { displayString("Err"); return;}
  unsigned char digits[4]={0,0,0,0};
  unsigned char positif = ( value >= 0  ? true : false );
  unsigned char position = numToDigit( (positif== true ? value : -value ), digits, 4);
  digits[position]=asciiDef[(positif== true ? ' ' : '-' )-OFFSET_ASCII];
  displaySegments(digits, 4);
}


void LibAff1637::displayValue(unsigned int value) {
  displayClear();
  if(value>9999) { displayString("Err"); return;}
  unsigned char digits[4]={0,0,0,0};
  numToDigit(value, digits, 4);
  displaySegments(digits, 4);
}


unsigned char LibAff1637::numToDigit(unsigned int value, unsigned char digits[], unsigned char nbDigit) {
  do {
    nbDigit--;
    digits[nbDigit] = asciiDef[(value%10)+(0x30-OFFSET_ASCII)];
    value /= 10;
  } while( (value !=0 ) || (nbDigit<0));
  nbDigit--;
  return nbDigit;
}


void LibAff1637::displaySegments( unsigned char * segments, unsigned char length, unsigned char pos = 0) {
  writeDigits(segments, length, pos);
}


void LibAff1637::writeDigits(const unsigned char digits[], unsigned char length, unsigned char pos) {
  // Commande 1 avec autoIncrement 
  start();
  writeByte(TM1637_I2C_COMM1);
  stop();

  // Commande 2 avec sélection du premier digit a ecrire
  start();
  writeByte(TM1637_I2C_COMM2 + (pos & 0x03));

  // Ecriture des digits
  for (unsigned char i=0; i < length; i++)
    writeByte(digits[i]);
  stop();
}


void LibAff1637::writeBrightness(unsigned char brightness) {
  // Command 3 + brightness
  start();
  writeByte(TM1637_I2C_COMM3 + (brightness & 0x0f));
  stop();
}


bool LibAff1637::writeByte(unsigned char byte) {
  unsigned char ack;

  // on ecrit les 8 bits
  for(unsigned char i = 0; i<8; i++) {
    pinMode(m_pinClk, OUTPUT); // CLK low
    bitDelay();
    pinMode(m_pinDIO, (byte & 0x01 ? INPUT : OUTPUT));
    bitDelay();
    pinMode(m_pinClk, INPUT); // CLK high
    bitDelay();
    byte = byte >> 1;
  }

  // Wait for acknowledge
  pinMode(m_pinClk, OUTPUT); // CLK low
  pinMode(m_pinDIO, INPUT);
  bitDelay();
  pinMode(m_pinClk, INPUT);  // CLK high
  bitDelay();
  ack = digitalRead(m_pinDIO);
  if (ack == 0)
    pinMode(m_pinDIO, OUTPUT);

  bitDelay();
  pinMode(m_pinClk, OUTPUT); // CLK low
  bitDelay();

  return ack;
}


void LibAff1637::start() {
  pinMode(m_pinDIO, OUTPUT);
  bitDelay();
}


void LibAff1637::stop() {
  pinMode(m_pinDIO, OUTPUT);
  bitDelay();
  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinDIO, INPUT);
  bitDelay();
}


void LibAff1637::bitDelay() {
  delayMicroseconds(m_bitDelay);
}
