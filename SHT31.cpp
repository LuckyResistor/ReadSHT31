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
#include "SHT31.hpp"


#include "I2CBus.hpp"

#include <iostream>
#include <cmath>


namespace lr {


SHT31::SHT31(Address address, int i2cBus, bool debuggingEnabled)
    : _bus(nullptr)
{
    const uint8_t chipAddress = (address == Address0 ? 0x44 : 0x45);
    _bus = new I2CBus(chipAddress, i2cBus);
    _bus->setDebugging(debuggingEnabled);
}


SHT31::~SHT31()
{
    if (_bus) { // forgot to close the bus?
        _bus->closeBus();
    }
    delete _bus;
}


SHT31::Status SHT31::openBus()
{
    if (hasError(_bus->openBus())) {
        return Status::Error;
    }
    return CallStatus::Success;
}


SHT31::Status SHT31::closeBus()
{
    if (_bus != nullptr) {
        if (hasError(_bus->closeBus())) {
            return Status::Error;
        }
        delete _bus;
        _bus = nullptr;
    }
    return CallStatus::Success;
}


SHT31::THResult SHT31::readValues()
{
    const uint8_t command[] = {0x2c, 0x06};
    if (hasError(_bus->writeData(command, 2))) {
        return THResult::error();
    }
    uint8_t data[6];
    if (hasError(_bus->readData(data, 6))) {
        return THResult::error();
    }
    // Check the CRC of the values.
    const auto temperatureCalculatedCrc = getCrc8(data, 2);
    const auto temperatureExpectedCrc = data[2];
    if (temperatureCalculatedCrc != temperatureExpectedCrc) {
        std::cerr << "CRC code of read temperature does not match." << std::endl;
        return THResult::error();
    }
    const auto humidityCalculatedCrc = getCrc8(data + 3, 2);
    const auto humidityExpectedCrc = data[5];
    if (humidityCalculatedCrc != humidityExpectedCrc) {
        std::cerr << "CRC code of read humidity does not match." << std::endl;
        return THResult::error();
    }
    const uint16_t temperatureValue = (static_cast<uint16_t>(data[0]) << 8) | static_cast<uint16_t>(data[1]);
    const uint16_t humidityValue = (static_cast<uint16_t>(data[3]) << 8) | static_cast<uint16_t>(data[4]);
    const double rawTemperature = -45.0 + 175.0 * (static_cast<double>(temperatureValue) / static_cast<double>(0xffff));
    const double rawHumidity = 100.0 * (static_cast<double>(humidityValue) / static_cast<double>(0xffff));
    // Round the values to one decimal place.
    const double temperature =  std::round(10.0 * rawTemperature) / 10.0;
    const double humidity =  std::round(10.0 * rawHumidity) / 10.0;
    return THResult::success(std::make_tuple(temperature, humidity));
}


uint8_t SHT31::getCrc8(const uint8_t *data, int size)
{
    const uint8_t polynomial(0x31);
    uint8_t result(0xFF);
    for (int j = size; j; --j) {
        result ^= *data++;
        for (int i = 8; i; --i) {
            result = (result & 0x80) ? (result << 1) ^ polynomial : (result << 1);
        }
    }
    return result;
}


SHT31::SResult SHT31::readStatus()
{
    const uint8_t command[] = {0xf3, 0x2d};
    if (hasError(_bus->writeData(command, 2))) {
        return SResult::error();
    }
    uint8_t data[2];
    if (hasError(_bus->readData(data, 2))) {
        return SResult::error();
    }
    const uint16_t statusValue = (static_cast<uint16_t>(data[0]) << 8) | static_cast<uint16_t>(data[1]);
    return SResult::success(statusValue);
}


SHT31::Status SHT31::controlHeater(bool enabled)
{
    if (enabled) {
        const uint8_t command[] = {0x30, 0x6d};
        if (hasError(_bus->writeData(command, 2))) {
            return Status::Error;
        }
    } else {
        const uint8_t command[] = {0x30, 0x66};
        if (hasError(_bus->writeData(command, 2))) {
            return Status::Error;
        }
    }
    return CallStatus::Success;
}


}
