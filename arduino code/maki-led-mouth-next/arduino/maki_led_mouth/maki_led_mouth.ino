#include <Arduino.h>

#define USE_USBCON
#include <ros.h>
#include <std_msgs/Int8.h>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include "bit_utils.h"
#include "tlc59xx_driver.h"
#include "tlc59xx_packet.h"

ros::NodeHandle  nh;

typedef tlc59xx::Driver _Driver;
typedef tlc59xx::Packet _Packet;
typedef tlc59xx::FunctionControlMessage _FunctionControlMsg;

// use pin 4 for data and pin 6 for clock
_Driver led_driver( 4, 6 );

int led_mat[50][3];
int mouth;
int color;
int R;
int G;
int B;

void messageCb(const std_msgs::Int8& mouth_val){
mouth = mouth_val.data;
color = mouth % 10;
mouth /= 10;
}

ros::Subscriber<std_msgs::Int8> sub("led_array", &messageCb);

void setup()
{
  Serial.begin(9600);
 
  // keep inputs from floating and declare pin use case
  led_driver.initialize();

  _Packet packet;

  // reset function flags
  packet.function_control_msg_.setGrayscaleClockSource( _FunctionControlMsg::GrayscaleClockSource::INTERNAL_CLOCK );
  packet.function_control_msg_.setOutputTiming( _FunctionControlMsg::OutputTiming::RISING_EDGE );
  packet.function_control_msg_.setTimingResetMode( _FunctionControlMsg::TimingResetMode::RESET_DISABLED );
  packet.function_control_msg_.setDisplayRepeatMode( _FunctionControlMsg::DisplayRepeatMode::REPEAT_DISABLED );
  packet.function_control_msg_.setBlankMode( _FunctionControlMsg::BlankMode::OUTPUTS_DISABLED );

  // zero-out brightness
  packet.global_brightness_control_msg_.setValues( 0, 0, 0 );

  // zero-out colors
  packet.grayscale_control_msg_array_[0].setValues( 0, 0, 0 );
  packet.grayscale_control_msg_array_[1].setValues( 0, 0, 0 );
  packet.grayscale_control_msg_array_[2].setValues( 0, 0, 0 );
  packet.grayscale_control_msg_array_[3].setValues( 0, 0, 0 );

  // send a copy of the packet for each IC (last IC is addressed first)
  for( uint8_t chip_idx = 0; chip_idx < 13; ++chip_idx )
  {
    packet.send( led_driver );
  }

  // delay to trigger latch pulse
  delay( 1 );
   
  nh.initNode();
  nh.subscribe(sub); 
}

