#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "spi.h"

#include "acc_sensor_driver.h"
#include "afproto.h"
#include "gpio.h"
#include "usart.h"

// Khai bao cau truc du lieu luu thong tin doc tu cam bien
static Lis3dsh __cur_acc_data;

volatile uint8_t userBtnPressed = 0;

//Change these values if accelerometer reading are different:
//How far the accerometer is tilted before
//starting to move the mouse:
const int MovementThreshold = 18;

//The average zero acceleration values read
//from the accelerometer for each axis:
const int ZeroXValue = 0;
const int ZeroYValue = 0;
//const int ZeroZValue = 0;

//The maximum (positive) acceleration values read
//from the accelerometer for each axis:
const int MaxXValue = 4096;
const int MaxYValue = 4096;
//const int MaxZValue = 4096;

//The minimum (negative) acceleration values read
//from the accelerometer for each axis:
const int MinXValue = -4096;
const int MinYValue = -4096;
//const int MinZValue = -4096;

//The sign of the mouse movement relative to the acceleration.
//If your cursor is going in the opposite direction you think it
//should go, change the sign for the appropriate axis.
const int XSign = 1;
const int YSign = 1;
//const int ZSign = 1;

//The maximum speed in each axis (x and y)
//that the cursor should move. Set this to a higher or lower
//number if the cursor does not move fast enough or is too fast.
const int MaxMouseMovement = 50;  

//This reduces the 'twitchiness' of the cursor by calling
//a delay function at the end of the main loop.
//There are better way to do this without delaying the whole
//microcontroller, but that is left for another tutorial or project.
const int MouseDelay = 12;

#include <string.h>
#define MAX_FRAME_BUFFER   (256)
static uint32_t _gFrameLen = 0;
static char  _gFrameBuf[MAX_FRAME_BUFFER];

void acc_sensor_init(void)
{	
	uint8_t chip_id = 0;
	
	LIS3DSH_init(&__cur_acc_data);
	/* Doc ID cua chip */
	chip_id = LIS3DSH_read_chip_id();

	if (chip_id != 0x3F)
	{
		while(1) {}
	}

	LIS3DSH_readSetting(&__cur_acc_data);
}	




void __acc_send_dataframe(Lis3dsh *acc_data) 
{
	MovementStruct_t movement;
	
	acc_sensor_process_movement(acc_data, &movement);
	//memset(_gFrameBuf, 0x00, MAX_FRAME_BUFFER);
	_gFrameLen = acc_sensor_build_frame(&movement, _gFrameBuf);
	
	__sendBufferToUart(_gFrameBuf, _gFrameLen);
}

void __acc_sensor_led_indicator(Lis3dsh *acc_data)
{
	uint16_t data_x = acc_data->accX;
	uint16_t data_y = acc_data->accY;
	
	if (49000 < data_x && data_x < 65000 ) 
	{
		gpio_led_state(LED4_GREEN_ID, 1);
	}else
	{
		gpio_led_state(LED4_GREEN_ID, 0);
	}
	
	if (0 < data_x && data_x < 17000 ) 
	{
		gpio_led_state(LED5_RED_ID, 1);
	}else
	{
		gpio_led_state(LED5_RED_ID, 0);
	}
	
	if (49000 < data_y && data_y < 65000 ) 
	{
		gpio_led_state(LED6_BLUE_ID, 1);
	}else
	{
		gpio_led_state(LED6_BLUE_ID, 0);
	}
	
	if (0 < data_y && data_y < 17000 ) 
	{
		gpio_led_state(LED3_ORANGE_ID, 1);
	}else
	{
		gpio_led_state(LED3_ORANGE_ID, 0);	
	}
	
}

void acc_sensor_reading(void)
{
	float pitch, roll;
	
	/* Doc thong so xyz tu cam bien */
	LIS3DSH_readXYZ(&__cur_acc_data);
	
	// __acc_sensor_led_indicator(&__cur_acc_data);
	/* Tin toan pitch roll */
//	LIS3DSH_calc_pitch_roll(&__cur_acc_data, &pitch, &roll);
}		

Lis3dsh* acc_get_data_pointer(void)
{
	return &__cur_acc_data;
}

uint32_t acc_sensor_build_frame(MovementStruct_t* mov, char* out_buf)
{
	uint32_t write_len = 0;
	char  tmp_buf[128] = {0};
	
	sprintf(tmp_buf, "%d,%d,%d", mov->XMovement, mov->YMovement, mov->clicked);
	afproto_frame_data(tmp_buf, strlen(tmp_buf), out_buf, &write_len);
	
	return write_len;
}


void acc_sensor_process_movement(Lis3dsh *acc_data, MovementStruct_t *movement)
{
  //Initialize values for the mouse cursor movement.
  int16_t MouseXMovement = 0;
  int16_t MouseYMovement = 0;
  
  //Calculate mouse movement
  //If the analog X reading is ouside of the zero threshold...
  if( MovementThreshold < abs( acc_data->accX - ZeroXValue ) )
  {
    //...calculate X mouse movement based on how far the X acceleration is from its zero value.
    MouseXMovement = XSign * ( ( ( (float)( 2 * MaxMouseMovement ) / ( MaxXValue - MinXValue ) ) * ( acc_data->accX - MinXValue ) ) - MaxMouseMovement );
    //it could use some improvement, like making it trigonometric.
  }
  else
  {
    //Within the zero threshold, the cursor does not move in the X.
    MouseXMovement = 0;
  }

  //If the analog Y reading is ouside of the zero threshold... 
  if( MovementThreshold < abs( acc_data->accY - ZeroYValue ) )
  {
    //...calculate Y mouse movement based on how far the Y acceleration is from its zero value.
    MouseYMovement = YSign * ( ( ( (float)( 2 * MaxMouseMovement ) / ( MaxYValue - MinYValue ) ) * ( acc_data->accY - MinYValue ) ) - MaxMouseMovement );
    //it could use some improvement, like making it trigonometric.
  }
  else
  {
    //Within the zero threshold, the cursor does not move in the Y.
    MouseYMovement = 0;
  }
 
    movement->XMovement = MouseXMovement;
	  movement->YMovement = MouseYMovement;
	  movement->clicked = userBtnPressed;
		userBtnPressed = 0;
}
