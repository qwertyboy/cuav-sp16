#!/usr/bin/python

import time
import smbus
import pygame
import funcs as f

# define some constants
TEENSYADDR = 0x10
PWMACMD = 0x01
DIRACMD = 0x02
PWMBCMD = 0x06
DIRBCMD = 0x07
SLPCMD = 0x0C

# init i2c and pygame
bus = smbus.SMBus(1)
pygame.init()
pygame.joystick.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()

while 1:
	# process events, ignore them pretty much
	pygame.event.pump()
	
	# get axis readings and map them
	axis0 = joystick.get_axis(0);
	axis1 = -1 * joystick.get_axis(1);
	
	# set the direction pins
	if(axis0 < 0.0):
		bus.write_word_data(TEENSYADDR, DIRACMD, 1)
	else:
		bus.write_word_data(TEENSYADDR, DIRACMD, 0)
	
	if(axis1 < 0.0):
		bus.write_word_data(TEENSYADDR, DIRBCMD, 1)
	else:
		bus.write_word_data(TEENSYADDR, DIRBCMD, 0)
	
	# map the pwm 
	pwm1 = int(f.remap(abs(axis0), 0.0, 1.0, 0, 1024))
	pwm2 = int(f.remap(abs(axis1), 0.0, 1.0, 0, 1024))
	
	# update pwm on the teensy
	bus.write_word_data(TEENSYADDR, PWMACMD, pwm1)
	bus.write_word_data(TEENSYADDR, PWMBCMD, pwm2)
	
	time.sleep(0.01)

