import RPi.GPIO as GPIO
from time import sleep

led = [4, 4, 5, 15, 14]
status ={"ON": GPIO.HIGH, "OFF": GPIO.LOW}

def main():
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    for i in range(1,len(led)):
        GPIO.setup(led[i], GPIO.OUT, initial=True)
    print("main() program running...")

    try:
        while True:
            led_num = int(input("LED NUMBER: "))
            led_set = input("LED SET: ")
            GPIO.output(led[led_num], status[led_set])
    except KeyboardInterrupt:
        GPIO.cleanup()

if __name__ == '__main__':
    main()