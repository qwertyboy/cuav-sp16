import pygame
import time

pygame.init()

# initialize the joystick
pygame.joystick.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()
axes = joystick.get_numaxes()

while True:
	for i in range(axes):
		# print out the axis values
		axis = joystick.get_axis(i)
		#s = 'Axis i: ' + axis
		print 'Axis %d:\t%6.4f' % (i, axis)
	#time.sleep(0.25)

