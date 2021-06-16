import sys
import time
from pymata4 import pymata4

DIGITAL_PIN = 6

def blink(my_board, pin):
    my_board.set_pin_mode_digital_output(pin)

    for x in range(4):
        print('on')
        my_board.digital_write(pin, 1)
        time.sleep(1)
        print('off')
        my_board.digital_write(pin, 0)
        time.sleep(1)
    # my_board.shutdown()

board = pymata4.Pymata4()


def servo(my_board, pin):

    my_board.set_pin_mode_servo(pin)
    for _ in range(5):
        for i in range(80):
            my_board.servo_write(pin, i*2)
            time.sleep(0.1)
            # my_board.servo_write(pin, 90)
            # time.sleep(0.1)
            # my_board.servo_write(pin, 180)

try:
    blink(board, DIGITAL_PIN)
    time.sleep(1)
    servo(board, 9)
except:
    board.shutdown()
    sys.exit(0)
