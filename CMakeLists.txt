cmake_minimum_required( VERSION 3.18.4 )

project( ChaosEmulator "CXX" )

add_library( libmos6502 STATIC libs/mos6502/mos6502.cpp )
target_include_directories( libmos6502 PUBLIC libs/mos6502 )

find_package( SFML COMPONENTS graphics REQUIRED )

add_executable( ${PROJECT_NAME} src/chaosnes.cpp src/ppu.cpp src/nesram.cpp )

target_link_libraries( ${PROJECT_NAME} PUBLIC libmos6502 )
target_link_libraries( ${PROJECT_NAME} PUBLIC sfml-graphics )