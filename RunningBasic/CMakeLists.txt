cmake_minimum_required(VERSION 3.15)

set(CMAKE_SOURCE_DIR "./src")
set(CMAKE_ARDUINO_LIBRARY_DIR "../ZPC_lib/ZPC_funcs")

set(CMAKE_TOOLCHAIN_FILE "../cmake/ArduinoToolchain.cmake")

set(ARDUINO_CXX_FLAGS "${ARDUINO_CXX_FLAGS}")

set(ARDUINO_C_FLAGS      "-ffunction-sections -fdata-sections")
set(ARDUINO_CXX_FLAGS    "${ARDUINO_C_FLAGS} -fno-exceptions")
set(ARDUINO_LINKER_FLAGS "-Wl,--gc-sections")

project(WorkingWithRAM C CXX ASM)

set(PY_SCRIPT "hexify.py")
set(ASM_FILE "simple")
# exec_program("python ${CMAKE_SOURCE_DIR}/${PY_SCRIPT}"
#                     ARGS "${ASM_FILE}")           
                    
exec_program("python3" ARGS "${CMAKE_SOURCE_DIR}/${PY_SCRIPT} ${ASM_FILE} ${CMAKE_SOURCE_DIR}")  

register_hardware_platform(${ARDUINO_SDK_PATH}/hardware/arduino/avr)
#print_board_settings(mega2560)
set(ARDUINO_DEFAULT_BOARD "mega")
set(mega.build.mcu "atmega2560")
set(ARDUINO_DEFAULT_PORT "/dev/ttyACM0")
set(ARDUINO_DEFAULT_SERIAL "picocom @SERIAL_PORT@ -b 9600 -l")
set(ARDUINO_DEFAULT_PROGRAMMER "avrisp")


message(${ARDUINO_AVRDUDE_CONFIG_PATH})
message(${ARDUINO_AVRDUDE_PROGRAM})
# generate_arduino_library(ZPC_lib 
#             SRCS "${CMAKE_ARDUINO_LIBRARY_DIR}/ZPC_mem.cpp" "${CMAKE_ARDUINO_LIBRARY_DIR}/ZPC_setup.cpp"
#             HDRS "${CMAKE_ARDUINO_LIBRARY_DIR}/ZPC_lib.h"
#             )

link_directories(${CMAKE_ARDUINO_LIBRARY_DIR})

# add_custom_command(TARGET memory PRE_BUILD
#                     COMMAND "python ${PY_SCRIPT}" "${ASM_FILE}"
#                     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")


generate_arduino_firmware(memory SRCS "${CMAKE_SOURCE_DIR}/rendered_main.cpp" AFLAGS "-b 9600")

# generate_arduino_firmware(memory 
#             SRCS "${CMAKE_SOURCE_DIR}/main.cpp"
#             LIBS ZPC_lib)
