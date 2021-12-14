/**
  ******************************************************************************
  * @file           : MAX308.h
  * @brief          : Header for MAX308.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAX308_H
#define __MAX308_H

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "kmGlobal.h"
/* Exported define -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	GPIO_OptTypedef		 A;
	GPIO_OptTypedef		 B;	
	GPIO_OptTypedef		 C;
	
}ASW_OptTypedef;
typedef struct
{
	GPIO_OptTypedef		 RMS;
	GPIO_OptTypedef		 LOG;	
	
}RL_OptTypedef;

typedef struct
{
	ASW_OptTypedef		 asw;
	RL_OptTypedef			 rl;
}AG_OptTypedef;

typedef struct
{
	int32_t   uVmea;		//������ѹ
	double_t  uVrel;		//�����ѹ
	double_t  ngain;		//�Ŵ���
	uint8_t   cfg_gain; //��������
	uint8_t   cfg_mode;	//ģʽ����
	uint8_t   en;				//ʹ��
	
}AutoGainDataTypedef;

/* Exported functions prototypes ---------------------------------------------*/
//�Զ��Ŵ�������
void autoGainScanSingle(AG_OptTypedef 			* agt	, 
												AutoGainDataTypedef * agd	,
												uint8_t 							ch	,
												int32_t 							uv	);


#endif /* __FILTER_H */

/************************ (C) COPYRIGHT kmakise ****************END OF FILE****/
