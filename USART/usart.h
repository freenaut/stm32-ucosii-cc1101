/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USART1_Configuration(void);
void USART_SendString(USART_TypeDef *  USARTx, char * format);

#endif /* __USART_H */
