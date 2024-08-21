cmake_minimum_required(VERSION 3.28)
project(jason_calc VERSION 1.0.1 DESCRIPTION "A supporting library for Jason that provides calculation entities")

set(CMAKE_CXX_STANDARD 20)

include(GNUInstallDirs)

add_library(jason_calc SHARED src/StdCalc.cpp
        src/VariableType.cpp
        src/Scalar.cpp
        src/Matrix.cpp
        src/MathVector.cpp)

set_target_properties(jason_calc PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR})

configure_file(jason_calc.pc.in jason_calc.pc @ONLY)
target_include_directories(jason_calc PRIVATE .)
install(TARGETS jason_calc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}})
install(FILES ${CMAKE_BINARY_DIR}/jason_calc.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
