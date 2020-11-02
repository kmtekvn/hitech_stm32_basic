
#include "spi.h"
#include "LIS3DSH.h"
#include <math.h>

#define M_PI (3.14)

/*Khai bao con tro ham de link toi ham xu ly driver tuong ung tung MCU*/
void (*SPI_Write_Reg)(uint8_t reg, uint8_t *data, uint16_t len);
void (*SPI_Read_Reg)(uint8_t reg, uint8_t *data, uint16_t len);
void (*SPI_CS_Set)(void);
void (*SPI_CS_Reset)(void);

void LIST3DSH_register_ll_function(void);

//LIS3DSH initialization function. 
// Writes a value of 0x67 into control register to initialize accelerometer.
void LIS3DSH_init(Lis3dsh *lis3dsh) 
{
	uint8_t data = 0x67;
	
	/* Dang ky ham ham driver*/
	LIST3DSH_register_ll_function();
	
	/* Cau hinh ban dau cho cam bien*/
	SPI_CS_Reset();
	SPI_Write_Reg(CR4, &data, 2);
	SPI_CS_Set();
	
	/*Khoi tao cau truc du lieu ban dau*/
	lis3dsh->accX = 0;
	lis3dsh->accY = 0;
	lis3dsh->accZ = 0;

}
/* Dang ky ham low level driver */
void LIST3DSH_register_ll_function(void)
{
	SPI_Read_Reg = _spi_read_reg;
	SPI_Write_Reg = _spi_write_reg;
	SPI_CS_Reset = _spi_cs_reset;
	SPI_CS_Set = _spi_cs_set;
}

//function for reading accelerometer reading on X, Y and Z axes.
uint8_t LIS3DSH_read_chip_id(void)
{
	//variables used to hold raw accelerometer readings
	uint8_t spi_rx_buf;

	SPI_CS_Reset();
	//Start reading from X_L register. 
	//Subsequent registers hold X_H, Y_L, Y_H, and Z_L, Z_H values. 
	// Store these values in rx_buffer
	SPI_Read_Reg(WHOAMI, &spi_rx_buf, 1);
	SPI_CS_Set();
	
	return spi_rx_buf;
}

//function for reading accelerometer reading on X, Y and Z axes.
void LIS3DSH_readXYZ(Lis3dsh *lis3dsh)
{
	//variables used to hold raw accelerometer readings
	int16_t x, y, z;
	uint8_t spi_rx_buf[6];

	SPI_CS_Reset();

	//Start reading from X_L register. 
	//Subsequent registers hold X_H, Y_L, Y_H, and Z_L, Z_H values. 
	//Store these values in rx_buffer
	SPI_Read_Reg(OUTX_L, spi_rx_buf, 6);

	SPI_CS_Set();

	//complete accelerometer reading(signed 16 bits) is made up of 2 unsigned 8 bit readings(LOW and HIGH registers).
	//concatenate these 2 readings into a signed, 16 bit variable to construct actual value
	x = (spi_rx_buf[1] << 8) | spi_rx_buf[0] ;
	y = (spi_rx_buf[3] << 8) | spi_rx_buf[2] ;
	z = (spi_rx_buf[5] << 8) | spi_rx_buf[4] ;

	//set accelerometer readings of struct to the ones derived above, scaled by a factor of 2000 as configured.
	lis3dsh->accX = x / 2000.0;
	lis3dsh->accY = y / 2000.0;
	lis3dsh->accZ = z / 2000.0;
}

//calculates pitch and roll in degrees from accelerometer readings
void LIS3DSH_calc_pitch_roll(Lis3dsh *lis3dsh, float *pitch, float *roll)
{
	*pitch = 180 * atan2(lis3dsh->accY, lis3dsh->accZ) / M_PI;
	*roll = 180 * atan2(lis3dsh->accX, lis3dsh->accZ) / M_PI;
}


