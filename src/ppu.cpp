#include "glitchmachine.hpp"

sf::Color PPU::palette[0xf] = {
    sf::Color( 0x000000ff ), sf::Color( 0xffffffff ), sf::Color( 0x880000ff ), sf::Color( 0xaaffeeff),
    sf::Color( 0xcc44ccff ), sf::Color( 0x00cc55ff ), sf::Color( 0x0000aaff ), sf::Color( 0xeeee77ff ),
    sf::Color( 0xdd8855ff ), sf::Color( 0x664400ff ), sf::Color( 0xff7777ff ), sf::Color( 0x333333ff ),
    sf::Color( 0x777777ff ), sf::Color( 0xaaff66ff ), sf::Color( 0x0088ffff )//, sf::Color( 0xbbbbbbff )
};

PPU::PPU()
{
    pixel_size.x = (float) SCREEN_WIDTH/32;
    pixel_size.y = (float) SCREEN_HEIGHT/32;

    assert( screen_frame.create( SCREEN_WIDTH, SCREEN_HEIGHT ) == true);

    assert( (int) screen_memory_start + screen_memory_length < UINT16_MAX );
}

// Minimum Viable Product is to run snake6502!
void PPU::MakeFrame( NESRAM * mem )
{
    /*
    if ( mem->read( PPUCTRL ) )
    {
        printf( "Rendering disabled! \n" );
        return;
    }
    */

    screen_frame.clear();
    sf::Vector2f screen_ptr( 0.0, 0.0 );
    uint16_t mem_ptr = screen_memory_start;

    for ( int y = 0; y < 32; y++ )
        for ( int x = 0; x < 32; x++ )
        {
            screen_ptr.x = pixel_size.x * x;
            screen_ptr.y = pixel_size.y * y;

            sf::RectangleShape pixel( pixel_size );
            pixel.setPosition( screen_ptr );
            
            pixel.setFillColor( palette[ mem->read( mem_ptr ) & 0x0f ] );

            screen_frame.draw( pixel );

            mem_ptr++;
        }

    screen_frame.display();
}