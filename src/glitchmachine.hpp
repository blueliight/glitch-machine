#ifndef CHAOSNESH
#define CHAOSNESH

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include <fstream>

#include <SFML/Graphics.hpp>
#include <mos6502.h>
#include "imgui.h"
#include "../libs/imgui-sfml/imgui-SFML.h"

extern "C"{
    #include "../libs/nativefiledialog/src/include/nfd.h"
}

// consts
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const uint16_t PPUCTRL = 0x2000;
const uint16_t PPUMASK = 0x2001;

char * NFDOpen();

class NESRAM : public ReadWriteable
{
private:
    uint8_t mem[ UINT16_MAX ];
public:
    NESRAM();
    uint8_t read( uint16_t addr );
    void write( uint16_t addr, uint8_t data );
    void load_rom_at( uint16_t addr, FILE * rom );
};

class PPU
{
    private:
        bool rendering_enabled;
        u_int16_t screen_memory_start = 0x200;
        int screen_memory_length = 32 * 32;
        sf::Vector2f pixel_size;

        static sf::Color palette[0xf];

    public:
        PPU();
        void MakeFrame( NESRAM * ram );
        sf::VertexArray screen_frame;
};

typedef struct Settings
{
    bool continuous_run;
    bool step;
} Settings;

class GlitchMachine
{
    public:
        FILE * rom = NULL;
        PPU ppu;
        NESRAM ram;
        mos6502 * cpu;
        sf::Clock delta_clock;
        bool ready = false;
        uint64_t cycle_count = 0;
        Settings settings = {
            false, // continuous run
            false, // step
        };

        GlitchMachine( const GlitchMachine& ) = delete; // copy constructor
        static GlitchMachine& Get();

        void KeyPressed( uint8_t key_code );
        void InitGui( sf::RenderWindow * window );
        void OpenRom( char * path );
        void Update();
    
    private:
        GlitchMachine();
        ~GlitchMachine();

        static GlitchMachine gm_instance;
};

#endif