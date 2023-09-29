#include "glitchmachine.hpp"

GlitchMachine GlitchMachine::gm_instance;

char * NFDOpen()
{
    nfdchar_t * out_path = NULL;
    nfdresult_t result = NFD_OpenDialog( NULL, NULL, &out_path );

    if ( result == NFD_OKAY )
    {
        return out_path;
    }
    else if ( result == NFD_CANCEL )
    {
        return NULL;
    }
    else
    {
        std::cerr << "NativeFileDialog error: " << NFD_GetError() << std::endl;
        return NULL;
    }
}

int main( int argc, char** argv )
{
    sf::Clock delta_clock;
    bool show_processor_status = false;
    bool show_open_file = false;
    bool show_glitch_controls = false;

    sf::RenderWindow window( sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), "Glitch Machine"  );
    if ( !ImGui::SFML::Init( window ) )
    {
        std::cerr << "Unable to initialize imgui!" << std::endl;
    }


    if ( argc > 1 )
    {
        GlitchMachine::Get().OpenRom( argv[1] );
    }


    while( window.isOpen() )
    {
        sf::Event event;
        while ( window.pollEvent( event ) )
        {
            ImGui::SFML::ProcessEvent( window, event );
            if ( event.type == sf::Event::Closed )
                window.close();
            
            if ( event.type == sf::Event::TextEntered )
            {
                GlitchMachine::Get().KeyPressed( event.key.code & 0xff ); // unicode -> ascii
            }
        }

        GlitchMachine::Get().Update();

        //
        // HERE COMES THE UI CODE ( please find a better place to put it )
        //

        ImGui::SFML::Update( window, delta_clock.restart() );

        if ( ImGui::BeginMainMenuBar() )
        {
            if ( ImGui::BeginMenu( "File" ) )
            {
                ImGui::MenuItem( "Open", NULL, &show_open_file );
                ImGui::EndMenu();
            }
            if ( ImGui::BeginMenu( "Tools" ) )
            {
                ImGui::MenuItem( "Glitch Controls", NULL, &show_glitch_controls );
                ImGui::EndMenu();
            }
            if( ImGui::BeginMenu( "View" ) )
            {
                ImGui::MenuItem( "CPU Status", NULL, &show_processor_status );
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if( show_processor_status )
            if ( ImGui::Begin( "Processor Status" ) )
            {
                // draw table of processor registers
                if( ImGui::BeginTable( "Register Table", 2, 
                    ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
                ))
                {
                    ImGui::TableNextColumn();
                    ImGui::TableHeader( "Register" );
                    ImGui::TableNextColumn();
                    ImGui::TableHeader( "Status" );

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text( "PC" );
                    ImGui::TableNextColumn();
                    ImGui::Text( "%x", GlitchMachine::Get().cpu->GetPC() );

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text( "A" );
                    ImGui::TableNextColumn();
                    ImGui::Text( "%x", GlitchMachine::Get().cpu->GetA() );

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text( "P" );
                    ImGui::TableNextColumn();
                    ImGui::Text( "%x", GlitchMachine::Get().cpu->GetP() );

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text( "X" );
                    ImGui::TableNextColumn();
                    ImGui::Text( "%x", GlitchMachine::Get().cpu->GetX() );

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text( "Y" );
                    ImGui::TableNextColumn();
                    ImGui::Text( "%x", GlitchMachine::Get().cpu->GetY() );

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text( "S" );
                    ImGui::TableNextColumn();
                    ImGui::Text( "%x", GlitchMachine::Get().cpu->GetS() );
                
                    ImGui::EndTable();
                }

            ImGui::Text( "Current instruction = %x", GlitchMachine::Get().ram.read( GlitchMachine::Get().cpu->GetPC() ) );

            ImGui::Checkbox( "Run", &GlitchMachine::Get().settings.continuous_run );

            if ( !GlitchMachine::Get().settings.continuous_run )
                GlitchMachine::Get().settings.step = ImGui::Button( "Step" );
            ImGui::End();
        }

        if (show_open_file)
        {
            char * path = NFDOpen();
            if ( path != NULL )
                GlitchMachine::Get().OpenRom( path );
            else
                std::cerr << "NativeFileDialog error!" << std::endl;
            show_open_file = false;
        }


        window.clear();
        window.draw( GlitchMachine::Get().ppu.screen_frame );
        ImGui::SFML::Render( window );
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}