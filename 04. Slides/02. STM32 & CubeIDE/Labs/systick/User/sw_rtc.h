#ifndef  SW_RTC_H_
#define	 SW_RTC_H_

#include "common.h"

typedef struct{
	uint16_t second;
	uint16_t minute;
	uint16_t hour;
	uint16_t date;
	uint16_t month;
	uint16_t year;
}time_t;

void sw_rtc_update( void );
void sw_rtc_gettime( time_t* );

#endif /* SW_RTC_H_ */