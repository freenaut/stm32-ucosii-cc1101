/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
///******************************************************************************* 
//* Function Name  : EXTI_IRQHandler 
//* Description    : 按键中断
//*******************************************************************************/ 
//void EXTI2_IRQHandler(void)
//{
//    OS_CPU_SR  cpu_sr;
//
//    OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
//    OSIntNesting++;
//    OS_EXIT_CRITICAL();
//
//	if ( EXTI_GetITStatus(EXTI_Line2) != RESET )//检查是否真的发生中断
//	{
//		EXTI_ClearITPendingBit(EXTI_Line2);	//清除EXIT中断线挂起位
//		GPIO_WriteBit(GPIOD , GPIO_Pin_6 , (BitAction)!GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_6));//取反
//		Delay(0xfffff);//消除抖动
//	}
//
//    OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
//}
//void EXTI3_IRQHandler(void)
//{
//    OS_CPU_SR  cpu_sr;
//
//    OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
//    OSIntNesting++;
//    OS_EXIT_CRITICAL();
//
//	if ( EXTI_GetITStatus(EXTI_Line3) != RESET )//检查是否真的发生中断
//	{
//		EXTI_ClearITPendingBit(EXTI_Line3);	//清除EXIT中断线挂起位
//		GPIO_WriteBit(GPIOD , GPIO_Pin_13 , (BitAction)!GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_13));//取反
//		Delay(0xfffff);//消除抖动
//	}
//
//    OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
//}
//void EXTI4_IRQHandler(void)
//{
//    OS_CPU_SR  cpu_sr;
//
//    OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
//    OSIntNesting++;
//    OS_EXIT_CRITICAL();
//
//	if ( EXTI_GetITStatus(EXTI_Line4) != RESET )//检查是否真的发生中断
//	{
//		EXTI_ClearITPendingBit(EXTI_Line4);	//清除EXIT中断线挂起位
//		GPIO_WriteBit(GPIOC , GPIO_Pin_7 , (BitAction)!GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_7));//取反
//		Delay(0xfffff);//消除抖动
//	}
//
//    OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
//}
//void EXTI9_5_IRQHandler(void)
//{
//    OS_CPU_SR  cpu_sr;
//
//    OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
//    OSIntNesting++;
//    OS_EXIT_CRITICAL();
//
//	if ( EXTI_GetITStatus(EXTI_Line5) != RESET )//检查是否真的发生中断
//	{
//		EXTI_ClearITPendingBit(EXTI_Line5);	//清除EXIT中断线挂起位
//		GPIO_WriteBit(GPIOC , GPIO_Pin_6 , (BitAction)!GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_6));//取反
//		Delay(0xfffff);//消除抖动
//	}
//
//    OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
