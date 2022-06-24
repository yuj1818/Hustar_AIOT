from time import sleep
import RPi.GPIO as GPIO

from flask import Flask
app = Flask(__name__)

LED_1 = 4
LED_2 = 5
LED_3 = 15
LED_4 = 14
led = [4, 4, 5, 15, 14]

@app.route('/')
def hello():
    return "hello world"

@app.route('/led/<num>/<onoff>')
def led1onoff(num, onoff):
    if onoff == "on":
        print("LED Turn on")
        GPIO.output(led[int(num)], 1)
        return "LED on"
    elif onoff == "off":
        print("LED Turn off")
        GPIO.output(led[int(num)], 0)
        return "LED off"
        
if __name__ == "__main__":
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(LED_1, GPIO.OUT, initial=False)
    GPIO.setup(LED_2, GPIO.OUT, initial=False)
    GPIO.setup(LED_3, GPIO.OUT, initial=False)
    GPIO.setup(LED_4, GPIO.OUT, initial=False)
    app.run(host='0.0.0.0', port=5000, debug=True)
