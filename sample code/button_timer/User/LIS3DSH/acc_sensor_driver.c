#include "spi.h"
#include "LIS3DSH.h"

// Khai bao cau truc du lieu luu thong tin doc tu cam bien
static Lis3dsh lis3dsh;

static uint8_t chip_id = 0;

void acc_sensor_init(void)
{	
		LIS3DSH_init(&lis3dsh);
	
		/* Doc ID cua chip */
		chip_id = LIS3DSH_read_chip_id();
}	

void acc_sensor_reading(void)
{
	float pitch, roll;
	
	/* Doc thong so xyz tu cam bien */
	LIS3DSH_readXYZ(&lis3dsh);
	
	/* Tin toan pitch roll */
	LIS3DSH_calc_pitch_roll(&lis3dsh, &pitch, &roll);
}		