#ifndef CHAOSNESH
#define CHAOSNESH

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include <fstream>

#include <SFML/Graphics.hpp>
#include "../libs/another6502/mos6502.h"

// consts
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const uint16_t PPUCTRL = 0x2000;
const uint16_t PPUMASK = 0x2001;

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
public:
    PPU();
    void MakeFrame( NESRAM * ram );
    sf::RenderTexture screen_frame;
};

#endif