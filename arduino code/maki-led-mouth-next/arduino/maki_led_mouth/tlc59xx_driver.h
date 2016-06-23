namespace tlc59xx
{

class Driver
{
public:
protected:
    uint8_t _data_pin;
    uint8_t _clock_pin;

public:
    Driver( uint8_t data_pin, uint8_t clock_pin )
    :
        _data_pin( data_pin ),
        _clock_pin( clock_pin )
    {
        //
    }

    void initialize()
    {
        pinMode( _data_pin, OUTPUT );
        pinMode( _clock_pin, OUTPUT );

        digitalWrite( _data_pin, LOW );
        digitalWrite( _clock_pin, LOW );
    }

    // can write up to 292 ICs worth of data
    // writes MSB first up to size_bits bits
    void writeData( uint8_t * data, uint16_t size_bits )
    {
        // optimization for single byte or less
        if( size_bits <= 8 )
        {
            for( uint8_t bit_idx = 7 - (size_bits-1); bit_idx < 8; ++bit_idx )
            {
                writeBit( get_bit( *data, 7 - bit_idx ) );
            }
        }
        else
        {
            uint8_t byte_idx;
            uint8_t size_bytes = ceil( size_bits / 8 ) - 1;
            for( uint8_t bit_idx = 0; bit_idx < size_bits; ++bit_idx )
            {
                byte_idx = floor( bit_idx / 8 );

                // update the data pin
                // we do MSB to LSB (7->0) but bits are indexed LSB to MSB (0->7) using the _BV macro
                // thus we need to invert the bit indices (7 - idx)
                writeBit( get_bit( data[size_bytes - byte_idx], 7 - ( bit_idx % 8 ) ) );
            }
        }
    }

    void updateClock()
    {
        digitalWrite( _clock_pin, HIGH );
        // need to ensure we have a measureable clock pulse to avoid spurrious latch pulses
        // 4-5 us seems to work well for the arduino micro but this may need to be tweaked for other platforms
        delayMicroseconds( 4 );
        digitalWrite( _clock_pin, LOW );
    }

    void writeBit( bool value )
    {
//      Serial.println( static_cast<int>( value ) );

        digitalWrite( _data_pin, value ? HIGH : LOW );
        updateClock();
    }

};

} // tlc59xx
