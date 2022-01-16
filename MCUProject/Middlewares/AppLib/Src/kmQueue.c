/**
  ******************************************************************************
  * File Name          : kmQueue.c
  * Description        : Code for queue lib
  ******************************************************************************
  * @author  kmakise
  * @version V1.0.0
  * @date    2021-12-13
  * @brief   
	******************************************************************************
  * @attention
  * <h2><center>&copy; Copyright (c) kmakise
  * All rights reserved.
  *
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "kmQueue.h"



/* Private function prototypes -----------------------------------------------*/
//������ ���� ���ش����ۻ�
uint32_t enRingQueueSingle(RingQueueTypedef * rq,RQ_DATATYPE data)
{
	if((rq->pw + 1) != rq->pr)
	{
		rq->buffer[rq->pw] = data;
		rq->pw = (rq->pw < (RQ_QUEUE_MAXSIZE - 1)) ? rq->pw + 1 : 0;
		return 1;
	}
	else
	{
		rq->errornum++;
		return rq->errornum;
	}
	return 0; 
}
//������ ���� ���ش����ۻ�
uint32_t enRingQueuePlural(RingQueueTypedef * rq,RQ_DATATYPE * pData,uint32_t size)
{
	for(int i = 0;i < size;i++)
	{
		if(enRingQueueSingle(rq,*pData++) == 0)
		{
			return i;
		}
	}
	return 0;
}

//������ �����ṩ
uint32_t deRingQueueSingleOut(RingQueueTypedef * rq,RQ_DATATYPE * pData)
{
	if(rq->pr != rq->pw)
	{
		*pData = rq->buffer[rq->pr];
		rq->pr = (rq->pr < (RQ_QUEUE_MAXSIZE - 1)) ? rq->pr + 1 : 0;
		return 1;
	}
	else
	{
		return 0;
	}
	return 0;
}
//������ ����
uint32_t deRingQueueSingle(RingQueueTypedef * rq)
{
	if(rq->pr != rq->pw)
	{
		rq->data[rq->pd++] = rq->buffer[rq->pr];
		rq->pr = (rq->pr < (RQ_QUEUE_MAXSIZE - 1)) ? rq->pr + 1 : 0;
		return 1;
	}
	else
	{
		return 0;
	}
	return 0;
}
//������ ���� ���ض�ȡ����
uint32_t deRingQueuePlural(RingQueueTypedef * rq)
{
	uint32_t len = 0; 
	while(deRingQueueSingle(rq) == 1)
	{
		len = (len + 1) % RQ_DATA_MAXSIZE;
	}
	return len;
}



/* Private application code --------------------------------------------------*/

/************************ (C) COPYRIGHT kmakise ****************END OF FILE****/


