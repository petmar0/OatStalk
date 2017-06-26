#!/bin/bash

gpio -g 18 pwm    #Set pin 18 to pwm mode
gpio pwm-ms       #Set up pwm mode
gpio pwmc 192     #Set up pwm clock 1
gpio pwmr 2000    #Set up pwm clock 2
gpio -g pwm 200   #Push servo all the way to the right (2.00 msec)
sleep 5           #Stay in this state for 5 sec (power on t>4 sec)
gpio -g pwm 150   #Push servo back to midpoint (1.50 msec)
sleep 1           #Stay in this state for 1 sec
gpio -g pwm 200   #Push servo all the way to the right (2.00 msec)
sleep 1           #Stay in this state for 1 sec (record)
gpio -g pwm 150   #Push servo back to midpoint (1.5 msec)
