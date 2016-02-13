import time
import pygame
pygame.init()
pygame.joystick.init()

joystickCount = pygame.joystick.get_count()
print("Number of joysticks: %d" % joystickCount)

for i in range(joystickCount):
	joystick = pygame.joystick.Joystick(i)
	joystick.init()
	
	name = joystick.get_name()
	print("Joystick name: %s" % name)
	
	axes = joystick.get_numaxes()
        print("Number of axes: %d" % axes)
        
	pygame.event.pump()
        for j in range(axes):
            axis = joystick.get_axis(j)
            print("Axis value: %6.3f" % axis)

pygame.quit()
