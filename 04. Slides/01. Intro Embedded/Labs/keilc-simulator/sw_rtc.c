#include "sw_rtc.h"

static time_t t;

static char not_leap(void);

/**
 * Update and calculate time.
 */
void sw_rtc_update( void )
{
	if(++t.second == 60)
	{
		t.second = 0;

		if(++t.minute == 60)
		{
			t.minute = 0;

			if(++t.hour == 24)
			{
				t.hour = 0;

				if(++t.date == 32)
				{
					t.month++;
					t.date=1;
				}
				else if(t.date == 31)
				{
					if((t.month == 4) || (t.month == 6) || (t.month == 9) ||
							(t.month == 11))
					{
						t.month++;
						t.date = 1;
					}
				}
				else if(t.date == 30)
				{
					if(t.month == 2)
					{
						t.month++;
						t.date = 1;
					}
				}
				else if (t.date == 29)
				{
					if((t.month == 2) && (not_leap()))
					{
						t.month++;
						t.date = 1;
					}
				}

				if(t.month == 13)
				{
					t.month = 1;
					t.year++;
				}
			}
		}
	}
}

void sw_rtc_gettime( time_t* cur_time_ptr)
{
	/* Check NULL pointer */
	if (cur_time_ptr == 0)
	{
		return;
	}
	
	cur_time_ptr->second = t.second;
	cur_time_ptr->minute = t.minute;
	cur_time_ptr->hour = t.hour;
	cur_time_ptr->date = t.date;
	cur_time_ptr->month = t.month;
	cur_time_ptr->year = t.year;
}

/**
 * Check for leap year.
 *
 * \return	0 if leap year.
 */
static char not_leap(void)
{
	if(!(t.year%100))
	{
		return (char)(t.year % 400);
	}
	else
	{
		return (char)(t.year % 4);
	}
}