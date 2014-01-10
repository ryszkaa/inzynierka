#define LED_NUM     8                   /* Number of user LEDs                */

/*Definition of User_Button*/
//GPIOB
#define USER_BUTTON GPIO_Pin_7

void prvSetupHardware ( void );
void GPIO_Conf(void);
void ADC_Conf(void);
void SPI_Conf(void);
void RCC_Conf(void);
void NVIC_Conf(void);

