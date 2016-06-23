static void set_bit( uint8_t & data, uint8_t index, bool value )
{
    if( value ) data |= _BV( index );
    else data &= ~_BV( index );
}

static uint8_t get_bit( uint8_t const & data, uint8_t index )
{
    return data & _BV( index );
}
