#include "glitchmachine.hpp"

NESRAM::NESRAM()
{
    memset( mem, 0x00, sizeof( u_int8_t ) * UINT16_MAX );
}

uint8_t NESRAM::read( uint16_t addr )
{
    return mem[addr];
}

void NESRAM::write( uint16_t addr, uint8_t data )
{
    mem[ addr ] = data;
}

void NESRAM::load_rom_at( uint16_t addr, FILE * rom )
{
    uint16_t max_bytes = UINT16_MAX - addr;

    if( fread( &mem[addr], sizeof( uint8_t ), max_bytes, rom ) )
        return;
    
    std::cout << "Warning: fread returned 0 while loading ROM! " << std::endl;
    return;
}