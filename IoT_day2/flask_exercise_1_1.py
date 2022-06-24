from time import sleep
import RPi.GPIO as GPIO

from flask import Flask
app = Flask(__name__)

LED_1 = 4
LED_2 = 5
LED_3 = 14
LED_4 = 15

led_status = False

def ledOffAll():
    GPIO.output(LED_1, 0)
    GPIO.output(LED_2, 0)
    GPIO.output(LED_3, 0)
    GPIO.output(LED_4, 0)

def ledOnAll():
    GPIO.output(LED_1, 1)
    GPIO.output(LED_2, 1)
    GPIO.output(LED_3, 1)
    GPIO.output(LED_4, 1)

def partyMode():
    while(1):
        ledOnAll()
        sleep(0.5)
        ledOffAll()
        sleep(0.5)

@app.route('/')
def hello():
    return "hello world"

@app.route('/led/<onoff>')
def ledonoff(onoff):
    global led_status
    if onoff == "on":
        print("LED Turn on")
        ledOnAll()
        led_status = True
        return "LED on"
    elif onoff == "off":
        print("LED Turn off")
        ledOffAll()
        led_status = False
        return "LED off"
    elif onoff == "party":
        partyMode()

if __name__ == "__main__":
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(LED_1, GPIO.OUT, initial=False)
    GPIO.setup(LED_2, GPIO.OUT, initial=False)
    GPIO.setup(LED_3, GPIO.OUT, initial=False)
    GPIO.setup(LED_4, GPIO.OUT, initial=False)
    app.run(host='0.0.0.0', port=5000, debug=True)
