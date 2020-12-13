
# Read SHT31

A simple command line tool for Raspberry-Pi to read a SHT31 sensor over the I2C bus.

## Usage

There are a number of arguments you can use:

```
Usage: read_sht31 [-h][-v]
 -h|--help   Display this help.
 -s          Get the sensor status.
 -t1 -t0     Enable/disable heater.
 -a0 -a1     Select the chip address. 0 is the default.
 -b0 -b1     Select the bus. 1 is the default.
 -d          Show debugging messages.
```

If you call the command, you will get JSON output:

```
$ read_sht32
{ "temperature_celsius": 24.8, "relative_humidity": 28.4 }
```

The idea is to call this command from your script and parse the returned JSON output.

## How to Compile and Install

In order to compile and install the tool on your Raspberry-Pi, you need to install the compiler, CMake and the I2C-Tools first:

```
sudo apt install gcc cmake i2c-tools
```

Download the files of this git repository to a sub directory in your home directory. In this example, I downloaded the files to the directory `/home/pi/read_sht31`.

Now create a separate build directory. In this example, I create the directory `/home/pi/build_read_sht31`.

Next switch to the build directory, execute `cmake` to create the build script. Build the executable with `make` and install the executable in your system with `sudo make install`.

```
cd build_read_sht31
cmake ../read_sht31
make
sudo make install
```

## License (GPL v3)

Copyright (c) 2020 by Lucky Resistor.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

