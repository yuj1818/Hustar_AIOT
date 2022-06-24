import RPi.GPIO as GPIO
from time import sleep
import spidev

LED_1 = 4
LED_2 = 5
LED_3 = 14
LED_4 = 15


spi = spidev.SpiDev()
CDS_CHANNEL = 0

def initMcp3208():
    spi.open(0, 0) 
    spi.max_speed_hz = 1000000
    spi.mode = 3

def buildReadCommand(channel):

    startBit = 0x04
    singleEnded = 0x08

    configBit = [startBit | ((singleEnded | (channel & 0x07)) >> 2), (channel & 0x07) << 6, 0x00]
    
    return configBit

def processAdcValue(result):
    byte2 = (result[1] & 0x0F)
    return (byte2 << 8) | result[2]

def analogRead(channel):
    if(channel > 7) or (channel < 0):
        return -1

    r = spi.xfer2(buildReadCommand(channel))
    adc_out = processAdcValue(r)
    return adc_out

def controlMcp3208(channel):
    analogVal = analogRead(channel)
    return analogVal

def readSensor(channel):
    return controlMcp3208(channel)

def main():
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(LED_1, GPIO.OUT, initial=False)
    GPIO.setup(LED_2, GPIO.OUT, initial=False)
    GPIO.setup(LED_3, GPIO.OUT, initial=False)
    GPIO.setup(LED_4, GPIO.OUT, initial=False)
    initMcp3208()
    print("Setup pin as ouputs")

    try:
        while True:
            readVal = readSensor(CDS_CHANNEL)

            led_status = [GPIO.LOW, GPIO.LOW, GPIO.LOW, GPIO.LOW]

            if readVal > 4000:
                pass
            elif readVal >= 3000:
                led_status[0] = GPIO.HIGH
            elif readVal >= 2000:
                led_status[0] = GPIO.HIGH
                led_status[1] = GPIO.HIGH
            elif readVal >=1000:
                led_status[0] = GPIO.HIGH
                led_status[1] = GPIO.HIGH
                led_status[2] = GPIO.HIGH
            else:
                led_status[0] = GPIO.HIGH
                led_status[1] = GPIO.HIGH
                led_status[2] = GPIO.HIGH
                led_status[3] = GPIO.HIGH
            
            GPIO.output(LED_1, led_status[0])
            GPIO.output(LED_2, led_status[1])
            GPIO.output(LED_3, led_status[2])
            GPIO.output(LED_4, led_status[3])

            print(led_status, readVal)
            

    except KeyboardInterrupt:
        GPIO.cleaup()
        spi.close()

if __name__ == '__main__':
    main()