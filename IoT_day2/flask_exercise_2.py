from time import sleep
import board
import adafruit_dht
import RPi.GPIO as GPIO

from flask import Flask
app = Flask(__name__)

FAN_PIN1 = 18
FAN_PIN2 = 27

dhtDevice = adafruit_dht.DHT11(board.D17)

def fanOn():
    GPIO.output(FAN_PIN1, 1)
    GPIO.output(FAN_PIN2, 0)


def fanOff():
    GPIO.output(FAN_PIN1, 0)
    GPIO.output(FAN_PIN2, 0)

@app.route('/')
def hello():
    return "hello world"

@app.route('/fan/<time>')
def fanonoff(time):   
    print("FAN Turn on")
    fanOn()
    sleep(int(time))
    fanOff()
    return "FAN on"
        

if __name__ == "__main__":
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(FAN_PIN1, GPIO.OUT, initial=False)
    GPIO.setup(FAN_PIN2, GPIO.OUT, initial=False)
    app.run(host='0.0.0.0', port=5000, debug=True)
