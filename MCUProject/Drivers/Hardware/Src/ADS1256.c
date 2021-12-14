/**
  ******************************************************************************
  * File Name          : ADS1256.c
  * Description        : Code for ADS1256 bsp
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
#include "main.h"
#include "stdio.h"
#include "ADS1256.h"
#include "spi.h"
/* Private variables ---------------------------------------------------------*/
ADC_ConfigTypedef pCfgADCParam;
ADC_DataTypedef pADCHandle[ADS1256_PORT_NUM] = {
	
	[0].id 												= 0,
	[0].hwinfc.hspi 							= &hspi1,
	[0].hwinfc.drdy.Port					= DRDY_A_GPIO_Port,
	[0].hwinfc.drdy.Pin						=	DRDY_A_Pin,
	[0].hwinfc.agt.asw.A.Port			= NULL,
	[0].hwinfc.agt.asw.A.Pin			= NULL,
	[0].hwinfc.agt.asw.B.Port			= NULL,
	[0].hwinfc.agt.asw.B.Pin			= NULL,
	[0].hwinfc.agt.asw.C.Port			= NULL,
	[0].hwinfc.agt.asw.C.Pin			= NULL,
	[0].hwinfc.agt.rl.RMS.Port		= NULL,
	[0].hwinfc.agt.rl.RMS.Pin			= NULL,
	[0].hwinfc.agt.rl.LOG.Port		= NULL,
	[0].hwinfc.agt.rl.LOG.Pin			= NULL,
	
};

/* Private function prototypes -----------------------------------------------*/
//д�Ĵ���
void ADS1256WREG(ADC_HWInterface * hi,uint8_t regaddr,uint8_t databyte)
{
	uint8_t cmd;
	while(HAL_GPIO_ReadPin(hi->drdy.Port,hi->drdy.Pin));
	//��Ĵ���д�����ݵ�ַ
	cmd = ADS1256_CMD_WREG|(regaddr & 0x0F);
	HAL_SPI_Transmit(hi->hspi, &cmd, 1, 10);
  //д�����ݵĸ���n-1
	cmd = 0x00;
	HAL_SPI_Transmit(hi->hspi, &cmd, 1, 10);
  //��regaddr��ַָ��ļĴ���д������databyte
  HAL_SPI_Transmit(hi->hspi, &databyte, 1, 10);
}
//д����
void ADS1256WCMD(ADC_HWInterface * hi,uint8_t cmd)
{
	HAL_SPI_Transmit(hi->hspi, &cmd, 1, 10);
}

//��ȡADֵ������ͨ��
int32_t ADS1256ReadData(ADC_HWInterface * hi,uint8_t channel)  
{
	int32_t sum;
  uint8_t data[3];

  while(HAL_GPIO_ReadPin(hi->drdy.Port,hi->drdy.Pin));
  //�л�ͨ�� ע�⣺����д�����ͨ�����´ζ�ȡʱ�Ż���Ч�����ζ���������һ�ε�ֵ
  switch(channel)
  {
    case 0:ADS1256WREG(hi,ADS1256_MUX, ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);break;
    case 1:ADS1256WREG(hi,ADS1256_MUX, ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM);break; 
    case 2:ADS1256WREG(hi,ADS1256_MUX, ADS1256_MUXP_AIN2 | ADS1256_MUXN_AINCOM);break; 
    case 3:ADS1256WREG(hi,ADS1256_MUX, ADS1256_MUXP_AIN3 | ADS1256_MUXN_AINCOM);break; 
    case 4:ADS1256WREG(hi,ADS1256_MUX, ADS1256_MUXP_AIN4 | ADS1256_MUXN_AINCOM);break; 
    case 5:ADS1256WREG(hi,ADS1256_MUX, ADS1256_MUXP_AIN5 | ADS1256_MUXN_AINCOM);break; 
    case 6:ADS1256WREG(hi,ADS1256_MUX, ADS1256_MUXP_AIN6 | ADS1256_MUXN_AINCOM);break; 
    case 7:ADS1256WREG(hi,ADS1256_MUX, ADS1256_MUXP_AIN7 | ADS1256_MUXN_AINCOM);break; 
  }
  ADS1256WCMD(hi,ADS1256_CMD_SYNC);
  ADS1256WCMD(hi,ADS1256_CMD_WAKEUP);
  ADS1256WCMD(hi,ADS1256_CMD_RDATA);
  HAL_SPI_Receive(hi->hspi, data, 3, 10);
  sum = (unsigned int)(data[0]<<16)|(data[1]<<8)|data[2];
	sum |= (sum & 0x800000) ? 0xFF000000 : 0;
  return sum;
}
//��������
void ADCHandleCfgInit(ADC_DataTypedef * adc)
{
	for(int i = 0;i < 8;i++)
	{
		adc->filter[i].meanNum  = pCfgADCParam.mean[adc->id][i];
		adc->atgain[i].cfg_mode = pCfgADCParam.mode[adc->id][i];
		adc->atgain[i].cfg_gain = pCfgADCParam.gain[adc->id][i];
		adc->atgain[i].ngain		= 1;
		adc->atgain[i].en = 0;
	}
	adc->atgain[0].en = 1;
	adc->atgain[2].en = 1;
	adc->atgain[4].en = 1;
	adc->atgain[6].en = 1;
	
	adc->state = ADS1256_INIT_OK;
}

//��ʼ��ADS1256
void ADS1256_Init(ADC_DataTypedef * adc)
{
	ADS1256WREG(&(adc->hwinfc),ADS1256_STATUS,0x00);//��λ��ǰ��������У׼�����û���
	ADS1256WREG(&(adc->hwinfc),ADS1256_ADCON,ADS1256_GAIN_1 );// �Ŵ���1
	ADS1256WREG(&(adc->hwinfc),ADS1256_DRATE,ADS1256_DRATE_30000SPS);
	ADS1256WREG(&(adc->hwinfc),ADS1256_MUX, ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);// ��ʼ���˿�A0Ϊ��+����AINCOMλ��-��
	ADCHandleCfgInit(adc);
}
/* Private application code --------------------------------------------------*/

//�ⲿ�ж�ɨ���ȡ
void ADC_ISR_Read(ADC_DataTypedef * adc)
{
	int32_t ulResult = 0;
	
	if(adc->state == ADS1256_INIT_OK)
	{
		//����ɨ��ʱ��
		if(adc->ch == 0)
		{
			adc->tTick = HAL_GetTick() - adc->lTick;
			adc->lTick = HAL_GetTick();
		}
		//��ȡADC���л�ͨ��
		ulResult = ADS1256ReadData(&(adc->hwinfc),(adc->ch + 1) % ADS1256_CH_NUM);
		// ����ADC
		adc->adcVal[adc->ch] = ulResult;
		//��ֵ�˲���
		meanFilterSingleWrite(&(adc->filter[adc->ch]),ulResult);
		//�����ѹ vref2.5v uV
		adc->uV[adc->ch] = (long double)adc->filter[adc->ch].meanVal *ADS1256_ADC_UV;
		//�Զ������� 0 2 4 6 �Զ�����
		autoGainScanSingle(&(adc->hwinfc.agt),&(adc->atgain[adc->ch]),adc->ch,adc->uV[adc->ch]);
		//�ƶ�ɨ��ͨ��
		adc->ch = (adc->ch < (ADS1256_CH_NUM - 1)) ? adc->ch + 1:0;
	}
}

//0���ⲿ�ж�
void EXIT_ADC_DRDY_A(void)
{
	ADC_ISR_Read(&pADCHandle[0]);
}

/************************ (C) COPYRIGHT kmakise ****************END OF FILE****/

