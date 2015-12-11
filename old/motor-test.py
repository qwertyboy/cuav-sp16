try:
	import RPi.GPIO as GPIO
except RuntimeError:
	print("Error importing RPi.GPIO! Make sure module is installed and try running as root.")

import time
from multiprocessing import Process

# use GPIO header pin numbers
GPIO.setmode(GPIO.BOARD)

# set up pins as outputs
m1step	= 10
m1dir	= 8
m2step	= 12
m2dir	= 16
outputPins = [m1step, m1dir, m2step, m2dir, 18]
GPIO.setup(outputPins, GPIO.OUT)


# function for controlling a motor with linear acceleration
def moveMotor(stepPin, dirPin, speed):
	# set up some things
	maxStepRate 	= 1000
	newTime		= 0
	oldTime 	= 0
	stepState	= 0
	currentSteps	= 0
	
	# get magnitude of speed and remap it to the step rate
	speedMagnitude = abs(speed)
	stepRate = remap(speedMagnitude, 0, 1, 0, maxStepRate)
	
	# set up compare values
	stepTime = float((1 / stepRate) / 2)
	
	# set the direction
	if speed < 0:
		GPIO.output(dirPin, 0)
		#print("reverse")
	else:
		GPIO.output(dirPin, 1)
		#print("forward")
	
	# send the correct number of steps
	newTime = time.time()
	if (newTime - oldTime) > stepTime:
		#GPIO.output(18, 1)
		currentSteps += 1
		oldTime = newTime
		#stepState = not stepState
		GPIO.output(stepPin, 1)
		#GPIO.output(18, 0)
	
	# make sure pin is off after stepping
	GPIO.output(stepPin, 0)
	return currentSteps
# end moveMotor


# simple function to re-map a range of values
def remap(value, fromLow, fromHigh, toLow, toHigh):
	# get how wide each range is
	fromRange = fromHigh - fromLow
	toRange = toHigh - toLow
	
	# convert low range into a 0-1 range
	valueNew = float(value - fromLow) / float(fromRange)
	
	# convert 0-1 range to high range
	return toLow + (valueNew * toRange)
# end remap()


moveMotor(m2step, m2dir, 0.750, 2000)
moveMotor(m1step, m1dir, 0.750, 2000)
#moveMotor(2, 0.1, 10)

# clean up gpio pins
GPIO.cleanup()
