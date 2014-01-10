#ifndef RTC_H
#define RTC_H
void PWR_BackupAccessCmd(FunctionalState NewState);
void BKP_DeInit(void);
void RTC_Configuration(void);
void get_time(void);
extern volatile unsigned long int stanRTC;
extern unsigned char stanRTCTekst[17];
#endif
