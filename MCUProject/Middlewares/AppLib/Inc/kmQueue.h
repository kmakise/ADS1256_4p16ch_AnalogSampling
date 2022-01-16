/**
  ******************************************************************************
  * @file           : kmQueue.h
  * @brief          : Header for queue file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KMQUEUE_H
#define __KMQUEUE_H

/* Private includes ----------------------------------------------------------*/
#include "main.h"

/* Exported define -----------------------------------------------------------*/
#define RQ_QUEUE_MAXSIZE  		64
#define RQ_DATA_MAXSIZE  			32
#define RQ_DATATYPE 					uint8_t

/* Exported types ------------------------------------------------------------*/

typedef struct
{
	volatile RQ_DATATYPE 		buffer[RQ_QUEUE_MAXSIZE];		//���л�����
	volatile RQ_DATATYPE 		data[RQ_DATA_MAXSIZE];			//��ȡ����
	volatile uint32_t				pd;													//��ȡ�����±�
	volatile uint32_t 			pw;													//���л�����д���
	volatile uint32_t				pr;													//���л�������ȡ��
	volatile uint32_t 			errornum;										//��ʧ��
}RingQueueTypedef;



/* Exported functions prototypes ---------------------------------------------*/
uint32_t enRingQueueSingle(RingQueueTypedef * rq,RQ_DATATYPE data);
uint32_t enRingQueuePlural(RingQueueTypedef * rq,RQ_DATATYPE * pData,uint32_t size);
uint32_t deRingQueueSingleOut(RingQueueTypedef * rq,RQ_DATATYPE * pData);
uint32_t deRingQueueSingle(RingQueueTypedef * rq);
uint32_t deRingQueuePlural(RingQueueTypedef * rq);


#endif /* __KMQUEUE_H */

/************************ (C) COPYRIGHT kmakise ****************END OF FILE****/
