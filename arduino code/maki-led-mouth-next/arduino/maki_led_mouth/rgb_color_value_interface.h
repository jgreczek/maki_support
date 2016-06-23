class RGBControlMessage
{
public:
    enum class ColorChannel : uint8_t
    {
        BLUE = 0,
        GREEN = 1,
        RED = 2
    };
};

template<class __Data>
class RGBColorValueInterface : public RGBControlMessage
{
public:
    __Data data_[3];

    RGBColorValueInterface( __Data value_red = 0, __Data value_green = 0, __Data value_blue = 0 )
    :
        data_{ value_blue, value_green, value_red }
    {
        //
    }

    void setRed( __Data value )
    {
        data_[static_cast<uint8_t>(ColorChannel::RED)] = value;
    }

    void setGreen( __Data value )
    {
        data_[static_cast<uint8_t>(ColorChannel::GREEN)] = value;
    }

    void setBlue( __Data value )
    {
        data_[static_cast<uint8_t>(ColorChannel::BLUE)] = value;
    }

    __Data & getValue( ColorChannel color_channel )
    {
        return data_[color_channel];
    }

    void setValue( ColorChannel color_channel, __Data value )
    {
        data_[static_cast<uint8_t>(color_channel)] = value;
    }

    void setValues( __Data value_red, __Data value_green, __Data value_blue )
    {
        setValue( ColorChannel::RED, value_red );
        setValue( ColorChannel::GREEN, value_green );
        setValue( ColorChannel::BLUE, value_blue );
    }

    void setValuesPercent( float value_red, float value_green, float value_blue )
    {
        setValuePercent( ColorChannel::RED, value_red );
        setValuePercent( ColorChannel::GREEN, value_green );
        setValuePercent( ColorChannel::BLUE, value_blue );
    }

    virtual void setValuePercent( ColorChannel color_channel, float value ) = 0;
};
