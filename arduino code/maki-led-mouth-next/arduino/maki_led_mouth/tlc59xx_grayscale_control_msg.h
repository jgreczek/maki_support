namespace tlc59xx
{

// this is actually the color information
class GrayscaleControlMessage : public RGBColorValueInterface<uint16_t>
{
public:
    typedef RGBColorValueInterface<uint16_t> _RGBColorValue;

    GrayscaleControlMessage( uint16_t value_red = 0, uint16_t value_green = 0, uint16_t value_blue = 0 )
    :
        _RGBColorValue( value_red, value_green, value_blue )
    {
        //
    }

    virtual void setValuePercent( ColorChannel color_channel, float value )
    {
        this->data_[static_cast<uint8_t>(color_channel)] = static_cast<uint16_t>( round( 65535.0f * value ) );
    }

    void send( ::tlc59xx::Driver & driver )
    {
        driver.writeData( reinterpret_cast<uint8_t *>( &this->data_[static_cast<uint8_t>(ColorChannel::BLUE)] ), 16 );
//      Serial.println( "---" );
        driver.writeData( reinterpret_cast<uint8_t *>( &this->data_[static_cast<uint8_t>(ColorChannel::GREEN)] ), 16 );
//      Serial.println( "---" );
        driver.writeData( reinterpret_cast<uint8_t *>( &this->data_[static_cast<uint8_t>(ColorChannel::RED)] ), 16 );
    }
};

} // tlc59xx
