# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
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

add_executable(uvsensor uvsensor.cpp ssd1306_i2c.cpp wifi_connection.cpp mqtt_client.cpp mqtt_publish_context.cpp sensor_values.cpp )

pico_set_program_name(uvsensor "uvsensor")
pico_set_program_version(uvsensor "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(uvsensor 1)
pico_enable_stdio_usb(uvsensor 0)

# Add the standard library to the build
target_link_libraries(uvsensor
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(uvsensor PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(uvsensor 
        hardware_i2c
        sensor_VEML6070
        sensor_DHT22
        sensor_BMP085
        pico_cyw43_arch_lwip_poll
        pico_lwip_mqtt
        )

pico_add_extra_outputs(uvsensor)

