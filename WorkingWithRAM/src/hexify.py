from re import findall, sub
import jinja2
import os
import sys

def assemblify(filename, asm="sjasmplus"):
    
    os.system(f"{asm} {filename}")
    

def hexify(binary):

    with open(binary, 'rb') as f:
        
        hexcode = f.read().hex()
    
    formatted = sub("(?m)(..)(?!$)", "0x\g<1>, ", hexcode) #Still need to add 0x to the last byte though!
    return f"{formatted[:-2]}0x{formatted[-2:]}" 
#findall("..", hexcode)

def jinjify(hexcode):

    jinja_env = jinja2.Environment(
        variable_start_string='%!!',
        variable_end_string='!!%',
        loader=jinja2.FileSystemLoader(os.path.abspath('.'))
    )
    
    template = jinja_env.get_template('main.cpp')
    new_file = template.render(array=hexcode)
    #print(new_file)
    with open("rendered_main.cpp", "w") as f:
        
        f.write(new_file)

def main():
    
    #print(f"{sys.argv[1]}")
    assemblify(f"{sys.argv[1]}.asm")
    jinjify(hexify(f"{sys.argv[1]}.bin"))
    
    
if __name__ == "__main__":
    
    # result = hexify("simple.bin")
    # print(result)
    # jinjify(result)
    os.chdir(os.path.abspath(sys.argv[2] if len(sys.argv) >= 3 else "."))
    main()