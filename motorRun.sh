#!/bin/bash

gpio -g mode 23 in    #Set GPIO 23 (pin 16) as input for near limit switch
gpio -g mode 24 in    #Set GPIO 24 (pin 18) as input for far limit switch
gpio -g mode 5 out    #Set GPIO 5 (pin 29) as output for relay 1 (+direction)
gpio -g mode 6 out    #Set GPIO 6 (pin 31) as output for relay 2 (-direction)
gpio -g mode 13 out   #Set GPIO 13 (pin 33) as output for relay 3 (+power)
gpio -g mode 19 out   #Set GPIO 19 (pin 35) as output for relay 4 (-power)
gpio -g mode 26 out   #Set GPIO 26 (pin 37) as output for charging relay
gpio -g write 5 0     #Set the motor direction to forward
gpio -g write 6 0     #Set the motor direction to forward
gpio -g write 13 0    #Set the motor running
gpio -g write 19 0    #Set the motor running
gpio -g write 26 1    #Turn off the charging relay
gpio -g wfi 24 both   #Watch the far limit switch for a toggle
gpio -g write 13 1    #Turn off the motor
gpio -g write 19 1    #Turn off the motor
sleep 1               #Wait for a second
gpio -g write 5 1     #Set the motor direction to reverse
gpio -g write 6 1     #Set the motor direction to reverse
gpio -g write 13 0    #Set the motor running
gpio -g write 19 0    #Set the motor running
gpio -g wfi 23 both   #Watch the near limit switch for a toggle
gpio -g write 13 1    #Turn off the motor
gpio -g write 19 1    #Turn off the motor
sleep 1               #Wait for a second
gpio -g write 26 0    #Turn on the charging relay
