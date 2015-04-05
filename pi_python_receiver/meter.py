#!/usr/bin/python
 
import spidev
import time
from time import strftime, gmtime
import decimal
import RPi.GPIO as GPIO
import mysql.connector

#The GPIO.BOARD option specifies that you are referring to the pins by the number of the pin the plug (i.e. - the numbers printed on the board 
#GPIO.setmode(GPIO.BOARD);
#The GPIO.BCM option means that you are referring to the pins by the "Broadcom SOC channel" number, these are the numbers after GPIO  
#see http://raspberrypi.stackexchange.com/questions/12966/what-is-the-difference-between-board-and-bcm-for-gpio-pin-numbering 
#GPIO.setmode(GPIO.BCM);

add_record = "INSERT INTO record (record_timestamp, record_address, record_desc, record_measure) VALUES (%s, %s, %s, %s)"

totalGallons = 0.0;
prevOutput = 0;
prevTotalGallons = 0.0;
firstLoop = True;

print("Setup started.");
#GPIO.setup(9, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
#GPIO.add_event_detect(9,GPIO.RISING)
print("Setup ended.");

spi = spidev.SpiDev()
spi.open(0, 0)
 
def readadc(adcnum):
# read SPI data from MCP3008 chip, 8 possible adc's (0 thru 7)
    if adcnum > 7 or adcnum < 0:
	print("ADC point out of bounds!");
        return -1

    r = spi.xfer2([1, 8 + adcnum << 4, 0])
    adcout = ((r[1] & 3) << 8) + r[2]
    return adcout

while True:
    try:
	output = readadc(0)

	if(firstLoop):
	  prevOutput = output;
	  firstLoop = False;		
	
	if(((output > 500) and (prevOutput <=500)) or ((output <= 500) and (prevOutput > 500))):
	  try:
	  	cnx = mysql.connector.connect(user='controller', password='c0ntr0113r', host='127.0.0.1', database='controller')
		cursor = cnx.cursor();
		totalGallons += .375;
		data_record = (strftime("%Y-%m-%d %H:%M:%S"),'1003 Rougemont, Charlottesville, VA 22902','Basement',totalGallons);
		cursor.execute(add_record, data_record);
		cnx.commit();
		cursor.close();
		cnx.close();
	  except mysql.connector.Error as err:
		if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
			print("Username or password error!");
		elif err.errno == errorcode.ER_BAD_DB_ERROR:
			print("Database does not exist!");
		else:
			print(err);
	  else:
		cnx.close();
	prevOutput = output;
	prevTotalGallons = totalGallons;
	#print("Total Gallons %f" % totalGallons);
	time.sleep(.05);
    except KeyboardInterrupt:
	GPIO.cleanup()
