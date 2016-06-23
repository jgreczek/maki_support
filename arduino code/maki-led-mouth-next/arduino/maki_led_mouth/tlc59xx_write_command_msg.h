namespace tlc59xx
{

class WriteCommandMessage
{
public:
    uint8_t data_;
    static const uint8_t num_bits_ = 6;

    WriteCommandMessage()
    :
        data_( 0x25 )
    {
        //
    }

    void send( ::tlc59xx::Driver & driver )
    {
        driver.writeData( &data_, 6 );
    }
};

} // tlc59xx
