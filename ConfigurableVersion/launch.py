import argparse
from uart import SerialFactory
import re
import platform

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Z80-PC cli launcher and communicator")
    
    serial_group = parser.add_argument_group(title="Serial commands", description="Commands that interact with the Z80 system via serial port iterface. To inialize a serial interface, use the -s option. Please note that only -s and one of the following command arguments can be specified at a time")
    serial_group.add_argument("-s", "--serial", nargs="?", help="Initialize the serial port interface. Must be present if any other serial commands are specified", const="auto")
    serial_group.add_argument("-sl", "--seriallaunch", action="store_true", help="launch the interactive serial port communication utility")
    serial_group.add_argument("-sw", "--serialwrite", nargs="*", help="Write the contents of a file F into RAM, starting from address ADDR. Supply variable sequence of F ADDR pairs, e.g. -sw example_1.txt 8000 example_2.txt 9000. Optionally, if only one file is specified, address 0 will be implicitly assumed. The files must not interlap in memory")


    arguments = parser.parse_args()
    serial_args = [arguments.seriallaunch, arguments.serialwrite]

    if len(serial_args) - (serial_args.count(None) + serial_args.count(False)) > 1:

        print(serial_args)
        parser.error("Multiple serial options specified simulataneously.")

    if any(serial_args):

        if not(arguments.serial):

            parser.error("Serial interface not initialized")

        s = None #declare the handle
        if arguments.serial == "auto":
            s = SerialFactory().default()
        elif (platform.system() == "Linux" and re.match(r"(?i)\s*/?dev/\w+", arguments.serial)) or (platform.system() == "Windows" and re.match(r"(?i)COM\d", arguments.serial)):
            s = SerialFactory().from_port(arguments.serial)
        else:
            s = SerialFactory().from_device_desc(arguments.serial)
        
        if arguments.seriallaunch:

            s.run()

        elif arguments.serialwrite:

            print(arguments.serialwrite)
            if len(arguments.serialwrite) == 1:

                s.open()
                s.handle_write(arguments.serialwrite[0], 0)
                s.close()

            else:

                if len(arguments.serialwrite) % 2 != 0:

                    parser.error("Invalid input format: not enough addresses")
                
                s.open()
                for i in range(len(arguments.serialwrite), -1, 2):

                    s.handle_write(arguments.serialwrite[i], int(arguments.serialwrite[i+1], base=(16 if arguments.serialwrite[i+1].startswith("0x") else 10)))
                s.close()