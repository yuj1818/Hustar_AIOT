import RPi.GPIO as GPIO
from time import sleep
import GPIO_EX

ROW0_PIN = 0
ROW1_PIN = 1
ROW2_PIN = 2
ROW3_PIN = 3
COL0_PIN = 4
COL1_PIN = 5
COL2_PIN = 6

COL_NUM = 3
ROW_NUM = 4

led = [4, 4, 5, 15, 14]
status = {1: GPIO.LOW, 2: GPIO.LOW, 3: GPIO.LOW, 4: GPIO.LOW}

g_preData = 0

colTable = [COL0_PIN, COL1_PIN, COL2_PIN]
rowTable = [ROW0_PIN, ROW1_PIN, ROW2_PIN, ROW3_PIN]

def initKeypad():
    for i in range(0, COL_NUM):
        GPIO_EX.setup(colTable[i], GPIO_EX.IN)
    for i in range(0, ROW_NUM):
        GPIO_EX.setup(rowTable[i], GPIO_EX.OUT)

def selectRow(rowNum):
    for i in range(0, ROW_NUM):
        if rowNum == (i + 1):
            GPIO_EX.output(rowTable[i], GPIO_EX.HIGH)
            sleep(0.001)
        else:
            GPIO_EX.output(rowTable[i], GPIO_EX.LOW)
            sleep(0.001)
    return rowNum

def readCol():
    keypadstate = -1
    for i in range(0, COL_NUM):
        inputKey = GPIO_EX.input(colTable[i])
        if inputKey:
            keypadstate = keypadstate + (i + 2)
            sleep(0.5)

    return keypadstate

def readKeypad():
    global g_preData
    keyData = -1

    runningStep = selectRow(1)
    row1Data = readCol()
    selectRow(0)
    sleep(0.001)
    if(row1Data != -1):
        keyData = row1Data

    if runningStep == 1:
        if keyData == -1:
            runningStep = selectRow(2)
            row2Data = readCol()
            selectRow(0)
            sleep(0.001)
            if (row2Data != -1):
                keyData = row2Data + 3
    
    if runningStep == 2:
        if keyData == -1:
            runningStep = selectRow(3)
            row3Data = readCol()
            selectRow(0)
            sleep(0.001)
            if (row3Data != -1):
                keyData = row3Data + 6

    if runningStep == 3:
        if keyData == -1:
            runningStep = selectRow(4)
            row4Data = readCol()
            selectRow(0)
            sleep(0.001)
            if (row4Data != -1):
                if row4Data == 1:
                    keyData = "*"
                elif row4Data == 2:
                    keyData = 0
                elif row4Data == 3:
                    keyData = "#"

    sleep(0.1)

    if keyData == -1:
        return -1


    if g_preData == keyData:
        g_preData = -1
        return -1
    g_preData = keyData

    print("\r\nKeypad Data : " + str(keyData))

    return keyData

def initLed():
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    for i in range(1,len(led)):
        GPIO.setup(led[i], GPIO.OUT, initial=False)

def controlLed(keyData):
    if keyData == -1:
        return -1
    global status

    if keyData>4:
        for i in range(1, 5):
            GPIO.output(led[i], GPIO.LOW)
            status[i] = GPIO.LOW
        return 
    status[keyData] = ~status[keyData]
    GPIO.output(led[keyData], status[keyData])
    

def main():
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)

    initKeypad()
    initLed()
    print("setup keypad pin")
    try:
        while True:
            keyData = readKeypad()
            controlLed(keyData)

    except KeyboardInterrupt:
        GPIO.cleanup()

if __name__ == '__main__':
    main()