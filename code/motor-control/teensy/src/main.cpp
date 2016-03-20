#include "Arduino.h"
#include "Wire.h"

// pin defs
#define PWMA 3
#define PWMB 4

// i2c cmd defs
#define PWMACMD 0x01
#define PWMBCMD 0x02
#define 

// function prototypes
void rxEvent(int);

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
	char cmd = Wire.read();
	uint16_t val = 0;
	
	char lowByte = Wire.read();
	char highByte = Wire.read();
	val = (highByte << 8) | lowByte;
	
	switch(cmd){
	case PWMACMD:
		// bound the val
		if(val > 1024){val = 1024;}
		else if(val < 0){val = 0;}
		// set pwm output
		analogWrite(PWMA, val);
		break;
	case PWMBCMD:
		// bound the value
		if(val > 1024){val = 1024;}
		else if(val < 0){val = 0;}
		// set pwm
		analogWrite(PWMB, val);
		break;
	default:
		break;
	}
}
