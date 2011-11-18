/* Includes ------------------------------------------------------------------*/
#include "tft.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void TFT_GPIOConfig(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @摘要  初始化TFT
  * @参数  None 
  * @返回值  None
  */
void TFT_Configuration()
{
	/*配置TFT管脚*/
	TFT_GPIOConfig();
//	/*初始化LCD*/
//	LCD_Initialization();
}
/**
  * @摘要  配置TFT的管脚
  * @参数  None 
  * @返回值  None
  */
void TFT_GPIOConfig()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*开启相应时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
							RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE| RCC_APB2Periph_AFIO, ENABLE);  
	/*所有Lcd引脚配置为推挽输出*/
	/*16位数据*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	/*控制脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	 //TP_CS
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
