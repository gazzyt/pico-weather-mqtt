# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PEDANTIC_WARNINGS 1)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

set(PICO_SENSORS_FETCH_FROM_GIT 1)
include(pico_sensors_import.cmake)

project(uvsensor C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(uvsensor uvsensor.cpp ssd1306_i2c.cpp wifi_connection.cpp mqtt_client.cpp mqtt_publish_context.cpp 
    sensor_values.cpp power_status.cpp memory_status.cpp logger.cpp )

target_compile_definitions(uvsensor PRIVATE
    PICO_STACK_SIZE=4096
)

target_compile_options(uvsensor PRIVATE
$<$<CXX_COMPILER_ID:GNU>:-Wall -Wno-unused-function -Wno-psabi>
$<$<AND:$<CXX_COMPILER_ID:GNU>,$<BOOL:${PEDANTIC_WARNINGS}>>:-Wpedantic -Wextra>
)

pico_set_program_name(uvsensor "uvsensor")
pico_set_program_version(uvsensor "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(uvsensor 1)
pico_enable_stdio_usb(uvsensor 0)

# Add the standard library to the build
target_link_libraries(uvsensor
        pico_stdlib
        hardware_adc
        hardware_gpio
        hardware_watchdog)

# Add the standard include files to the build
target_include_directories(uvsensor PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(uvsensor 
        hardware_i2c
        hardware_timer
        sensor_VEML6070
        sensor_DHT22
        sensor_BMP085
        sensor_BME680
        sensor_SHT31
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip_mqtt
        )

pico_add_extra_outputs(uvsensor)

# Work out the openocd target from the PICO SDK configuration
if (PICO_PLATFORM STREQUAL "rp2040")
    set (OPENOCD_TARGET "rp2040")
elseif (PICO_PLATFORM STREQUAL "rp2350-arm-s")
    set (OPENOCD_TARGET "rp2350")
elseif (PICO_PLATFORM STREQUAL "rp2350-riscv")
    set (OPENOCD_TARGET "rp2350-riscv")
else()
    message (FATAL_ERROR "Unrecognised PICO_PLATFORM ${PICO_PLATFORM}")
endif()


add_custom_target(flash
    COMMAND sudo openocd -f ../openocd.cfg -f interface/cmsis-dap.cfg -f target/${OPENOCD_TARGET}.cfg -c "adapter speed 5000" -c "program uvsensor.elf verify reset exit"
)

add_dependencies(flash uvsensor)
