/**
  ******************************************************************************
  * @file    RFID.h
  * @created 11/02/1398 9:01:22 AM
  * @author  Mehdi Adham
  * @brief   This file contains all the functions RFID & password
  ******************************************************************************
  @verbatim     
  ==============================================================================      
                        ##### How to use this Library #####
  ============================================================================== 

  mode -> 

  Login ,
  NewCard ,
  NewPass ,
  DeleteCard ,
  DeleteAllCard ,
  DeletePass ,
  DeleteAllPass ,
  New_Admin_Card ,
  Change_Pass ,
  Change_Admin_Pass

  Example :
  if(mode == NewCard 
    New_Card();

  extern uint8_t Detect_card  varible in main.c or our file

  When you near the card Detect_card = 1 

  Add RFID_getCode(code)  Function in USART IRQ Handler 

  RFID Control:
   _________________________________________________________________________
  |      Command Name         |                     code                    |
  |___________________________|_____________________________________________|
  |         NewCard           |               *1#Near Card                  |
  |        DeleteCard         |               *3#Near Card                  |
  |       DeleteAllCard       |               *4#Near AdminCard             |
  |       New_Admin_Card      |                  ----------                 |
  |          NewPass          |               *2#Near yourCard|beep|pass    |
  |         DeletePass        |               *5#AdminPass#Pass#            |
  |       DeleteAllPass       |               *6#AdminPass#                 |
  |        Change_Pass        |               *8#Oldpass#NewPass#           |
  |      Change_Admin_Pass    |               *9#AdminPass#NewAdminPass#    |


  ******************************************************************************
  @endverbatim
  */


#include "main.h"
#include "RFID.h"
#include "RFID_Mem.h"
#include "string.h"

//Only for test on ram
uint8_t lock_flag=0;
uint8_t unsuccess_login_num=0;
extern uint8_t mode  ;
uint8_t card_code[RFID_Max_user][RFID_MAX_Byte_Card+RFID_MAX_Byte_Pass]={

{52,67,48,48,55,53,48,49,67 ,53 ,8,240},
{48,51,48,48,57,51,49,65,57 ,51}
};
int ncard=1;
uint8_t clr[10]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

uint8_t card_datacode[10];
uint8_t pass_code[2];
uint8_t npass;


uint8_t uart_counter=0 , uart_flag=0;
uint8_t RxBuffer[20]; 
uint16_t ReceiveBuffer = 0;
uint8_t Detect_card = 0;

uint8_t rcv_checksum = 0;
uint8_t _checksum = 0;
uint8_t val;
uint8_t rcv_val;
uint8_t  receive;


/* Private variables */
extern UART_HandleTypeDef huart1;
uint32_t RFID_CNT=0;
char str1[512]={'H','e','l','l','o'};//"Hello";
uint8_t flag=0;

//Card Function

