import os


def main():
    os.chdir("../")
    os.system("platformio run")
    os.system("platformio run --target upload")
    os.system("platformio device monitor")
    
if __name__ == "__main__":
    main()
