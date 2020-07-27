from re import findall, sub
import jinja2
import os
import sys


ASM_PATH = "sjasmplus"
PLATFORMIO_PATH = "platformio"

def assemblify(filename, asm="sjasmplus"):
    
    os.system(f"{asm} {filename} --lst")

    

def hexify(binary, outfile="program.h"):

    with open(binary, 'rb') as f:
        
        hexcode = f.read()
        # print(hexcode)
        fromatted_hex = ", ".join(map(hex, hexcode))
        # print(fromatted_hex)
        header = f"""
#ifndef Z80ASM_PROG
#define Z80ASM_PROG

#define PROG_SIZE {len(hexcode)}

uint8_t program_CUSTOM[] = {{
{fromatted_hex}
}};

#endif
"""
        with open(outfile, 'w') as of:

            of.write(header)
        
    # formatted = sub("(?m)(..)(?!$)", "0x\\g<1>, ", hexcode) #Still need to add 0x to the last byte though!
    # return f"{formatted[:-2]}0x{formatted[-2:]}" 
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
    assemblify(f"{sys.argv[1]}.asm", ASM_PATH)
    # jinjify(hexify(f"{sys.argv[1]}.bin"))
    hexify(f"{sys.argv[1]}.bin")
    # os.chdir("../")
    # os.system(f"{PLATFORMIO_PATH} run")
    os.system(f"{PLATFORMIO_PATH} run --target upload")
    os.system(f"{PLATFORMIO_PATH} device monitor")

    
if __name__ == "__main__":
    
    # result = hexify("simple.bin")
    # print(result)
    # jinjify(result)
    # os.chdir(os.path.abspath(sys.argv[2] if len(sys.argv) >= 3 else "."))
    main()
