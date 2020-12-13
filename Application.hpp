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


#include "SHT31.hpp"

#include <iostream>


namespace lr {


class Application
{
public:
    /// The actions
    ///
    enum class Action {
        ReadValues,
        ReadStatus,
        HeaterEnable,
        HeaterDisable
    };

public:
    /// Run the application.
    ///
    int run(int argc, char **argv);

private:
    /// Show the help.
    ///
    void showHelp();

    /// Parse the command line parameters.
    ///
    bool parseCommandLine(int argc, char *argv[]);

private:
    bool _debuggingEnabled = false; ///< If debugging shall be enabled.
    Action _action = Action::ReadValues; ///< The requested _action.
    int _bus = 1; ///< The I2C bus to use.
    lr::SHT31::Address _chipAddress = lr::SHT31::Address0; ///< The chip address.
};


}

