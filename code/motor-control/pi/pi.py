#!/usr/bin/python

import time
import smbus
import pygame
import pi_funcs as f

# define some constants
TEENSYADDR = 0x10
PWM1CMD = 0x01
PWM2CMD = 0x02

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
	axis1 = joystick.get_axis(1);
	pwm1 = int(f.remap(axis0, -1.0, 1.0, 0, 1024))
	pwm2 = int(f.remap(axis1, 1.0, -1.0, 0, 1024))
	
	# update pwm on the teensy
	bus.write_word_data(TEENSYADDR, PWM1CMD, pwm1)
	bus.write_word_data(TEENSYADDR, PWM2CMD, pwm2)
	
	time.sleep(0.01)

