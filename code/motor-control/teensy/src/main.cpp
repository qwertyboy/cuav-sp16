#include <Arduino.h>
#include <Wire.h>
#include <ADC.h>

// motor pin defs
#define PWMA 3
#define DIRA 5
#define SLPA 6
#define PWMB 4
#define DIRB 7
#define SLPB 8

// adc pin defs
#define ADC1 0
#define ADC2 1
#define ADCPIN1 A1
#define ADCPIN2 A2

// i2c cmd defs
#define I2CADDR 0x10
#define PWMACMD 0x01
#define DIRACMD 0x02
#define PWMBCMD 0x06
#define DIRBCMD 0x07
#define SLPCMD  0x0C

#define ADC1CMD 0x0D
#define ADC2CMD 0x0E

// function prototypes
void pwmSetup(void);
void adcSetup(void);
void rxEvent(int);
void txEvent(void);
void setPWM(uint8_t, uint16_t);
void setDirection(uint8_t, uint16_t);
void setSleep(uint8_t, uint8_t, uint16_t);

// adc object
ADC * adc = new ADC();
elapsedMicros time;

// variable defs
uint16_t pwmAval, pwmBval = 0;
uint8_t dirAflag, dirBflag = 0;
uint8_t sleepFlag = 0;
uint16_t adc1val, adc2val = 0;

int main(){
	// setup functions
	pwmSetup();
	adcSetup();
	ADC::Sync_result result;
	
	// begin I2C, slave address 0x01
	// register event on receive
	Wire.begin(I2CADDR);
	Wire.onReceive(rxEvent);
	Wire.onRequest(txEvent);
	
	Serial.begin(9600);
	
	while(1){
		// read the adc
		result = adc->readSynchronizedContinuous();
		adc1val = (uint16_t)result.result_adc0;
		adc2val = (uint16_t)result.result_adc1;
		
		// update outputs
		setSleep(SLPA, SLPB, sleepFlag);
		setDirection(DIRA, dirAflag);
		setDirection(DIRB, dirBflag);
		setPWM(PWMA, pwmAval);
		setPWM(PWMB, pwmBval);
	}
}

// pwm setup function
void pwmSetup(){
	// set output pins
        pinMode(PWMA, OUTPUT);
        pinMode(DIRA, OUTPUT);
        pinMode(SLPA, OUTPUT);
        pinMode(PWMB, OUTPUT);
        pinMode(DIRB, OUTPUT);
        pinMode(SLPB, OUTPUT);

        // change pwm frequency, PWMA and PWMB are on the same timer
        analogWriteFrequency(PWMA, 20000);

        // set resolution to 10 bits
        analogWriteResolution(10);

        // set pwm to 0
        analogWrite(PWMA, 0);
        analogWrite(PWMB, 0);
}

// adc setup function
void adcSetup(){
	// pins as input
	pinMode(ADCPIN1, INPUT);
	pinMode(ADCPIN2, INPUT);
	
	// ADC0 setup
	adc->setAveraging(1);
	adc->setResolution(12);
	adc->setConversionSpeed(ADC_HIGH_SPEED);
	adc->setSamplingSpeed(ADC_HIGH_SPEED);
	
	// ADC1 setup
	adc->setAveraging(1, ADC_1);
	adc->setResolution(12, ADC_1);
	adc->setConversionSpeed(ADC_HIGH_SPEED, ADC_1);
	adc->setSamplingSpeed(ADC_HIGH_SPEED, ADC_1);
	
	// start reading
	adc->startSynchronizedContinuous(ADCPIN1, ADCPIN2);
}

// function that gets called when i2c data is received
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
		pwmAval = val;
		break;
	case PWMBCMD:
		pwmBval = val;
		break;
	case DIRACMD:
		dirAflag = !val;
		break;
	case DIRBCMD:
		dirBflag = !val;
		break;
	case SLPCMD:
		sleepFlag = val;
		break;
	default:
		break;
	}
}

// function that gets called when a request is recieved
void txEvent(){
	char cmd = Wire.read();
	
	switch(cmd){
	case ADC1CMD:
		Wire.write(adc1val);
		Serial.println(adc1val * (3.3 / adc->getMaxValue()));
		break;
	case ADC2CMD:
		break;
	default:
		break;
	}
}

// function to set the pwm output
void setPWM(uint8_t pwmPin, uint16_t val){
	// bound the value
	if(val > 1023){val = 1023;}
	else if(val < 0){val = 0;}
	// set output
	analogWrite(pwmPin, val);
}

// function to set the direction of the motor
void setDirection(uint8_t dirPin, uint16_t state){
	if(state == 0 || state == 1){
		digitalWrite(dirPin, state);
	}
}

// function to power down or enable the motor drivers
void setSleep(uint8_t slpAPin, uint8_t slpBPin, uint16_t state){
	if(state == 0 || state == 1){
		digitalWrite(slpAPin, state);
		digitalWrite(slpBPin, state);
	}
}
