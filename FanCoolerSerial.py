# Python Script for Fan Controller
import time                    # Calling time to allow delays to be used
import serial
import subprocess              # Calling subprocess to get the CPU temperature

ser = serial.Serial("/dev/ttyUSB0", 115200)
minTemp = 20                   # Temperature and speed range variables, edit these to adjust max and min temperatures and speeds
maxTemp = 60
minSpeed = 0
maxSpeed = 100

def get_temp():                             # Function to read in the CPU temperature and return it as a float in degrees celcius
    output = subprocess.run(['vcgencmd', 'measure_temp'], capture_output=True)
    temp_str = output.stdout.decode()
    try:
        return float(temp_str.split('=')[1].split('\'')[0])
    except (IndexError, ValueError):
        raise RuntimeError('Could not get temperature')
    
def renormalize(n, range1, range2):         # Function to scale the read temperature to the fan speed range
    delta1 = range1[1] - range1[0]
    delta2 = range2[1] - range2[0]
    return (delta2 * (n - range1[0]) / delta1) + range2[0]

while 1:                                    # Execute loop forever
    temp = get_temp()                       # Get the current CPU temperature
    print("Get Temp: " + str(get_temp()))
    if temp < minTemp:                      # Constrain temperature to set range limits
        temp = minTemp
    elif temp > maxTemp:
        temp = maxTemp
    tempnorm = int(renormalize(temp, [minTemp, maxTemp], [minSpeed, maxSpeed]))
  
  # Set fan duty based on temperature, from minSpeed to maxSpeed
    command = str(tempnorm)
    try:
         ser.write(command.encode())
         f = open('fancooler.log', 'w')
         f.write(str(temp) + ' C ' + str(tempnorm) + '% \n')
         f.close()
         print(tempnorm)
    except:
         f = open('fancooler.err', 'w')
         f.write('Serial Port Error\n')
         f.close()
         print("Serial Port Error\n")
    
    time.sleep(5)                           # Sleep for 5 seconds
