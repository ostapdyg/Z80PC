import os
ASM_PATH = "sjasmplus"
PLATFORMIO_PATH = "platformio"



def main():
    # os.chdir("../")
    # os.system("platformio run")
    os.system(f"{PLATFORMIO_PATH} run")
    os.system(f"{PLATFORMIO_PATH} run --target upload")
    os.system(f"{PLATFORMIO_PATH} device monitor")

if __name__ == "__main__":
    main()
