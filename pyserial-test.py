import serial
import time

ser = serial.Serial('/dev/ttyACM1', 9600)
print 'Serial port warming up'
time.sleep(5)

ser.write('h')
print 'Wrote h'

#time.sleep(1)

ser.write('x')
print 'Wrote x'

ser.close()

#time.sleep(2)

#while True:
#    print ser.readline()
