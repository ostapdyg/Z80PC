import serial
from serial.tools import list_ports
import re
from enum import IntEnum
# from pynput import keyboard
# from pynput.keyboard import Key, Controller

class Commands(IntEnum):

    WRITE = 0xba
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

    def __init__(self, handle:serial.Serial, baudrate, dtr=False):

        self.__serial_handle = handle
        self.set_baudrate(baud=baudrate)
        self.set_dtr(dtr)

    def __del__(self):

        self.close()

    def open(self):

        if not self.__serial_handle.is_open:

            self.__serial_handle.open()

    def close(self):

        if self.__serial_handle.is_open:

            self.__serial_handle.close()

    def set_baudrate(self, baud):

        self.__serial_handle.baudrate = baud

    def set_dtr(self, dtr): #May not work on some systems

        self.__serial_handle.dtr = dtr

    def handle_write(self, fname, start_address):

        f = open(fname, 'rb')
        f.seek(0,2)
        fsize = f.tell()
        f.seek(0,0)
        fcontent = f.read()
        print(f"file size: {fsize}, \nfile content{fcontent}")
        UPPER_BYTE_MASK = 0xff00
        LOWER_BYTE_MASK = 0x00ff
        self.__serial_handle.write(bytes([Commands.WRITE, 
                                          (start_address & UPPER_BYTE_MASK) >> 8, start_address & LOWER_BYTE_MASK, 
                                          (fsize & UPPER_BYTE_MASK) >> 8, fsize & LOWER_BYTE_MASK])
                                          + fcontent)

    def handle_send(self, data):
        
        self.__serial_handle.write(data) #TODO: check whether len == 1

    def handle_continue(self):

        self.__serial_handle.write(bytes([Commands.CONTINUE]))

    def handle_mode_change(self, mode):
        
        self.__serial_handle.write(bytes([Commands.MODE_0 + mode])) #MODE_1 and MODE_2 are just offsets from MODE_0

    def run(self):

        self.open()
        while True:
            
            commands = re.split("&&|\n", input("(ZPC) ")) #fetch user input and split by commands
            for cmd in commands:

                cmd = cmd.strip().split()
                cmd_name = cmd[0]
                if cmd_name in ["stop", "quit", "close", "exit"]: # if command is STOP

                    break
                
                elif cmd_name in ["dump", "write", "w", "program"]:

                    self.handle_write(cmd[1], cmd[2])

                elif cmd_name in ["send", "s", "transmit", "tx", "enter"]:

                    self.handle_send(cmd[1])

                elif cmd_name in ["continue", "c", "next"]:

                    self.handle_continue()
                
                elif cmd_name in ["mode", "m", "change", "clockmode"]:

                    self.handle_mode_change(cmd[1])

                # elif cmd_name in ["interact", "i", "keyboard"]: #Warning: experimental
                    
                    # #TODO: rewrite this
                    # with keyboard.Listener(on_press=lambda key: False if key == Key.esc else self.__serial_handle.write(key), 
                    #                        suppress=True) as listener:

                    #     listener.join()

                else:

                    print("Invalid command.")
        self.close()