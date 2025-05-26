#######################################################################
# 01 - Serial Test
#
# >>>> Copy to board:
#      "mpremote connect <poer> fs cp ./01-Serial-Test.py :main.py"
# >>>> Then, start a serial repl shell:
#      "mpremote connect <port> repl"
#######################################################################

from machine import Pin
import time

LED_GREEN = Pin(16, Pin.OUT)

def main():
    
    while True:
        
        LED_GREEN.value(not LED_GREEN.value())
        
        print('MicroPython install OK! :-D')
        
        time.sleep(1)

main()
