# -*- coding: utf-8 -*-
"""
Spyder Editor

This temporary script file is located here:
C:\Users\jmorris\.spyder2\.temp.py
"""


import serial
import time
ser = serial.Serial('COM6', 9600, timeout=0)
i = 0
while i<10:
 try:
  ser.write("5")
  print ser.readall()
  
  time.sleep(1)
  i+=1
 except ser.SerialTimeoutException:
  print('Data could not be read')
  time.sleep(1)
  ser.close()

ser.close()


    
    
    
    