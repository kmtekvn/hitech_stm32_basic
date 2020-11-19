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
	memset(_gFrameBuf, 0x00, MAX_FRAME_BUFFER);
	_gFrameLen = acc_sensor_build_frame(acc_data, _gFrameBuf);
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

uint32_t acc_sensor_build_frame(Lis3dsh *acc_data, char* out_buf)
{
	uint32_t write_len = 0;
	char  tmp_buf[128] = {0};
	
	sprintf(tmp_buf, "%d,%d,%d", acc_data->accX, acc_data->accY, acc_data->accZ);
	afproto_frame_data(tmp_buf, strlen(tmp_buf), out_buf, &write_len);
	
	return write_len;
}