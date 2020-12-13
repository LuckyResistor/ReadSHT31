//
// (c)2020 by Lucky Resistor. See LICENSE for details.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#include "I2CBus.hpp"


#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cstring>
#include <chrono>
#include <thread>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>


namespace lr {


std::string I2CBus::_devicePathBase = "/dev/i2c-";


I2CBus::I2CBus(uint8_t chipAddress, int busId)
:
    _chipAddress(chipAddress),
    _busId(busId),
    _isOpen(false),
    _debugging(false),
    _i2cFd(0)
{
}


I2CBus::~I2CBus()
{
    if (isOpen()) {
        closeBus();
    }
}


void I2CBus::setDebugging(bool enabled)
{
    _debugging = enabled;
}


I2CBus::Status I2CBus::openBus()
{
    if (_debugging) {
        std::cout << "# Open the bus." << std::endl;
    }
    const auto devicePath = getDevicePath();
    _i2cFd = open(devicePath.c_str(), O_RDWR);
    if (_i2cFd < 0) {
        std::cerr << "Failed to open the I2C bus device. Path: " << devicePath << std::endl;
        return Status::Error;
    }
    if (ioctl(_i2cFd, I2C_SLAVE, _chipAddress) < 0) {
        std::cerr << "Failed to configure the I2C bus device." << std::endl;
        writeIoError();
        return Status::Error;
    }
    _isOpen = true;
    return Status::Success;
}


I2CBus::Status I2CBus::closeBus()
{
    if (_isOpen) {
        if (_debugging) {
            std::cout << "# Close the bus." << std::endl;
        }
        close(_i2cFd);
        _i2cFd = 0;
        _isOpen = false;
    }
    return Status::Success;
}


bool I2CBus::isOpen() const
{
    return _isOpen;
}


I2CBus::Status I2CBus::readData(uint8_t *data, int size)
{
    if (!isOpen()) {
        std::cerr << "Call to readData() in closed state." << std::endl;
        return Status::Error;
    }
    if (read(_i2cFd, data, size) != size) {
        std::cerr << "Failed to read from the bus." << std::endl;
        writeIoError();
        return Status::Error;
    }
    if (_debugging) {
        std::cout << "# Read " << size << " bytes from the bus: ";
        for (int i = 0; i < size; ++i) {
            if (i != 0) {
                std::cout << ", ";
            }
            std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(data[i]);
        }
        std::cout << std::endl;
    }
    return Status::Success;
}


I2CBus::Status I2CBus::writeData(const uint8_t *data, int size)
{
    if (!isOpen()) {
        std::cerr << "Call to writeData() in closed state." << std::endl;
        return Status::Error;
    }
    if (_debugging) {
        std::cout << "# Write " << size << " bytes to the bus: ";
        for (int i = 0; i < size; ++i) {
            if (i != 0) {
                std::cout << ", ";
            }
            std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(data[i]);
        }
        std::cout << std::endl;
    }
    if (write(_i2cFd, data, size) != size) {
        std::cerr << "Failed to write to the bus." << std::endl;
        writeIoError();
        return Status::Error;
    }
    return Status::Success;
}


std::string I2CBus::getDevicePath() const
{
    std::stringstream devicePath;
    devicePath << _devicePathBase << _busId;
    return devicePath.str();
}


void I2CBus::writeIoError()
{
    std::cerr << "Error: " << strerror(errno) << " (errno=" << errno << ")" << std::endl;
}


}

