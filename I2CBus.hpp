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

#include <string>
#include <cstdint>


namespace lr {


/// Wrapper around the quite complicated I2C bus implementation
///
class I2CBus
{
public:
    using Status = CallStatus;

public:
    /// Create a new bus accessor.
    ///
    explicit I2CBus(uint8_t chipAddress, int busId = 1);

    /// dtor
    ///
    /// If the bus object is deconstructed while the bus is open, it is closed.
    ///
    ~I2CBus();

public:
    /// Enable or disable debugging mode.
    ///
    /// In this mode, every bus communication is dumped to the console for debugging.
    ///
    void setDebugging(bool enabled);

    /// Open the bus.
    ///
    /// @return The status of the call.
    ///
    Status openBus();

    /// Close the bus.
    ///
    /// @return The status of the call.
    ///
    Status closeBus();

    /// Check if the bus is open.
    ///
    /// @return `true` on success.
    ///
    bool isOpen() const;

    /// Write data to the I2C bus.
    ///
    /// @param data The data to write.
    /// @param size The size of the data to write.
    /// @return The status of the call.
    ///
    Status writeData(const uint8_t *data, int size);

    /// Read data from the I2C bus.
    ///
    /// @param data The buffer to read data into it.
    /// @param size The number of bytes to read.
    /// @return The status of the call.
    ///
    Status readData(uint8_t *data, int size);

private:
    /// Get the device path.
    ///
    [[nodiscard]] std::string getDevicePath() const;

    /// Write an IO error to cerr
    ///
    static void writeIoError();

private:
    static std::string _devicePathBase; ///< The base path for the I2C device.
    uint8_t _chipAddress; ///< The chip address.
    int _busId; ///< The bus id.
    bool _isOpen; ///< Flag if the bus is open.
    bool _debugging; ///< Flag if debugging the bus is enabled.
    int _i2cFd; ///< The I2C file descriptor.
};


}

