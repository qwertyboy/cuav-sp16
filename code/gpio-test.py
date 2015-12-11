try:
	import RPi.GPIO as GPIO
except RuntimeError:
	print("Error importing RPi.GPIO! Make sure module is installed and try running as root.")

import time

# use GPIO header pin numbers
GPIO.setmode(GPIO.BOARD)

# set up pins as outputs
ledPin = 18
GPIO.setup(ledPin, GPIO.OUT)

timeout = 0.5
currentTime = 0
previousTime = 0

ledState = 0

while 1:
	#GPIO.output(ledPin, ledState)
	
	currentTime = time.time()
	if (currentTime - previousTime) > timeout:
		previousTime = currentTime
		
		ledState = not ledState
		GPIO.output(ledPin, ledState)
		print("Changed")


# clean up gpio pins
GPIO.cleanup()