void RFID_getCode(uint8_t code){

	RFID_CNT=RFID_TIMEOUT;

	if(flag == 0){

	if(code == RFID_RDM_630_HEADER){//02
		uart_counter = 0;
		receive = 0;
		_checksum = 0;
		rcv_checksum = 0;
		val = 0;
		RFID_CNT = RFID_TIMEOUT;
		//Detect_card = 0;
	}

	else if(code == RFID_RDM_630_TAIL){//03
		receive =1;
		flag=1;
		/*RFID_CNT=RFID_TIMEOUT;*/
	}


	if(uart_counter >= RFID_MAX_Card_BUFFER_SIZE ){//get data
		 uart_counter = 0;
	}

	RxBuffer[uart_counter++] = code;

	if(receive == 1){

		uint8_t H_VAL;
		uint8_t L_VAL;

		if(uart_counter == RFID_MAX_Card_BUFFER_SIZE){

			for(uint8_t i=1 ;i<10+1 ;i=i+2){

				if ((RxBuffer[i]>47) && (RxBuffer[i]<58))
					H_VAL = (16*(RxBuffer[i]-48));
				else if ((RxBuffer[i]>64) && (RxBuffer[i]<90)){
					switch(RxBuffer[i]){
					case 65:
						H_VAL = 10;
						break;
					case 66:
						H_VAL = 11;
						break;
					case 67:
						H_VAL = 12;
						break;
					case 68:
						H_VAL = 13;
						break;
					case 69:
						H_VAL = 14;
						break;
					case 70:
						H_VAL = 15;
						break;
					}
					H_VAL *= 16;
				}

				if ((RxBuffer[i+1]>47) && (RxBuffer[i+1]<58))
					L_VAL = RxBuffer[i+1]-48;
				else if ((RxBuffer[i+1]>64) && (RxBuffer[i+1]<90)){
					switch(RxBuffer[i+1]){
					case 65:
						L_VAL = 10;
						break;
					case 66:
						L_VAL = 11;
						break;
					case 67:
						L_VAL = 12;
						break;
					case 68:
						L_VAL = 13;
						break;
					case 69:
						L_VAL = 14;
						break;
					case 70:
						L_VAL = 15;
						break;
					}
				}

				val ^=  (H_VAL + L_VAL) ;//3
			}
			_checksum = val;



			//for (uint8_t i=11 ;i<13 ;i+=2){
			if ((RxBuffer[11]>47) && (RxBuffer[11]<58))
				H_VAL = (16*(RxBuffer[11]-48));
			else if ((RxBuffer[11]>64) && (RxBuffer[11]<90)){
				switch(RxBuffer[11]){
				case 65:
					H_VAL = 10;
					break;
				case 66:
					H_VAL = 11;
					break;
				case 67:
					H_VAL = 12;
					break;
				case 68:
					H_VAL = 13;
					break;
				case 69:
					H_VAL = 14;
					break;
				case 70:
					H_VAL = 15;
					break;
				}
				H_VAL *= 16;
			}

			if ((RxBuffer[12]>47) && (RxBuffer[12]<58))
				L_VAL = RxBuffer[12]-48;
			else if ((RxBuffer[12]>64) && (RxBuffer[12]<90)){
				switch(RxBuffer[12]){
				case 65:
					L_VAL = 10;
					break;
				case 66:
					L_VAL = 11;
					break;
				case 67:
					L_VAL = 12;
					break;
				case 68:
					L_VAL = 13;
					break;
				case 69:
					L_VAL = 14;
					break;
				case 70:
					L_VAL = 15;
					break;
				}
			}

			rcv_checksum = (H_VAL + L_VAL) ;




			if(_checksum == rcv_checksum){
				Detect_card = 1;

				for(int i=0;i<10;i++)
					card_datacode[i]=RxBuffer[i+1];
			}
		}
		receive=0;
	}
}
}

/**
  * @brief  None
  * @param  None
  * @retval None
  */
