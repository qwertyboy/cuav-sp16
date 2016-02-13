try:
	import RPi.GPIO as GPIO
except RuntimeError:
	print("Error importing RPi.GPIO! Make sure module is installed and try running as root.")

import time

# use GPIO header pin numbers
GPIO.setmode(GPIO.BOARD)

# set up pins as outputs
m1step	= 10
m1dir	= 8
m2step	= 12
m2dir	= 16
outputPins = [m1step, m1dir, m2step, m2dir]
GPIO.setup(outputPins, GPIO.OUT)
GPIO.output(outputPins, 0)

# set up arrays for motor information
motor1		= 0
motor2		= 1
motorCount	= 2
motorPins	= [[m1step, m1dir], [m2step, m2dir]]
stepTime	= [0, 0]
direction	= [0, 0]
oldTime		= [0, 0]
totalSteps	= [0, 0]
currentSteps	= [0, 0]


# function for updating speed array
def updateSpeed(motorNum, speed):
	# set up some things
	maxStepRate = 1000
	
	# set direction
	if speed < 0:
		# reverse
		direction[motorNum] = 0
	else:
		# forward
		direction[motorNum] = 1
	
	# get magnitude of speed and remap it to the step rate
	speedMagnitude = abs(speed)
	stepRate = remap(speedMagnitude, 0, 1, 0, maxStepRate)
	
	# set up compare values
	if (stepRate == 0):
		stepTime[motorNum] = 0
	else:
		stepTime[motorNum] = float(1 / stepRate)
# end updateSpeed


# function for updating motors while keeping track of steps
def updateMotorSteps():
	newTime = time.time()
	for i in range(motorCount):
		# set direction pin
		GPIO.output(motorPins[i][1], direction[i])
		
		# check time and step count
		if (newTime - oldTime[i] > stepTime[i]) and (currentSteps[i] < totalSteps[i]):
			oldTime[i] = newTime
			currentSteps[i] += 1
			GPIO.output(motorPins[i][0], 1)
		elif (newTime - oldTime[i] > (stepTime[i] / 2)):
			GPIO.output(motorPins[i][0], 0)
# end updateMotors


# function for updating only the speed of the motor
def updateMotorSpeed():
	# get current time
	newTime = time.time()
	for i in range(motorCount):
		# set direction pin
		GPIO.output(motorPins[i][1], direction[i])
		
		# check time
		if (newTime - oldTime[i] > stepTime[i]):
			oldTime[i] = newTime
			GPIO.output(motorPins[i][0], 1)
		elif (newTime - oldTime[i] > (stepTime[i] / 2)):
			GPIO.output(motorPins[i][0], 0)
# end updateMotorSpeed


# function to set a motor to a specific angle
def setAngle(motorNum, speed, angle):
	totalSteps[motorNum] = angle / 1.8
	updateSpeed(motorNum, speed)
	
	if (currentSteps[motorNum] < totalSteps[motorNum]):
		updateMotorSteps()
		return 0
	return 1
# end setAngle


# function to set motor speed
def setSpeed(motorNum, speed):
	updateSpeed(motorNum, speed)
	updateMotorSpeed()
# end setSpeed


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


loopRunning = 1
while loopRunning:
	setAngle(motor1, -0.500, 180)
	setAngle(motor2, -0.500, 270)
	
	loopRunning = 0
	for i in range(motorCount):
		if (currentSteps[i] != totalSteps[i]):
			loopRunning = 1
			break


# clean up gpio pins
GPIO.cleanup()
