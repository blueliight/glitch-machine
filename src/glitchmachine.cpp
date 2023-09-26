#include "glitchmachine.hpp"

GlitchMachine::GlitchMachine()
{
    bool error = false;
    cpu = new mos6502( &ram );
}

GlitchMachine::~GlitchMachine()
{
    delete cpu;
}

GlitchMachine& GlitchMachine::Get()
{
    return gm_instance;
}

void GlitchMachine::OpenRom( char * path )
{
    rom = fopen( path, "r" );
    if ( rom != NULL )
    {
        ram.load_rom_at( 0x00, rom );
        cpu->Reset();
        ready = true;
        std::cout << "Opened file " << path << std::endl;
    }
    else
        std::cerr << "Unable to open " << path << std::endl;
}

void GlitchMachine::Update()
{
    if ( ready )
        if ( settings.continuous_run || settings.step )
        {
            // update random number
            ram.write( 0x00fe, rand() & 0xff );

            // TODO: handle input


            cycle_count = 0;
            cpu->Run( 1, cycle_count, mos6502::CycleMethod::INST_COUNT );
            ppu.MakeFrame( &ram );
            settings.step = false;
        }
    else
    {
        // draw a help window w/ imgui
    }

    if ( cpu->illegalOpcode )
    {
        printf( "Illegal opcode %x at %x! \n", cpu->GetPC(), ram.read( cpu->GetPC() ) );
        cpu->Reset();
    }
}

sf::Texture GlitchMachine::GetScreenTexture()
{
    return ppu.screen_frame.getTexture();
}

void GlitchMachine::KeyPressed( uint8_t key_code )
{
    ram.write( 0xff, key_code );
}