int Get_FREE_ADD(void){

	uint8_t IS_free;

	uint8_t card_data[10];

	for(int n=0 ;n < RFID_Max_user ; n++){
		Mem_Read(RFID_USER_CARD_START(n),card_data,RFID_MAX_Byte_Card);
		IS_free = 1;
		for(int i=0; i < RFID_MAX_Byte_Card ;i++){

			if(card_data[i] != 0xff){
				IS_free = 0;
				break;
			}
		}
		if(IS_free){
			return n;
		}
	}
	return -1;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
uint8_t Get_USER_Card_nn(void){
  uint8_t val;
  
  Mem_Read(RFID_USERS,&val,1);

  return val;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void checksum(uint8_t val){

}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
uint8_t Update_USER_Card_nn(void){
  uint8_t nn;
  
  nn= Get_USER_Card_nn()+1;
  
  Mem_Write(RFID_USERS ,&nn ,1);
  
  return nn;
}
uint8_t xxxx ;
/**
  * @brief  None
  * @param  None
  * @retval None
  */
uint8_t  New_Card(void){

	xxxx = Get_FREE_ADD();//Update_USER_Card_nn();

	HAL_Delay(5);

	Mem_Write(RFID_USER_CARD_START(xxxx),card_datacode,RFID_MAX_Byte_Card);

	HAL_Delay(5);
	//  for(uint8_t i=0 ;i < RFID_MAX_Byte_Card ;i++)
	//    card_code[ncard][i]=RxBuffer[i+1];

	return xxxx;
}

/**
  * @brief  None
  * @param  None
  * @retval None
  */
bool Card_Authentication(uint8_t *USERnn){
  
  uint8_t IS_user ;

  uint8_t card_data[10];

  uint8_t users = 0;
  
  ncard = RFID_Max_user;//Get_USER_Card_nn();

  users = ncard;

  for(int n=0 ;n < users ; n++){
    USERnn[0] = 1;
    IS_user = 1;
    Mem_Read(RFID_USER_CARD_START(n),card_data,RFID_MAX_Byte_Card);

    for(int i=0; i < RFID_MAX_Byte_Card ;i++){

      if(card_data[i] != card_datacode[i]){
        IS_user = 0;
        break;
      }
    }

    if(IS_user){
      USERnn[0] = n;
      lock_flag = 0;
      return true;
    }
  }
  if(++unsuccess_login_num>_unsuccess_login_num){
	  lock_flag=1;
  }

  return false;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void Delete_AllCard(void){

  //uint8_t reset = 0;

  for(uint16_t j=0; j<RFID_MAX_Byte_Card ;j++){
    
    Mem_Write(RFID_USER_CARD_START(j),clr,RFID_MAX_Byte_Card);
//	    for(uint8_t i=0; i< RFID_MAX_Byte_Card ;i++)
//	      card_code[j][i]=0xff;
  }
  ncard=0;
  //Mem_Write(0 ,&reset ,MAX_Byte_Card);
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
bool Delete_Card(uint8_t *USERnn){

  uint8_t nn=0;
 // ncard-=1;
  if(Card_Authentication(&nn) == HAL_OK){
    Mem_Write(RFID_USER_CARD_START(nn),clr,RFID_MAX_Byte_Card);
//    for(uint8_t i=0; i< RFID_MAX_Byte_Card ;i++)
//      card_code[nn][i]=0xff;
    USERnn[0] = nn;
    return true;
  }
  else
    return false;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
bool IS_Admin_Card(void){
  uint8_t n ;
  uint8_t nn;
  
  n = 0;//Get_Admin_Card_nn();
  if(Card_Authentication(&nn) == true){
    if(n==nn)
      return true;
    else 
      return false;
  }
  else
    return false;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
uint8_t Get_Admin_Card_nn(void){
  //uint8_t val;
  
  //Mem_Read(ADMIN_CARD_nn,&val,1);

  //if(val==0) val=1;
  //return val;
  return 0;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void Set_Admin_Card(void){
  uint8_t usernn;
  
  usernn = 0;/*New_Card();*/
  
  Mem_Write(usernn,card_datacode,10);
  
}

//Password Function
/**
  * @brief  None
  * @param  None
  * @retval None
  */
uint8_t Get_USER_Pass_nn(void){
  uint8_t val;
  
  Mem_Read(RFID_USER_Pass_nn,&val,1);
  
  sprintf(str1,"USER_nn=%u\n\r",val);
  
  HAL_UART_Transmit_IT(&huart1, (uint8_t *)str1, strlen(str1));
  //if(val==0) val=1;
  return val;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
uint8_t Update_USER_Pass_nn(void){
  uint8_t nn;
  
  nn = Get_USER_Pass_nn()+1;
  
  Mem_Write(RFID_USER_Pass_nn,&nn,1);
  
  sprintf(str1,"New_nn=%u\n\r",nn);
  HAL_UART_Transmit_IT(&huart1, (uint8_t *)str1, strlen(str1));
  
  return nn;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
uint8_t New_Pass(uint16_t PASSWORD,uint8_t USERnn){
  //uint8_t x;
  uint8_t PASS[2];
  PASS[0] = (uint8_t)(PASSWORD >>8);
  PASS[1] = (uint8_t)(PASSWORD);
  
  //x = Update_USER_Pass_nn();

//  pass[x]=PASSWORD;
	Mem_Write(RFID_USER_PASS_START(USERnn),PASS ,RFID_MAX_Byte_Pass);


//    card_code[USERnn][RFID_MAX_Byte_Card]=(uint8_t)(PASSWORD >>8);
//    card_code[USERnn][RFID_MAX_Byte_Card+1]=(uint8_t)(PASSWORD);


  return USERnn;

}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void Delete_AllPass(void){
	uint8_t users=0;
	  ncard = RFID_Max_user;//Get_USER_Card_nn();

	  users = ncard;
  
  for(int j=0; j< users ;j++){

	  Mem_Write(RFID_USER_PASS_START(j),clr ,RFID_MAX_Byte_Pass);

/*	  card_code[j][RFID_MAX_Byte_Card] = 0xff;
	  	    card_code[j][RFID_MAX_Byte_Card+1] = 0xff;*/
	  }
	 // ncard=0;

  
  //Mem_Write(1 ,&reset ,MAX_Byte_Pass);
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
bool Delete_Pass(uint8_t *USERnn ,uint16_t PASSWORD){
  uint8_t n=0;
  
  
  if(Pass_Authentication(&n ,PASSWORD) == true){
    
    Mem_Write(RFID_USER_PASS_START(n),clr ,RFID_MAX_Byte_Pass);
/*	    card_code[n][RFID_MAX_Byte_Card] = 0xff;
	    card_code[n][RFID_MAX_Byte_Card+1] = 0xff;*/

    USERnn[0] = n;

    return true;
  }
  else
    return false;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
bool IS_Admin_Pass(uint16_t PASSWORD){
  uint8_t n ;
  uint8_t nn;
  
  //n = Get_Admin_Pass_nn();
  n=0;
  if(Pass_Authentication(&nn ,PASSWORD) == true){
    if(n==nn)
      return true;
    else 
      return false;
  }
  else
    return false;
}
uint16_t PASS;
/**
  * @brief  None
  * @param  None
  * @retval None
  */
bool Pass_Authentication(uint8_t *USERnn ,uint16_t PASSWORD){
  int users = 0;

  ncard = RFID_Max_user;//Get_USER_Card_nn();

  users = ncard;

  for(int n=0 ;n < users ; n++){
    USERnn[0] = 1;
    Mem_Read(RFID_USER_PASS_START(n),pass_code,RFID_MAX_Byte_Pass);

	  if(PASSWORD == ( pass_code[0]<<8)  +  pass_code[1] ){
		  USERnn[0] = n;
		  return true;
	  }
  }

  if(++unsuccess_login_num>_unsuccess_login_num){
	  lock_flag=1;
  }
  return false;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
bool Change_Password(uint8_t *USERnn ,uint16_t Oldpassword ,uint16_t Newpassword){
  uint8_t nn;
  //uint8_t PASS[2];
//  PASS[0] = (uint8_t)(Newpassword >>8);
//  PASS[1] = (uint8_t)(Newpassword);
  
  if(Pass_Authentication(&nn ,Oldpassword) == true){
    
    //Mem_Write(USER_PASS_START(nn),PASS ,MAX_Byte_Pass);
	  New_Pass( Newpassword , nn);


    USERnn[0] = nn;
    
    return true;
  }
  else 
    return false;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
uint8_t Get_Admin_Pass_nn(void){
  uint8_t val;
  
  Mem_Read(RFID_ADMIN_PASS_nn,&val,1);
  
  sprintf(str1,"MANAGER_CARD_nn=%u\n\r",val);
  HAL_UART_Transmit_IT(&huart1, (uint8_t *)str1, strlen(str1));
  //if(val==0) val=1;
  return val;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void Set_Admin_Pass(uint16_t password){
  uint8_t nn;
  
  //nn = New_Pass(password);
  
  HAL_UART_Transmit_IT(&huart1, (uint8_t *)str1, strlen(str1));
  
  Mem_Write(RFID_ADMIN_PASS_nn,&nn,1);
  
}