void loop()
{
  _Packet packet;
  // set function flags
  // use internal clock for duty cycle
  packet.function_control_msg_.setGrayscaleClockSource( _FunctionControlMsg::GrayscaleClockSource::INTERNAL_CLOCK );
  // turn LEDs on at rising edge of duty cycle clock
  packet.function_control_msg_.setOutputTiming( _FunctionControlMsg::OutputTiming::RISING_EDGE );
  // automatically reset the internal PWM counter (used to simulate analog duty cycle) during a latch pulse
  packet.function_control_msg_.setTimingResetMode( _FunctionControlMsg::TimingResetMode::RESET_ENABLED );
  // automatically repeat the latched data if no updates were made in a given display cycle
  packet.function_control_msg_.setDisplayRepeatMode( _FunctionControlMsg::DisplayRepeatMode::REPEAT_ENABLED );
  // enable current-sinking outputs
  packet.function_control_msg_.setBlankMode( _FunctionControlMsg::BlankMode::OUTPUTS_ENABLED );

  // set current limit for each channel (R, G, B)
  // the values here were selected to ensure the overall current would remain below 900 mA when all LEDs are at 25% duty cycle
  packet.global_brightness_control_msg_.setValues( 30,30,30 );


////////////////LED ARRAYS//////////////////////////

//////////////// Add Colors ///////////////////////
switch(color){
  case 1:     //Red
    R=10;
    G=0;
    B=0;
    break;
  case 2:     //Blue
    R=0;
    G=0;
    B=10;
    break;
  case 3:     //Green
    R=0;
    G=10;
    B=0;
    break;
  case 4:     //Yellow
    R=10;
    G=10;
    B=0;
    break;
  case 5:     //Purple
    R=10;
    G=0;
    B=10;
    break;
  case 6:     //White
    R=90;
    G=20;
    B=10;
    break;
  case 7:     //Blank or Off
    R=0;
    G=0;
    B=0;
    break;
  case 8:     //Cyan
    R=0;
    G=10;
    B=10;
    break;
  default:     //Blank
    R=0;
    G=0;
    B=0;
    break;
}
/////////////////////////////////////////////////
/////////////Add Visemes////////////////////////

switch(mouth){
  case 1:                   //:D smile AA_AH
  {
    int off_arr[] = {10,11,12,13,14,15,16,17,18,25,26,33,34,35,40,41,44,45,46,47};
    for(int i=0;i<50;i++){
        led_mat[i][0] = 0;
        led_mat[i][1] = 0;
        led_mat[i][2] = 0;
    }
    for(int i=0;i<sizeof(off_arr)/2;i++){
        int ind = off_arr[i];
        led_mat[ind][0] = R;
        led_mat[ind][1] = G;
        led_mat[ind][2] = B;
    }
    break;
  }
  case 2:                   //:O shocked or "O" AO_AW
  {
    int off_arr[] = {0,1,2,3,4,9,10,17,18,25,26,33,35,40,44,45,46,47};
    for(int i=0;i<50;i++){
        led_mat[i][0] = 0;
        led_mat[i][1] = 0;
        led_mat[i][2] = 0;
    }
    for(int i=0;i<sizeof(off_arr)/2;i++){
        int ind = off_arr[i];
        led_mat[ind][0] = R;
        led_mat[ind][1] = G;
        led_mat[ind][2] = B;
    }
    break;
  }
  case 3:                 //close-lipped smile
  {
    int off_arr[] = {10,17,18,25,27,28,29,30,31,32,19,24};
    for(int i=0;i<50;i++){
        led_mat[i][0] = 0;
        led_mat[i][1] = 0;
        led_mat[i][2] = 0;
    }
    for(int i=0;i<sizeof(off_arr)/2;i++){
        int ind = off_arr[i];
        led_mat[ind][0] = R;
        led_mat[ind][1] = G;
        led_mat[ind][2] = B;
    }
    break;
  }
  case 4:                 //[: smile
  {
    int off_arr[] = {10,17,18,25,27,28,29,30,31,32};
    for(int i=0;i<50;i++){
        led_mat[i][0] = 0;
        led_mat[i][1] = 0;
        led_mat[i][2] = 0;
    }
    for(int i=0;i<sizeof(off_arr)/2;i++){
        int ind = off_arr[i];
        led_mat[ind][0] = R;
        led_mat[ind][1] = G;
        led_mat[ind][2] = B;
    }
    break;
  }
  case 5://EY
  {
    int off_arr[] = {10,11,12,13,14,15,16,17,18,25,26,33,34,35,36,37,38,39,40,41};
    for(int i=0;i<50;i++){
        led_mat[i][0] = 0;
        led_mat[i][1] = 0;
        led_mat[i][2] = 0;
    }
    for(int i=0;i<sizeof(off_arr)/2;i++){
        int ind = off_arr[i];
        led_mat[ind][0] = R;
        led_mat[ind][1] = G;
        led_mat[ind][2] = B;
    }
    break;
  }
  case 6://N_NG_D_Z
  {
    int off_arr[] = {13,14,20,23,28,31,37,38};
    for(int i=0;i<50;i++){
        led_mat[i][0] = 0;
        led_mat[i][1] = 0;
        led_mat[i][2] = 0;
    }
    for(int i=0;i<sizeof(off_arr)/2;i++){
        int ind = off_arr[i];
        led_mat[ind][0] = R;
        led_mat[ind][1] = G;
        led_mat[ind][2] = B;
    }
    break;
  }
  case 7://B_M_P
  {
    int off_arr[] = {18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33};
    for(int i=0;i<50;i++){
        led_mat[i][0] = 0;
        led_mat[i][1] = 0;
        led_mat[i][2] = 0;
    }
    for(int i=0;i<sizeof(off_arr)/2;i++){
        int ind = off_arr[i];
        led_mat[ind][0] = R;
        led_mat[ind][1] = G;
        led_mat[ind][2] = B;
    }
    break;
  }
  case 8://EH_AE_AY
  {
    int off_arr[] = {10,11,12,13,14,15,16,17,18,25,26,27,28,29,30,31,32,33};
    for(int i=0;i<50;i++){
        led_mat[i][0] = 0;
        led_mat[i][1] = 0;
        led_mat[i][2] = 0;
    }
    for(int i=0;i<sizeof(off_arr)/2;i++){
        int ind = off_arr[i];
        led_mat[ind][0] = R;
        led_mat[ind][1] = G;
        led_mat[ind][2] = B;
    }
    break;
  }
  default:
    for(int i=0;i<50;i++){
        led_mat[i][0] = R;
        led_mat[i][1] = G;
        led_mat[i][2] = B;
      }
  
  
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////
  // for each IC
  // display a color pallete
  // keep in mind that the 0th chip is the nth chip in the data path (farthest from the data/clock source)
  // and the nth chip is the first chip in the data path
  for( uint8_t chip_idx = 0; chip_idx < 13; ++chip_idx )
  {
    if( chip_idx == 12 )
    {
      // set the RGB values for each output group [0-4] on the current IC
      packet.grayscale_control_msg_array_[0].setValuesPercent( led_mat[1][0], led_mat[1][1], led_mat[1][2] ); //1-2
      packet.grayscale_control_msg_array_[1].setValuesPercent( led_mat[0][0], led_mat[0][1], led_mat[0][2] ); //1-1
      packet.grayscale_control_msg_array_[2].setValuesPercent( led_mat[3][0], led_mat[3][1], led_mat[3][2] ); //1-4
      packet.grayscale_control_msg_array_[3].setValuesPercent( led_mat[2][0], led_mat[2][1], led_mat[2][2] ); //1-3
    }
    else if( chip_idx == 11 )
    {
      // set the RGB values for each output group [0-4] on the current IC
      packet.grayscale_control_msg_array_[0].setValuesPercent( led_mat[10][0], led_mat[10][1], led_mat[10][2] ); //3-1
      packet.grayscale_control_msg_array_[1].setValuesPercent( led_mat[0][0], led_mat[0][1], led_mat[0][2] ); //NA
      packet.grayscale_control_msg_array_[2].setValuesPercent( led_mat[4][0], led_mat[4][1], led_mat[4][2] ); //2-1
      packet.grayscale_control_msg_array_[3].setValuesPercent( led_mat[11][0], led_mat[11][1], led_mat[11][2] ); //3-2
    }
    else if( chip_idx == 10 )
    {
      // set the RGB values for each output group [0-4] on the current IC
      packet.grayscale_control_msg_array_[0].setValuesPercent( led_mat[12][0], led_mat[12][1], led_mat[12][2] );  //3-3
      packet.grayscale_control_msg_array_[1].setValuesPercent( led_mat[5][0], led_mat[5][1], led_mat[5][2] );  //2-2
      packet.grayscale_control_msg_array_[2].setValuesPercent( led_mat[6][0], led_mat[6][1], led_mat[6][2] );  //2-3
      packet.grayscale_control_msg_array_[3].setValuesPercent( led_mat[13][0], led_mat[13][1], led_mat[13][2] );  //3-4
    }
    else if( chip_idx == 9 )
    {
      // set the RGB values for each output group [0-4] on the current IC
      packet.grayscale_control_msg_array_[0].setValuesPercent( led_mat[14][0], led_mat[14][1], led_mat[14][2] );  //3-5
      packet.grayscale_control_msg_array_[1].setValuesPercent( led_mat[7][0], led_mat[7][1], led_mat[7][2] );  //2-4
      packet.grayscale_control_msg_array_[2].setValuesPercent( led_mat[8][0], led_mat[8][1], led_mat[8][2] );  //2-5
      packet.grayscale_control_msg_array_[3].setValuesPercent( led_mat[15][0], led_mat[15][1], led_mat[15][2] );  //3-6
    }
    else if( chip_idx == 8 )
    {
      // set the RGB values for each output group [0-4] on the current IC
      packet.grayscale_control_msg_array_[0].setValuesPercent( led_mat[16][0], led_mat[16][1], led_mat[16][2] );  //3-7
      packet.grayscale_control_msg_array_[1].setValuesPercent( led_mat[9][0], led_mat[9][1], led_mat[9][2] );  //2-6
      packet.grayscale_control_msg_array_[2].setValuesPercent( led_mat[3][0], led_mat[3][1], led_mat[3][2] );  //NA
      packet.grayscale_control_msg_array_[3].setValuesPercent( led_mat[17][0], led_mat[17][1], led_mat[17][2] );  //3-8
    }
    else if( chip_idx == 7 )
    {
      // set the RGB values for each output group [0-4] on the current IC
      packet.grayscale_control_msg_array_[0].setValuesPercent( led_mat[26][0], led_mat[26][1], led_mat[26][2] );  //5-1
      packet.grayscale_control_msg_array_[1].setValuesPercent( led_mat[18][0], led_mat[18][1], led_mat[18][2] );  //4-1
      packet.grayscale_control_msg_array_[2].setValuesPercent( led_mat[19][0], led_mat[19][1], led_mat[19][2] );  //4-2
      packet.grayscale_control_msg_array_[3].setValuesPercent( led_mat[27][0], led_mat[27][1], led_mat[27][2] );  //5-2
    }
    else if( chip_idx == 6 )
    {
      // set the RGB values for each output group [0-4] on the current IC
      packet.grayscale_control_msg_array_[0].setValuesPercent( led_mat[28][0], led_mat[28][1], led_mat[28][2] );  //5-3
      packet.grayscale_control_msg_array_[1].setValuesPercent( led_mat[20][0], led_mat[20][1], led_mat[20][2] );  //4-3
      packet.grayscale_control_msg_array_[2].setValuesPercent( led_mat[21][0], led_mat[21][1], led_mat[21][2] );  //4-4
      packet.grayscale_control_msg_array_[3].setValuesPercent( led_mat[29][0], led_mat[29][1], led_mat[29][2] );  //5-4
    }
    else if( chip_idx == 5 )
    {
      // set the RGB values for each output group [0-4] on the current IC
      packet.grayscale_control_msg_array_[0].setValuesPercent( led_mat[30][0], led_mat[30][1], led_mat[30][2] );  //5-5
      packet.grayscale_control_msg_array_[1].setValuesPercent( led_mat[22][0], led_mat[22][1], led_mat[22][2] );  //4-5
      packet.grayscale_control_msg_array_[2].setValuesPercent( led_mat[23][0], led_mat[23][1], led_mat[23][2] );  //4-6
      packet.grayscale_control_msg_array_[3].setValuesPercent( led_mat[31][0], led_mat[31][1], led_mat[31][2] );  //5-6
    }
    else if( chip_idx == 4 )
    {
      // set the RGB values for each output group [0-4] on the current IC
      packet.grayscale_control_msg_array_[0].setValuesPercent( led_mat[32][0], led_mat[32][1], led_mat[32][2] );  //5-7
      packet.grayscale_control_msg_array_[1].setValuesPercent( led_mat[24][0], led_mat[24][1], led_mat[24][2] );  //4-7
      packet.grayscale_control_msg_array_[2].setValuesPercent( led_mat[25][0], led_mat[25][1], led_mat[25][2] );  //4-8
      packet.grayscale_control_msg_array_[3].setValuesPercent( led_mat[33][0], led_mat[33][1], led_mat[33][2] );  //5-8
    }
    else if( chip_idx == 3 )
    {
      // set the RGB values for each output group [0-4] on the current IC
      packet.grayscale_control_msg_array_[0].setValuesPercent( led_mat[42][0], led_mat[42][1], led_mat[42][2] );  //7-1
      packet.grayscale_control_msg_array_[1].setValuesPercent( led_mat[34][0], led_mat[34][1], led_mat[34][2] );  //6-1
      packet.grayscale_control_msg_array_[2].setValuesPercent( led_mat[35][0], led_mat[35][1], led_mat[35][2] );  //6-2
      packet.grayscale_control_msg_array_[3].setValuesPercent( led_mat[43][0], led_mat[43][1], led_mat[43][2] );  //7-2
    }
    else if( chip_idx == 2 )
    {
      // set the RGB values for each output group [0-4] on the current IC
      packet.grayscale_control_msg_array_[0].setValuesPercent( led_mat[44][0], led_mat[44][1], led_mat[44][2] );  //7-3
      packet.grayscale_control_msg_array_[1].setValuesPercent( led_mat[36][0], led_mat[36][1], led_mat[36][2] );  //6-3
      packet.grayscale_control_msg_array_[2].setValuesPercent( led_mat[37][0], led_mat[37][1], led_mat[37][2] );  //6-4
      packet.grayscale_control_msg_array_[3].setValuesPercent( led_mat[45][0], led_mat[45][1], led_mat[45][2] );  //7-4
    }
    else if( chip_idx == 1 )
    {
      // set the RGB values for each output group [0-4] on the current IC
      packet.grayscale_control_msg_array_[0].setValuesPercent( led_mat[46][0], led_mat[46][1], led_mat[46][2] );  //7-5
      packet.grayscale_control_msg_array_[1].setValuesPercent( led_mat[38][0], led_mat[38][1], led_mat[38][2] );  //6-5
      packet.grayscale_control_msg_array_[2].setValuesPercent( led_mat[39][0], led_mat[39][1], led_mat[39][2] );  //6-6
      packet.grayscale_control_msg_array_[3].setValuesPercent( led_mat[47][0], led_mat[47][1], led_mat[47][2] );  //7-6
    }
    else // chip_idx == 0
    {
      // set the RGB values for each output group [0-4] on the current IC
      packet.grayscale_control_msg_array_[0].setValuesPercent( led_mat[48][0], led_mat[48][1], led_mat[48][2] );  //7-7
      packet.grayscale_control_msg_array_[1].setValuesPercent( led_mat[40][0], led_mat[40][1], led_mat[40][2] );  //6-7
      packet.grayscale_control_msg_array_[2].setValuesPercent( led_mat[41][0], led_mat[41][1], led_mat[41][2] );  //6-8
      packet.grayscale_control_msg_array_[3].setValuesPercent( led_mat[49][0], led_mat[49][1], led_mat[49][2] );  //7-8
    }

    // toggle the OutputTiming bit on every other IC to avoid noise from nearby ICs
    if( chip_idx % 2 == 0 ) packet.function_control_msg_.setOutputTiming( _FunctionControlMsg::OutputTiming::RISING_EDGE );
    else packet.function_control_msg_.setOutputTiming( _FunctionControlMsg::OutputTiming::FALLING_EDGE );

    // write out the entire packet (need to do this once for each IC)
    packet.send( led_driver );
  }
 
  // delay to trigger latch pulse
  nh.spinOnce();
  //delay( 2000 );
}
