// this is a personal academic project. dear pvs-studio, please check it.
// pvs-studio static code analyzer for c, c++ and c#: http://www.viva64.com

#include <rtc.h>
#include <libasm.h>

static uint8_t bcd_decimal ( uint8_t bcd )
{
	return ( bcd >> 4 ) * 10 + ( bcd & 0x0f );
}

static uint8_t get_rtc ( uint8_t mode )
{
	return bcd_decimal ( rtc ( mode ) );
}

uint8_t get_rtc_seconds()
{
	return get_rtc ( RTC_SECONDS );
}

uint8_t get_rtc_minutes()
{
	return get_rtc ( RTC_MINUTES );
}

uint8_t get_rtc_hours()
{
	return get_rtc ( RTC_HOURS );
}

uint8_t get_rtc_day_of_week()
{
	return get_rtc ( RTC_DAY_OF_WEEK );
}

uint8_t get_rtc_day_of_month()
{
	return get_rtc ( RTC_DAY_OF_MONTH );
}

uint8_t get_rtc_month()
{
	return get_rtc ( RTC_MONTH );
}

uint8_t get_rtc_year()
{
	return get_rtc ( RTC_YEAR );
}


