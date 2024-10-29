//
// Created by Hollan on 10/18/24.
//

#ifndef JASON_IOTESTER_H
#define JASON_IOTESTER_H

#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <iomanip>
#include <chrono>

#include "Core/Log.h"
#include "IO/Package.h"
#include "Calc/Numerics.h"
#include "Commands/CommandParser.h"

using namespace std;
using namespace std::chrono;

void IOTester() noexcept;

#endif //JASON_IOTESTER_H
