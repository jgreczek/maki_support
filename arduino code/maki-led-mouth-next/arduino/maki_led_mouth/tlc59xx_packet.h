#include "tlc59xx_write_command_msg.h"
#include "tlc59xx_function_control_msg.h"
#include "tlc59xx_global_brightness_control_msg.h"
#include "tlc59xx_grayscale_control_msg.h"

namespace tlc59xx
{

class Packet
{
protected:
    ::tlc59xx::WriteCommandMessage _write_command_msg;

public:
    ::tlc59xx::FunctionControlMessage function_control_msg_;
    ::tlc59xx::GlobalBrightnessControlMessage global_brightness_control_msg_;
    ::tlc59xx::GrayscaleControlMessage grayscale_control_msg_array_[4];

    Packet()
    {
        //
    }

    void send( ::tlc59xx::Driver & driver )
    {
//      Serial.println( "send >>>" );
//      Serial.println( "write command" );
        _write_command_msg.send( driver );
//      Serial.println( "func_control" );
        function_control_msg_.send( driver );
//      Serial.println( "brightness" );
        global_brightness_control_msg_.send( driver );

        for( uint8_t led_idx = 0; led_idx < 4; ++led_idx )
        {
//          Serial.print( "grayscale " );
//          Serial.println( static_cast<int>( led_idx ) );
            // array item 0 maps to output 3; need to invert indices to get correct mapping
            grayscale_control_msg_array_[3-led_idx].send( driver );
        }
    }
};

} // tlc59xx
