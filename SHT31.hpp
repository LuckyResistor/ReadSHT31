#pragma once
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


#include "StatusTools.hpp"

#include <tuple>
#include <cstdint>


namespace lr {


class I2CBus;


/// A class to access the SHT32 sensor.
///
class SHT31
{
public:
    /// A simple call status.
    ///
    using Status = CallStatus;

    /// The result with temperature and humidity.
    ///
    using THResult = StatusResult<std::tuple<double, double>>;

    /// The result with the status register.
    ///
    using SResult = StatusResult<uint16_t>;

    /// The address for the chip.
    ///
    enum Address {
        Address0,
        Address1
    };

public:
    /// Create a new access object for the SHT32 sensor.
    ///
    /// @param address The address of the chip.
    /// @param i2cBus The I2C bus to use.
    /// @param debuggingEnabled If debugging messages shall be enabled.
    ///
    SHT31(Address address = Address0, int i2cBus = 1, bool debuggingEnabled = false);

    /// dtor
    ///
    ~SHT31();

public:
    /// Open the bus
    ///
    Status openBus();

    /// Close the bus
    ///
    Status closeBus();

    /// Read temperature and humidity.
    ///
    /// @return The temperature in celsius, and the relative humidity in percent.
    ///
    THResult readValues();

    /// Read the status.
    ///
    /// @return The status register.
    ///
    SResult readStatus();

    /// Enable or disable the heater.
    ///
    Status controlHeater(bool enabled);

private:
    /// Calculate CRC-8 as specifieed in the datasheet.
    ///
    static uint8_t getCrc8(const uint8_t *data, int size);

private:
    I2CBus *_bus; ///< The I2C bus used to access the sensor.
};


}

