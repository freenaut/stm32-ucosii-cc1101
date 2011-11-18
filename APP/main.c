/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include <includes.h>
#include "cc1101.h"
#include <stdio.h>

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */
extern const GUI_BITMAP bmMicriumLogo; 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef __GUNC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];
static  OS_STK App_Task1Stk[APP_TASK_1_STK_SIZE];
static  OS_STK App_TaskCc1101Stk[APP_TASK_CC1101_STK_SIZE];
static  OS_STK App_TaskCc1101ReceiveStk[APP_TASK_CC1101_STK_SIZE];
/* Private function prototypes -----------------------------------------------*/
static  void App_TaskCreate(void);

static  void  App_TaskStart(void *p_arg);
static  void  App_Task1(void *p_arg);
void USART_Configuration(void);
/* Private functions ---------------------------------------------------------*/

void USART_Configuration(void)
{
/* USARTx configured as follow:
        - BaudRate = 9600 baud 
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
*/
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure USART1 Tx (PA9) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART1 Rx (PA10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	/* Enable USART1 */
	USART_Cmd(USART1, ENABLE);
}

/**
* @brief Retargets the C library printf function to the USART.
* @param None
* @retval None
*/
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
}



/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
	INT8U  os_err;
	OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel".         */

    os_err = OSTaskCreate((void (*)(void *)) App_TaskStart,  /* Create the start task.                               */
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO);

#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */

    return (0);
}

/**
  * @brief  App_TaskStart.
  * @param  None
  * @retval None
  */

static  void  App_TaskStart (void *p_arg)
{
	INT8U  os_err;
	GPIO_InitTypeDef GPIO_InitStructure;
	(void)p_arg;

	BSP_Init();                                                 /* Initialize BSP functions.                            */
	OS_CPU_SysTickInit();                                       /* Initialize the SysTick.                              */

#if (OS_TASK_STAT_EN > 0)
	OSStatInit();                                               /* Determine CPU capacity.                              */
#endif


  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

	USART_Configuration();
	/* Configure I/O for Flash Chip select */
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	CC1101_Init();
       CC1101_POWER_RESET();
	
	/* 在此可创建事件 */

	/* 在此可创建其他任务 */
	os_err = OSTaskCreate((void (*)(void *)) CC1101_Send_Main,  /* Create the start task.                               */
	                     (void          * ) 0,
	                     (OS_STK        * )&App_TaskCc1101Stk[APP_TASK_CC1101_STK_SIZE-1],
	                     (INT8U           ) APP_TASK_CC1101_PRIO+1);
#if 1
	os_err = OSTaskCreate((void (*)(void *)) CC1101_Receive_Main,  /* Create the start task.                               */
	                     (void          * ) 0,
	                     (OS_STK        * )&App_TaskCc1101ReceiveStk[APP_TASK_CC1101_STK_SIZE-1],
	                     (INT8U           ) APP_TASK_CC1101_PRIO);
#endif
	//	App_TaskCreate();
	//	TFT_Test();
	
	while (1) 
	{   
		printf("This is a example ! \r\n");
		OSTimeDlyHMSM(0, 0, 2, 0);
	}
}

//创建新任务
static  void App_TaskCreate(void)
{
    INT8U  os_err;

    os_err = OSTaskCreate((void (*)(void *)) App_Task1,  /* Create the start task.                               */
                             (void          * ) 0,
                             (OS_STK        * )&App_Task1Stk[APP_TASK_1_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_1_PRIO);
}

static  void  App_Task1(void *p_arg)
{
	(void)p_arg;
	
	while(1)
	{
		 MainTask();
//		 GUIDEMO_Touch();
	}	
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
