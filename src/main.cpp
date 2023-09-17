#include "glitchmachine.hpp"
const int MSG_MAX_LENGTH = 30;

int main( int argc, char** argv )
{   

    if ( argc < 2 )
    {
        std::cout << "No ROM file specified. Exiting." << std::endl;
        return 0;
    }

    FILE * rom = fopen( argv[1], "r" );
    if ( rom == NULL )
    {
        std::cout << "Unable to open " << argv[1] << ", exiting." << std::endl;
        return 0;
    }

    sf::Font font;
    if ( !font.loadFromFile( "libs/256/Dernyn's-256(baseline).ttf" ) )
    {
        std::cerr << "Error while loading font" << std::endl;
        return 1;
    }

    PPU ppu;
    NESRAM ram;
    mos6502 cpu( &ram );

    ram.load_rom_at( 0x00, rom );
    cpu.Reset();

    char instr_msg[MSG_MAX_LENGTH];
    char pc_msg[MSG_MAX_LENGTH];

    sf::Text instr_msg_text;
    instr_msg_text.setCharacterSize( 24 );
    instr_msg_text.setFillColor( sf::Color::Blue );
    instr_msg_text.setStyle( sf::Text::Regular );
    instr_msg_text.setFont( font );
    instr_msg_text.setPosition( 0.0, 0.0 );

    sf::Text pc_msg_text;
    pc_msg_text.setCharacterSize( 24 );
    pc_msg_text.setFillColor( sf::Color::Blue );
    pc_msg_text.setStyle( sf::Text::Regular );
    pc_msg_text.setFont( font );
    instr_msg_text.setPosition( 0.0, 25.0 );

    sf::RenderWindow window( sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), "Glitch Machine" );

    while( window.isOpen() )
    {
        sf::Event event;
        while ( window.pollEvent( event ) )
        {
            if ( event.type == sf::Event::Closed )
                window.close();
        }

        uint64_t cycle_count = 0;

        if ( snprintf( pc_msg, MSG_MAX_LENGTH, "PC = %x", cpu.GetPC() ) < 0 )
        {
            std::cerr << "Error formatting pc_msg!" << std::endl;
            return 1;
        }

        if ( snprintf( instr_msg, MSG_MAX_LENGTH, "Instruction = %x", ram.read( cpu.GetPC() ) ) < 0 )
        {
            std::cerr << "Error formatting instr_msg!" << std::endl;
            return 1;
        }

        sf::String pc_msg_string( pc_msg );
        pc_msg_text.setString( pc_msg_string );
        
        sf::String instr_msg_string( instr_msg );
        instr_msg_text.setString( instr_msg_string );

        cpu.Run( 1, cycle_count, mos6502::CycleMethod::INST_COUNT );
        ppu.MakeFrame( &ram );

        window.clear();
        window.draw( sf::Sprite( ppu.screen_frame.getTexture() ) );
        window.draw( pc_msg_text );
        window.draw( instr_msg_text );
        window.display();

        if ( cpu.illegalOpcode )
        {
            printf( "Illegal opcode %x at %x!", cpu.GetPC(), ram.read( cpu.GetPC() ) );
            cpu.Reset();
        }
    }

    return 0;
}