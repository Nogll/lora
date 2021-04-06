#include "SX1278.h"
#include <Arduino.h>
#include <SPI.h>
#include <string.h>

SX1278* SX1278::instance = NULL;

SX1278* SX1278::getInstance(SX1278_Config_t *hwConfig){
	if (SX1278::instance == NULL){
		if (hwConfig != NULL){
			SX1278::instance = new SX1278(hwConfig);
		}
	}
	return SX1278::instance;
}

SX1278::SX1278(SX1278_Config_t* hwConfig) {
	this->hwConfig = *(hwConfig);
	pinMode(this->hwConfig.pin_NSS, OUTPUT);
	digitalWrite(this->hwConfig.pin_NSS, HIGH);
	pinMode(this->hwConfig.pin_RESET, OUTPUT);
	digitalWrite(this->hwConfig.pin_RESET, HIGH);
	pinMode(this->hwConfig.pin_DIO0, INPUT);
}

void SX1278::HAL_init(){
	SPI.begin();
	SPI.beginTransaction(SPISettings(1400000, MSBFIRST, SPI_MODE0));
}


void SX1278::HAL_SetNSS(bool value) {
	digitalWrite(this->hwConfig.pin_NSS, (value) ? HIGH : LOW);
}

void SX1278::HAL_Reset() {
	digitalWrite(this->hwConfig.pin_NSS, HIGH);
	digitalWrite(this->hwConfig.pin_RESET, LOW);
	delay(1);
	digitalWrite(this->hwConfig.pin_RESET, HIGH);
	delay(100);
}

void SX1278::HAL_SPICommand(uint8_t cmd) {
	this->HAL_SetNSS(false);
	SPI.transfer(cmd);
}

uint8_t SX1278::HAL_SPIReadByte() {
	this->HAL_SetNSS(false);
	return SPI.transfer(0x00);
}

void SX1278::HAL_Delay(uint32_t msec) {
	delay(msec);
}

bool SX1278::HAL_GetDIO0() {
	return (digitalRead(this->hwConfig.pin_DIO0) == HIGH);
}

void SX1278::SPIBurstRead(uint8_t addr, uint8_t* rxBuf, uint8_t length) {
	  if(length <= 1){
		  return;
	  } else {
		 memset(rxBuf, 0x00, length);
	     this->HAL_SetNSS(false);
	     this->HAL_SPICommand(addr);
	     SPI.transfer(rxBuf, length);
	     this->HAL_SetNSS(true);
	  }
}
