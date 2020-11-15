#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "spi.h"
#include "LIS3DSH.h"


#include "afproto.h"
#include "gpio.h"

// Khai bao cau truc du lieu luu thong tin doc tu cam bien
static Lis3dsh lis3dsh;

void acc_sensor_init(void)
{	
	uint8_t chip_id = 0;
	
	LIS3DSH_init(&lis3dsh);
	/* Doc ID cua chip */
	chip_id = LIS3DSH_read_chip_id();

	if (chip_id != 0x3F)
	{
		while(1) {}
	}

	LIS3DSH_readSetting(&lis3dsh);
}	

void acc_sensor_led_indicator(Lis3dsh *acc_data)
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
	LIS3DSH_readXYZ(&lis3dsh);
	
	acc_sensor_led_indicator(&lis3dsh);
	/* Tin toan pitch roll */
//	LIS3DSH_calc_pitch_roll(&lis3dsh, &pitch, &roll);
}		

uint32_t acc_sensor_build_frame(char* out_buf)
{
	uint32_t write_len = 0;
	char  tmp_buf[128] = {0};
	
	sprintf(tmp_buf, "%d,%d,%d", lis3dsh.accX, lis3dsh.accY, lis3dsh.accZ);
	afproto_frame_data(tmp_buf, strlen(tmp_buf), out_buf, &write_len);
	
	return write_len;
}