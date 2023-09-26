cmake_minimum_required( VERSION 3.18.4 )

project( GlitchMachine "CXX" )
enable_language( C )

# another6502
set( ANOTHER6502_PATH "libs/another6502" )
add_library( another6502 STATIC ${ANOTHER6502_PATH}/mos6502.cpp )

# imgui-sfml
set( IMGUI_SFML_PATH "libs/imgui-sfml" )
set( IMGUI_DIR "../imgui" )
set( IMGUI_SFML_FIND_SFML OFF CACHE BOOL "Use find_package to find SFML" FORCE )
add_subdirectory( ${IMGUI_SFML_PATH} )

find_package( SFML COMPONENTS graphics REQUIRED )

# native file dialog
set( NFD_PATH "libs/nativefiledialog/src" )
set( NFD_SOURCES ${NFD_PATH}/nfd_common.c )

if( UINX AND NOT APPLE )
    list( APPEND NFD_SOURCES ${NFD_PATH}/nfd_gtk.c )
elseif( UNIX )
    list( APPEND NFD_SOURCES ${NFD_PATH}/nfd_zenity.c )
else()
    list( APPEND NFD_SOURCES ${NFD_PATH}/nfd_win.cpp )
endif()

add_library( NFD STATIC ${NFD_SOURCES} )
target_include_directories( NFD PUBLIC ${NFD_PATH}/include )
set_target_properties( NFD PROPERTIES LINKER_LANGUAGE C )

add_executable( ${PROJECT_NAME} src/ppu.cpp src/nesram.cpp src/glitchmachine.cpp src/main.cpp )

target_link_libraries( ${PROJECT_NAME} PUBLIC another6502 )
target_link_libraries( ${PROJECT_NAME} PUBLIC sfml-graphics )
target_link_libraries( ${PROJECT_NAME} PUBLIC ImGui-SFML::ImGui-SFML )
target_link_libraries( ${PROJECT_NAME} PUBLIC NFD )

target_include_directories( ${PROJECT_NAME} PUBLIC ${ANOTHER6502_PATH} )
target_include_directories( ${PROJECT_NAME} PUBLIC ${IMGUI_SFML_PATH} )
target_include_directories( ${PROJECT_NAME} PUBLIC libs/imgui )
target_include_directories( ${PROJECT_NAME} PUBLIC ${NFD_PATH}/include )