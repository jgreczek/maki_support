namespace tlc59xx
{

class FunctionControlMessage
{
public:
    uint8_t data_;
    static const uint8_t num_bits_ = 5;

    enum class BitValues : uint8_t
    {
        OUTPUT_TIMING = 4,
        GRAYSCALE_CLOCK_SOURCE = 3,
        TIMING_RESET_MODE = 2,
        DISPLAY_REPEAT_MODE = 1,
        BLANK_MODE = 0
    };

    enum class OutputTiming : uint8_t
    {
        RISING_EDGE,
        FALLING_EDGE
    };

    enum class GrayscaleClockSource : uint8_t
    {
        INTERNAL_CLOCK,
        EXTERNAL_CLOCK
    };

    enum class TimingResetMode : uint8_t
    {
        RESET_ENABLED,
        RESET_DISABLED
    };

    enum class DisplayRepeatMode : uint8_t
    {
        REPEAT_ENABLED,
        REPEAT_DISABLED
    };

    enum class BlankMode : uint8_t
    {
        OUTPUTS_ENABLED,
        OUTPUTS_DISABLED
    };

    FunctionControlMessage( uint8_t data = 0x00 )
    :
        data_( data )
    {
        //
    }

    void setOutputTiming( OutputTiming output_timing )
    {
        switch( output_timing )
        {
        case OutputTiming::RISING_EDGE:
            set_bit( data_, static_cast<uint8_t>(BitValues::OUTPUT_TIMING), 1 );
            break;
        case OutputTiming::FALLING_EDGE:
            set_bit( data_, static_cast<uint8_t>(BitValues::OUTPUT_TIMING), 0 );
            break;
        }
    }

    void setGrayscaleClockSource( GrayscaleClockSource grayscale_clock_source )
    {
        switch( grayscale_clock_source )
        {
        case GrayscaleClockSource::INTERNAL_CLOCK:
            set_bit( data_, static_cast<uint8_t>(BitValues::GRAYSCALE_CLOCK_SOURCE), 0 );
            break;
        case GrayscaleClockSource::EXTERNAL_CLOCK:
            set_bit( data_, static_cast<uint8_t>(BitValues::GRAYSCALE_CLOCK_SOURCE), 1 );
            break;
        }
    }

    void setTimingResetMode( TimingResetMode timing_reset_mode )
    {
        switch( timing_reset_mode )
        {
        case TimingResetMode::RESET_ENABLED:
            set_bit( data_, static_cast<uint8_t>(BitValues::TIMING_RESET_MODE), 1 );
            break;
        case TimingResetMode::RESET_DISABLED:
            set_bit( data_, static_cast<uint8_t>(BitValues::TIMING_RESET_MODE), 0 );
            break;
        }
    }

    void setDisplayRepeatMode( DisplayRepeatMode display_repeat_mode )
    {
        switch( display_repeat_mode )
        {
        case DisplayRepeatMode::REPEAT_ENABLED:
            set_bit( data_, static_cast<uint8_t>(BitValues::DISPLAY_REPEAT_MODE), 1 );
            break;
        case DisplayRepeatMode::REPEAT_DISABLED:
            set_bit( data_, static_cast<uint8_t>(BitValues::DISPLAY_REPEAT_MODE), 0 );
            break;
        }
    }

    void setBlankMode( BlankMode blank_mode )
    {
        switch( blank_mode )
        {
        case BlankMode::OUTPUTS_ENABLED:
            set_bit( data_, static_cast<uint8_t>(BitValues::BLANK_MODE), 0 );
            break;
        case BlankMode::OUTPUTS_DISABLED:
            set_bit( data_, static_cast<uint8_t>(BitValues::BLANK_MODE), 1 );
            break;
        }
    }

    void send( ::tlc59xx::Driver & driver )
    {
        driver.writeData( &data_, 5 );
    }
};

} // tlc59xx
