cmake_minimum_required(VERSION 3.15)

set(CMAKE_SOURCE_DIR "./src")
set(CMAKE_ARDUINO_LIBRARY_DIR "../ZPC_lib")

set(CMAKE_TOOLCHAIN_FILE "../cmake/ArduinoToolchain.cmake")

set(ARDUINO_CXX_FLAGS "${ARDUINO_CXX_FLAGS}")

set(ARDUINO_C_FLAGS      "-ffunction-sections -fdata-sections")
set(ARDUINO_CXX_FLAGS    "${ARDUINO_C_FLAGS} -fno-exceptions")
set(ARDUINO_LINKER_FLAGS "-Wl,--gc-sections")

project(WorkingWithRAM C CXX ASM)


register_hardware_platform(${ARDUINO_SDK_PATH}/hardware/arduino/avr)
#print_board_settings(mega2560)
set(ARDUINO_DEFAULT_BOARD "mega")
set(mega.build.mcu "atmega2560")
set(memory_PORT "/dev/ttyACM0")
set(memory_SERIAL "picocom @SERIAL_PORT@ -b 9600 -l")

generate_arduino_library(ZPC_lib 
            SRCS "${CMAKE_ARDUINO_LIBRARY_DIR}/ZPC_mem.cpp" "${CMAKE_ARDUINO_LIBRARY_DIR}/ZPC_setup.cpp"
            HDRS "${CMAKE_ARDUINO_LIBRARY_DIR}/ZPC_lib.h"
            )
#link_directories(${CMAKE_ARDUINO_LIBRARY_DIR})

generate_arduino_firmware(memory 
            SRCS "${CMAKE_SOURCE_DIR}/main.cpp"
            LIBS ZPC_lib)
