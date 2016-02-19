try:
	import RPi.GPIO as GPIO
except RuntimeError:
	print("Error importing RPi.GPIO! Make sure module is installed and try running as root.")

import time
import pygame

# use GPIO header pin numbers
GPIO.setmode(GPIO.BOARD)
GPIO.cleanup()

# set up pins as outputs
m1step	= 8
m1dir	= 10
m1en	= 12
m2step	= 16
m2dir	= 18
m2en	= 22
outputPins = [m1step, m1dir, m1en, m2step, m2dir, m2en]
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
# [[Motor 1 Step, Motor 1 Direction, Motor 1 Enable], [Motor 2 Step, Motor 2 Direction, Motor 2 Enable], ... ]
motorPins	= [[m1step, m1dir, m1en], [m2step, m2dir, m2en]]

# add or remove an element from each of these for additional motors
stepTime	= [0, 0]
oldTime		= [0, 0]
direction	= [0, 0]
position	= [0, 0]

# initialize pygame and joystick
pygame.init()
pygame.joystick.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()


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


# function to disable motor control
def disableMotors():
	for i in range(motorCount):
		GPIO.output(motorPins[i][2], 1)
# end disableMotors


# function to enable motor control
def enableMotors():
	for i in range(motorCount):
		GPIO.output(motorPins[i][2], 0)
# end enableMotors


# function to send the motors to the home position
def sendHome(speed):
	print("haha this doesnt actually do anything")
# end sendHome


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


loopRunning = 0
disabled = 0
while not loopRunning:
	# do this thing for some reason
	axis0 = joystick.get_axis(0)
	axis1 = -1 * joystick.get_axis(1)
	loopRunning = joystick.get_button(9)
	
	for event in pygame.event.get():
		if event.type == pygame.JOYBUTTONDOWN:
			if joystick.get_button(0):
				disabled = not disabled
				print("Motor state toggled.\nState: %d" % (1 - disabled))
	
	if disabled:
		disableMotors()
	else:
		enableMotors()
	
	m1Speed = remap(axis0, -1.0, 1.0, -0.75, 0.75)
	m2Speed = remap(axis1, -1.0, 1.0, -0.75, 0.75)
	
	setSpeed(motor1, m1Speed)
	setSpeed(motor2, m2Speed)
# clean up gpio pins
GPIO.cleanup()
