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
    my_board.shutdown()

board = pymata4.Pymata4()

try:
    blink(board, DIGITAL_PIN)
except:
    board.shutdown()
    sys.exit(0)
