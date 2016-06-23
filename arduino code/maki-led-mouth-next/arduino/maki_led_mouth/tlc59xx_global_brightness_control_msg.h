#include "rgb_color_value_interface.h"

namespace tlc59xx
{

// note that each channel value is only 7 bits (0-127)
// first bit will be skipped, so a value of 128 will be interpreted as 0 and a value of 255 will be intepreted as 127
class GlobalBrightnessControlMessage : public RGBColorValueInterface<uint8_t>
{
public:
    typedef RGBColorValueInterface<uint8_t> _RGBColorValue;

    GlobalBrightnessControlMessage( uint8_t value_red = 0, uint8_t value_green = 0, uint8_t value_blue = 0 )
    :
        _RGBColorValue( value_red, value_green, value_blue )
    {
        //
    }

    virtual void setValuePercent( ColorChannel color_channel, float value )
    {
        this->data_[static_cast<uint8_t>(color_channel)] = static_cast<uint8_t>( round( 127.0f * value ) );
    }

    void send( ::tlc59xx::Driver & driver )
    {
        driver.writeData( &this->data_[static_cast<uint8_t>(ColorChannel::BLUE)], 7 );
//      Serial.println( "---" );
        driver.writeData( &this->data_[static_cast<uint8_t>(ColorChannel::GREEN)], 7 );
//      Serial.println( "---" );
        driver.writeData( &this->data_[static_cast<uint8_t>(ColorChannel::RED)], 7 );
    }
};

} // tlc59xx
