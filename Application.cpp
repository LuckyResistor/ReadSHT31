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
#include "Application.hpp"


namespace lr {


void Application::showHelp()
{
    std::cerr << "Usage: read_sht31 [arguments]" << std::endl;
    std::cerr << " -h|--help   Display this help." << std::endl;
    std::cerr << " -s          Get the sensor status." << std::endl;
    std::cerr << " -t1 -t0     Enable/disable heater." << std::endl;
    std::cerr << " -a0 -a1     Select the chip address. 0 is the default." << std::endl;
    std::cerr << " -b0 -b1     Select the bus. 1 is the default." << std::endl;
    std::cerr << " -d          Show debugging messages." << std::endl;
}


bool Application::parseCommandLine(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        const auto arg = std::string(argv[i]);
        if (arg == "-h" || arg == "--help") {
            showHelp();
            return false;
        } else if (arg == "-d") {
            _debuggingEnabled = true;
        } else if (arg == "-s") {
            _action = Action::ReadStatus;
        } else if (arg == "-t1") {
            _action = Action::HeaterEnable;
        } else if (arg == "-t0") {
            _action = Action::HeaterDisable;
        } else if (arg == "-a0") {
            _chipAddress = lr::SHT31::Address0;
        } else if (arg == "-a1") {
            _chipAddress = lr::SHT31::Address1;
        } else if (arg == "-b0") {
            _bus = 0;
        } else if (arg == "-b1") {
            _bus = 1;
        } else {
            std::cerr << "Unknown argument \"" << arg << "\"." << std::endl;
            showHelp();
            return false;
        }
    }
    return true;
}


int Application::run(int argc, char **argv)
{
    // Initialize and open the bus.
    if (!parseCommandLine(argc, argv)) {
        return 1;
    }
    lr::SHT31 shtAccess(_chipAddress, _bus, _debuggingEnabled);
    if (hasError(shtAccess.openBus())) {
        return 1;
    }
    if (_action == Action::ReadValues) {
        const auto readResult = shtAccess.readValues();
        if (hasError(readResult)) {
            return 1;
        }
        // Output the values as JSON
        const auto [temperature, humidity] = readResult.getValue();
        std::cout << "{ \"temperature_celsius\": " << temperature << ", \"relative_humidity\": " << humidity << " }" << std::endl;
    } else if (_action == Action::ReadStatus) {
        const auto readResult = shtAccess.readStatus();
        if (hasError(readResult)) {
            return 1;
        }
        // Output the values as JSON
        std::cout << "{ \"status\": " << readResult.getValue() << " }" << std::endl;
    } else if (_action == Action::HeaterEnable) {
        const auto readResult = shtAccess.controlHeater(true);
        if (hasError(readResult)) {
            return 1;
        }
        // Output the values as JSON
        std::cout << R"({ "status": "heater_enabled" })" << std::endl;
    } else if (_action == Action::HeaterDisable) {
        const auto readResult = shtAccess.controlHeater(false);
        if (hasError(readResult)) {
            return 1;
        }
        // Output the values as JSON
        std::cout << R"({ "status": "heater_disabled" })" << std::endl;
    }
    // Close the bus.
    shtAccess.closeBus();
    // Success
    if (_debuggingEnabled) {
        std::cout << "# Success." << std::endl;
    }
    return 0;
}


}

