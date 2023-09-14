cmake_minimum_required( VERSION 3.18.4 )

project( GlitchMachine "CXX" )

add_library( another6502 STATIC libs/another6502/mos6502.cpp )
target_include_directories( another6502 PUBLIC libs/another6502 )

find_package( SFML COMPONENTS graphics REQUIRED )

add_executable( ${PROJECT_NAME} src/main.cpp src/ppu.cpp src/nesram.cpp )

target_link_libraries( ${PROJECT_NAME} PUBLIC another6502 )
target_link_libraries( ${PROJECT_NAME} PUBLIC sfml-graphics )