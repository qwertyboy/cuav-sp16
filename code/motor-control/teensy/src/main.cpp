#include "Arduino.h"
#include "Wire.h"

// pin defs
#define PWMA 3
#define DIRA 5
#define SLPA 6
#define PWMB 4
#define DIRB 7
#define SLPB 8

// i2c cmd defs
#define PWMACMD 0x01
#define DIRACMD 0x02
#define PWMBCMD 0x06
#define DIRBCMD 0x07
#define SLPCMD  0x0C

// function prototypes
void rxEvent(int);
void setPWM(uint8_t, uint16_t);
void setDirection(uint8_t, uint16_t);
void setSleep(uint8_t, uint8_t, uint16_t);

int main(){
	// set pwm output pins
	pinMode(PWMA, OUTPUT);
	pinMode(PWMB, OUTPUT);
	pinMode(12, OUTPUT);
	
	// change pwm frequency, PWMA and PWMB are on the same timer
	analogWriteFrequency(PWMA, 20000);
	
	// set resolution to 10 bits
	analogWriteResolution(10);
	
	// set pwm to 0
	analogWrite(PWMA, 0);
	analogWrite(PWMB, 0);
	
	// begin I2C, slave address 0x01
	// register event on receive
	Wire.begin(0x10);
	Wire.onReceive(rxEvent);
	
	while(1){
	}
}

void rxEvent(int numBytes){
	// first byte is the command
	char cmd = Wire.read();
	uint16_t val = 0;
	
	// next two bytes are the value
	char lowByte = Wire.read();
	char highByte = Wire.read();
	val = (highByte << 8) | lowByte;
	
	switch(cmd){
	case PWMACMD:
		setPWM(PWMA, val);
		break;
	case PWMBCMD:
		setPWM(PWMB, val);
		break;
	case DIRACMD:
		setDirection(PWMA, val);
		break;
	case DIRBCMD:
		setDirection(PWMB, val);
		break;
	case SLPCMD:
		setSleep(SLPA, SLPB, val);
		break;
	default:
		break;
	}
}

// function to set the pwm output
void setPWM(uint8_t channel, uint16_t val){
	// bound the value
	if(val > 1023){val = 1023;}
	else if(val < 0){val = 0;}
	// set output
	analogWrite(channel, val);
}

// function to set the direction of the motor
void setDirection(uint8_t channel, uint16_t state){
	if(state == 0 || state == 1){
		// 0 is forward, 1 is reverse
		digitalWrite(channel, !state);
	}
}

// function to power down or enable the motor drivers
void setSleep(uint8_t slpA, uint8_t slpB, uint16_t state){
	if(state == 0 || state == 1){
		digitalWrite(slpA, state);
		digitalWrite(slpB, state);
	}
}
