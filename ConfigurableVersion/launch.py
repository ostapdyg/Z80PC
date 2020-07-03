import argparse
from uart import SerialFactory

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Z80-PC cli launcher and communicator")
    parser.add_argument("-s", "--serial", help="launch the serial port communication utility.", action="store_true")
    # parser.add_argument("-m", "--miniterm", help="Launch miniterm.") 
    
    arguments = parser.parse_args()
    if arguments.serial:

        s = SerialFactory().default()
        s.run()

