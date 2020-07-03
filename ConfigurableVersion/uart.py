import serial
from serial.tools import list_ports
import re
from enum import Enum
from pynput import keyboard
from pynput.keyboard import Key, Controller

class Commands(Enum):

    WRITE = 0xb3
    CONTINUE = 0x81
    STOP = 0x83
    MODE_0 = 0x84
    MODE_1 = 0x85
    MODE_2 = 0x86


class SerialFactory():
    
    def __init__(self, baudrate=9600):

        self.baudrate = baudrate
    
    def default(self):
        
        ports = list_ports.comports()  #get all ports
        if (len(ports) != 1):
            raise serial.SerialException("Port selection is abiguous") # Either there are no suitable devices connected or more than one. Please explicitly specify the serial port or use other indetifiers.
        
        s = serial.serial_for_url(ports[0].device)  #create serial handle from device name e.g. /dev/ttyUSB0 or COM1
        return Z80PCSerial(s, self.baudrate)


    def from_port(self, port):
        
        s = serial.Serial()   #create, but do not open the port
        s.port = port         #set the port
        return Z80PCSerial(s, self.baudrate)


    def from_device_desc(self, desc):

        s = serial.serial_for_url(f"hwgrep://(?i).*desc:.*{desc}.*") #match regex against all device descriptions
        return Z80PCSerial(s, self.baudrate)

class Z80PCSerial():

    def __init__(self, handle, baudrate):

        self.__serial_handle = handle
        self.set_baudrate(baud=baudrate)

    def set_baudrate(self, baud):

        self.__serial_handle.baudrate = baud


    def run(self):

        with self.__serial_handle as s:

            while True:
                
                commands = re.split("&&|\n", input("(ZPC) ")) #fetch user input and split by commands
                for cmd in commands:

                    cmd = cmd.strip().split()
                    cmd_name = cmd[0]
                    if cmd_name in ["stop", "quit", "close", "exit"]: # if command is STOP

                        break
                    
                    elif cmd_name in ["dump", "write", "w", "program"]:

                        f = open(cmd[1], 'rb', encoding='ascii')
                        f.seek(0,2)
                        fsize = f.tell()
                        f.seek(0,0)
                        fcontent = f.read()
                        self.__serial_handle.write(bytes([Commands.WRITE, cmd[2], fsize]) + fcontent)

                    elif cmd_name in ["send", "s", "transmit", "tx", "enter"]:

                        self.__serial_handle.write(cmd[1]) #TODO: check whether len == 1

                    elif cmd_name in ["continue", "c", "next"]:

                        self.__serial_handle.write(bytes([Commands.CONTINUE]))
                    
                    elif cmd_name in ["mode", "m", "change", "clockmode"]:

                        self.__serial_handle.write(bytes([Commands.MODE_0 + cmd[1]])) #MODE_1 and MODE_2 are just offsets from MODE_0

                    elif cmd_name in ["interact", "i", "keyboard"]:
                        
                        
                        with keyboard.Listener(on_press=lambda key: False if key == Key.esc else self.__serial_handle.write(key), 
                                               suppress=True) as listener:

                            listener.join()

                    else:

                        print("Invalid command.")
