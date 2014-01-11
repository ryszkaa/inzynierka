#ifndef RTC_H
#define RTC_H
void PWR_BackupAccessCmd(FunctionalState NewState);
void BKP_DeInit(void);
void RTC_Configuration(void);
extern volatile uint8_t AlarmStatus;
extern volatile unsigned long int StanAlarm ;

#define ALARM_5MIN 300
#endif
