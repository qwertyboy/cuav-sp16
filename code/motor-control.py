try:
	import RPi.GPIO as GPIO
except RuntimeError:
	print("Error importing RPi.GPIO! Make sure module is installed and try running as root.")

import time

# use GPIO header pin numbers
GPIO.setmode(GPIO.BOARD)

# set up pins as outputs
timingPin = 22
m1step	= 10
m1dir	= 8
m2step	= 12
m2dir	= 16
outputPins = [m1step, m1dir, m2step, m2dir, timingPin]
GPIO.setup(outputPins, GPIO.OUT)
GPIO.output(outputPins, 0)

# set up arrays for motor information
# To add additional (or remove) motors, update motor count,
# add a new motor definition, and add an element to each list
motorCount	= 2
motor1		= 0
motor2		= 1
#motor3		= 2

# format for pins list:
# [[Motor 1 Step, Motor 1 Direction], [Motor 2 Step, Motor 2 Direction], ... ]
motorPins	= [[m1step, m1dir], [m2step, m2dir]]

# add or remove an element from each of these for additional motors
stepTime	= [0, 0]
oldTime		= [0, 0]
direction	= [0, 0]
position	= [0, 0]


# function for updating speed array
def updateSpeedVals(motorNum, speed):
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
			
			# update position array
			if ((direction[i] == 1) and (stepTime[i] != 0)):
				position[i] += 1
			elif ((direction[i] == 0) and (stepTime[i] != 0)):
				position[i] -= 1
			
			GPIO.output(motorPins[i][0], 1)
		elif (newTime - oldTime[i] > (stepTime[i] / 2)):
			GPIO.output(motorPins[i][0], 0)
# end updateMotorSpeed


# function to set absolute angle (assuming no steps are lost)
def setAngle(motorNum, speed, angle):
	updateSpeedVals(motorNum, speed)
	
	if (speed < 0):
		newPosition = int(angle / -1.8)
	else:
		newPosition = int(angle / 1.8)
	
	if (position[motorNum] != newPosition):
		updateMotorSpeed()
		return 0
	return 1
# end setAngle


# function to set motor speed
def setSpeed(motorNum, speed):
	updateSpeedVals(motorNum, speed)
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
	loopRunning = 0
	
	if (position[motor1] > -1000):
		setSpeed(motor1, -0.1)
	else:
		setSpeed(motor1, 0)
	
	if (position[motor2] < 2345):
		setSpeed(motor2, 0.75)
	else:
		setSpeed(motor2, 0)
	
	for i in range(motorCount):
		if (stepTime[i] != 0):
			loopRunning = 1
			break
	

# clean up gpio pins
GPIO.cleanup()
