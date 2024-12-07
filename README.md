# Debugging
## Connections
Use another PICO flashed with the `debugprobe` firmware connected to the PI5 via USB.

Connect to the other PICO as follows:
| PICO | PICO Probe |
|--|--|
| SWDIO | GP 3 (PIN 5) |
| SWD GND | GND (PIN 38) |
| SWCLK	| GP 2 (PIN 4) |
| UART0 TX (PIN 1)	| UART1 RX (PIN 7) |
| UART0 RX (PIN 2) | UART1 TX (PIN 6) |
| VSYS (PIN 39) | VSYS (PIN 39) |

## Building
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j4
```

## Flashing
```
sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program uvsensor.elf verify reset exit"
 ```

 ## Viewing Log Messages
 ```
 sudo minicom -D /dev/ttyACM0 -b 115200
 ```

## Running the Debugger
Start openocd server:
```
sudo openocd -f ../openocd.cfg -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000"
```

Run gdb:
```
 gdb uvsensor.elf
 > target remote localhost:3333
 > monitor reset init
 > continue
 ```
