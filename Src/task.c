/**
  ******************************************************************************
  * @file    task.h
  * @created 11/02/1398 9:01:22 AM
  * @author  Mehdi Adham
  * @brief   This file contains all the functions Event
  ******************************************************************************
  */

#include "main.h"
#include "task.h"
#include "RFID.h"

extern uint8_t get_rfid ,get_esp;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern uint32_t RFID_CNT;
extern uint8_t  Detect_card;

extern uint8_t flag;

uint32_t lock_time_cnt=0;
uint16_t lock_time=0;

extern uint8_t lock_flag;
extern uint8_t unsuccess_login_num;

extern TIM_HandleTypeDef htim2;




void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  if(huart -> Instance == USART1){
//    HAL_UART_Transmit_IT(&huart1, (uint8_t *)0x00, 1);
  }
  UNUSED(huart);
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  if(huart -> Instance == USART2){
  }
  if(huart ->Instance ==USART1){
    RFID_getCode(get_rfid);
    HAL_UART_Receive_IT(&huart1,&get_rfid,1);
  }
  UNUSED(huart);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if(htim -> Instance == TIM15){
	  if(--RFID_CNT==0){
	  flag=0;
	}


	  if(++lock_time_cnt>=60000){
		  lock_time_cnt=0;
		  if(++lock_time>_lock_time){
			  lock_time=0;
			  unsuccess_login_num=0;
			  lock_flag=0;
		  }
	  }


  }


  UNUSED(htim);
}






