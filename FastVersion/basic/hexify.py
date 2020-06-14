from re import findall, sub
import jinja2
import os
import sys

ASM_PATH = "sjasmplus"
PLATFORMIO_PATH = "platformio"


def assemblify(filename, asm="sjasmplus"):
    
    os.system(f"{asm} {filename} --lst")

    

def hexify(binary:str, *others:str):

    with open(binary, 'rb') as f:
        hexcode = f.read().hex()
    for other in others:
        with open(other, 'rb') as f:
            hexcode += f.read().hex()
    formatted = sub("(?m)(..)(?!$)", "0x\\g<1>, ", hexcode) #Still need to add 0x to the last byte though!
    return f"{formatted[:-2]}0x{formatted[-2:]}" 
#findall("..", hexcode)

def jinjify(hexcode):

    jinja_env = jinja2.Environment(
        variable_start_string='/*%!!',
        variable_end_string='!!%*/',
        loader=jinja2.FileSystemLoader(os.path.abspath('.'))
    )
    
    template = jinja_env.get_template('program_template.txt')
    new_file = template.render(array=hexcode, size=hexcode.count(",")+1)
    #print(new_file)
    with open("../program.h", "w") as f:
        
        f.write(new_file)

def main():
    
    #print(f"{sys.argv[1]}")
    # assemblify(f"{sys.argv[1]}.asm")
    # jinjify(hexify(f"{sys.argv[1]}.bin"))


    # asm_path = "\"C:\\Program Files\\sjasmplus-1.14.3.win\\sjasmplus.exe\""

    assemblify("S210718.asm", ASM_PATH)
    assemblify("basic.asm", ASM_PATH)

    jinjify(hexify("S210718.bin", "basic.bin"))
    
    # platformio = "C:\\Users\\Volodya\\.platformio\\penv\\Scripts\\platformio.exe"
    os.chdir("../")
    os.system(f"{PLATFORMIO_PATH} run")
    os.system(f"{PLATFORMIO_PATH} run --target upload")
    os.system(f"{PLATFORMIO_PATH} device monitor")

    
if __name__ == "__main__":
    
    # result = hexify("simple.bin")
    # print(result)
    # jinjify(result)
    # os.chdir(os.path.abspath(sys.argv[2] if len(sys.argv) >= 3 else "."))
    main()
