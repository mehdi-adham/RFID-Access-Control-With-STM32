#ifndef	_RFID_MEM_H
#define	_RFID_MEM_H


#include <stdbool.h>


extern I2C_HandleTypeDef hi2c2;

#define DevAddress hi2c2
//EEPROM24xx
#define MemAddress 0xA0
#define	MemAddSize I2C_MEMADD_SIZE_16BIT //I2C_MEMADD_SIZE_8BIT


#define                 RFID_ADMIN_CARD_nn                           (4)
#define                 RFID_ADMIN_PASS_nn                           (8)
#define                 RFID_USERS                                 (0)
#define                 RFID_LOG_nn                                  (2)
#define                 RFID_USER_Pass_nn                            (6)


#define					RFID_RDM_630_HEADER							(2)
#define 				RFID_RDM_630_TAIL							(3)


#define                 RFID_MAX_Byte_Pass                           (2)
#define                 RFID_MAX_Byte_log                            (9)

#define					RFID_MAX_Card_BUFFER_SIZE					(14)
#define                 RFID_MAX_Byte_Card                           (10)

#define                 RFID_Max_user                                (255)
#define                 RFID_Max_log                                 (255)
//#define                 LOGIN_PASS_START                        (16)
#define                 RFID_RFID_START_ADD                       	(64)


/*
#define                 RFID_USER_PASS_START(nn)                     (LOGIN_PASS_START+((nn)*MAX_Byte_Pass))
#define                 RFID_USER_PASS_END()                         (LOGIN_PASS_START+((Max_user)*MAX_Byte_Pass))//526
 */

#define                 RFID_Card_START                              (RFID_RFID_START_ADD)
#define                 RFID_USER_CARD_START(nn)                     (RFID_Card_START+((nn)*(RFID_MAX_Byte_Card + RFID_MAX_Byte_Pass)))
#define                 RFID_USER_CARD_END                         (RFID_Card_START+((RFID_Max_user)*(RFID_MAX_Byte_Card + RFID_MAX_Byte_Pass)))//3586

#define                 RFID_USER_PASS_START(nn)					(RFID_USER_CARD_START(nn) + RFID_MAX_Byte_Card)

#define                 RFID_LOG_START                               RFID_USER_CARD_END
#define                 RFID_LOG_Type_START(nn)                      (RFID_LOG_START+((nn)*(RFID_MAX_Byte_log)))
#define                 RFID_LOG_END()                             (RFID_LOG_START+((RFID_Max_log)*(RFID_MAX_Byte_log)))//5881


bool Mem_Write(uint16_t Address ,void *data ,size_t dataSize);
bool Mem_Read(uint16_t Address ,void *data ,size_t dataSize);


#endif
