
#include <stdint.h>

#ifndef TPE_ARQUI_RTC_H
#define TPE_ARQUI_RTC_H



#define RTC_SECONDS         0x00
#define RTC_SECONDS_ALARM   0x01
#define RTC_MINUTES         0x02
#define RTC_MINUTES_ALARM   0x03
#define RTC_HOURS           0x04
#define RTC_HOURS_ALARM     0x05
#define RTC_DAY_OF_WEEK     0x06
#define RTC_DAY_OF_MONTH    0x07
#define RTC_MONTH           0x08
#define RTC_YEAR            0x09

uint8_t get_rtc_seconds();
uint8_t get_rtc_minutes();
uint8_t get_rtc_hours();
uint8_t get_rtc_day_of_week();
uint8_t get_rtc_day_of_month();
uint8_t get_rtc_month();
uint8_t get_rtc_year();


#endif //TPE_ARQUI_RTC_H
