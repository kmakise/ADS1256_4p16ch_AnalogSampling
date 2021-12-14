/**
  ******************************************************************************
  * File Name          : MAX308.c
  * Description        : Code for MAX308 
  ******************************************************************************
  * @author  kmakise
  * @version V1.0.0
  * @date    2021-12-12
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
#include "MAX308.h"
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
//�趨ģ�⿪������ͨ��
void setSwitchChannel(ASW_OptTypedef * asw,uint8_t ch)
{
	if(	asw->A.Port == NULL || asw->A.Pin == NULL ||
			asw->B.Port == NULL || asw->C.Pin == NULL ||
			asw->B.Port == NULL || asw->C.Pin == NULL)
	{
		return;
	}
	switch(ch)
	{
		case 0:
			asw->A.Port->BSRR = asw->A.Pin << 16;
			asw->B.Port->BSRR = asw->B.Pin << 16;
			asw->C.Port->BSRR = asw->C.Pin << 16;
			break;
		case 1:
			asw->A.Port->BSRR = asw->A.Pin;
			asw->B.Port->BSRR = asw->B.Pin << 16;
			asw->C.Port->BSRR = asw->C.Pin << 16;
			break;
		case 2:
			asw->A.Port->BSRR = asw->A.Pin << 16;
			asw->B.Port->BSRR = asw->B.Pin;
			asw->C.Port->BSRR = asw->C.Pin << 16;
			break;
		case 3:
			asw->A.Port->BSRR = asw->A.Pin;
			asw->B.Port->BSRR = asw->B.Pin;
			asw->C.Port->BSRR = asw->C.Pin << 16;
			break;
		case 4:
			asw->A.Port->BSRR = asw->A.Pin << 16;
			asw->B.Port->BSRR = asw->B.Pin << 16;
			asw->C.Port->BSRR = asw->C.Pin;
			break;
		case 5:
			asw->A.Port->BSRR = asw->A.Pin;
			asw->B.Port->BSRR = asw->B.Pin << 16;
			asw->C.Port->BSRR = asw->C.Pin;
			break;
		case 6:
			asw->A.Port->BSRR = asw->A.Pin << 16;
			asw->B.Port->BSRR = asw->B.Pin;
			asw->C.Port->BSRR = asw->C.Pin;
			break;
		case 7:
			asw->A.Port->BSRR = asw->A.Pin;
			asw->B.Port->BSRR = asw->B.Pin;
			asw->C.Port->BSRR = asw->C.Pin;
			break;
		default:break;
	}
}
//�趨������ʽ 0 RMS 1 RSSI
void setCS_RMS_LOG(RL_OptTypedef * rl,uint8_t ch)
{
	if(	rl->RMS.Port == NULL || rl->RMS.Pin == NULL || 
			rl->LOG.Port == NULL || rl->LOG.Pin == NULL )
	{
		return;
	}
	switch(ch)
	{
		case 0:
			rl->RMS.Port->BSRR = rl->RMS.Pin;
			rl->LOG.Port->BSRR = rl->LOG.Pin << 16;
			break;
		case 1:
			rl->RMS.Port->BSRR = rl->RMS.Pin << 16;
			rl->LOG.Port->BSRR = rl->LOG.Pin;
			break;
		default:break;
	}
}

//�趨�Ŵ�λ
void setGainBeta(AG_OptTypedef * agt,AutoGainDataTypedef * agd,uint8_t ch)
{
		switch(ch)
		{
			case 1: // 1 V/V
				agd->ngain = 1;
				setSwitchChannel(&(agt->asw),0);
				break;
			case 2: // 10 V/V
				agd->ngain = 10;
				setSwitchChannel(&(agt->asw),1);
				break;
			case 3: // 100 V/V
				agd->ngain = 100;
				setSwitchChannel(&(agt->asw),2);
				break;
		}
}


//�Զ��Ŵ�������
void autoGainScanSingle(AG_OptTypedef * agt, AutoGainDataTypedef * agd,uint8_t ch,int32_t uv)
{
	if(!agd->en)//��ǰͨ���Ƿ�ʹ���Զ����ʵ���
	{
		return;
	}
		
	//�趨������ʽ 0 RMS 1 RSSI
	setCS_RMS_LOG(&(agt->rl),agd->cfg_mode);
	
	//���ڷŴ�������
	if(agd->cfg_mode == 0)//�Զ�ģʽ
	{
		//����AD637��⵽�ķ����������10����1���Ŵ�С��10mV,10���Ŵ�С��1mv,100���Ŵ�
		agd->uVmea = uv;
		agd->uVrel = uv / agd->ngain;
		
		if(agd->uVrel > 10000)//>10mv
		{
			setGainBeta(agt,agd,1);
		}
		else if(agd->uVrel > 1000)//>1mv
		{
			setGainBeta(agt,agd,2);
		}
		else // <1mv
		{
			setGainBeta(agt,agd,3);
		}
	}
	else//�趨ģʽ
	{
		setGainBeta(agt,agd,agd->cfg_mode);
	}
}


/* Private application code --------------------------------------------------*/

/************************ (C) COPYRIGHT kmakise ****************END OF FILE****/


