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

    assert( (int) screen_memory_start + screen_memory_length < UINT16_MAX );

    screen_frame.setPrimitiveType( sf::Triangles );
    screen_frame.resize( 32*32*6 );

    // initialize virtual screen vertex array
    sf::Vector2f left_top( 0.f, 0.f );
    sf::Vector2f right_bottom( 0.f, 0.f );
    int index = 0;
    for ( int y = 0; y < 32; y++ )
        for ( int x = 0; x < 32; x++ )
        {
            left_top.x = pixel_size.x * x;
            left_top.y = pixel_size.y * y;

            right_bottom.x = left_top.x + pixel_size.x;
            right_bottom.y = left_top.y + pixel_size.y;

            // top left triangle
            screen_frame[index] = sf::Vertex( left_top, palette[0] );
            screen_frame[index + 1] = sf::Vertex( sf::Vector2f( right_bottom.x, left_top.y ), palette[0] );
            screen_frame[index + 2] = sf::Vertex( sf::Vector2f( left_top.x, right_bottom.y ), palette[0] );

            // bottom right triangle
            screen_frame[index + 3] = sf::Vertex( sf::Vector2f( right_bottom.x, left_top.y ), palette[0] );
            screen_frame[index + 4] = sf::Vertex( right_bottom, palette[4] );
            screen_frame[index + 5] = sf::Vertex( sf::Vector2f( left_top.x, right_bottom.y ), palette[0] );

            index += 6;
        }
}

// Minimum Viable Product is to run snake6502!
void PPU::MakeFrame( NESRAM * mem )
{
    uint16_t mem_ptr = screen_memory_start;

    int p_index = 0;
    sf::Color p_color;
    for ( int y = 0; y < 32; y++ )
        for ( int x = 0; x < 32; x++ )
        {
            p_color = palette[ mem->read( mem_ptr ) & 0x0f ];
            
            screen_frame[ p_index ].color = p_color;
            screen_frame[ p_index + 1 ].color = p_color;
            screen_frame[ p_index + 2 ].color = p_color;
            screen_frame[ p_index + 3 ].color = p_color;
            screen_frame[ p_index + 4 ].color = p_color;
            screen_frame[ p_index + 5 ].color = p_color;
            
            p_index += 6;
            mem_ptr++;
        }
    
}