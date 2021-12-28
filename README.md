# RFID Access Control With STM32
RFID is commonly used in Access Control, using radio-frequency electromagnetic fields to allow the transfer data to identify people of objects without making physical contact.

### How to use:

```
while (1)
	{


		 HAL_Delay(100);
		/*********************************RFID MANAGE**************************/
		if(!lock_flag){

			if(mode == Login && Detect_card){
				/* Login */
				Detect_card=0;
				card_Manage(mode);
			}

			if(mode == NewCard && Detect_card){
				/* NewCard */
				Detect_card=0;
				card_Manage(mode);
				mode = Login;
			}

			if(mode == DeleteCard && Detect_card){
				/* DeleteCard */
				Detect_card=0;
				card_Manage(DeleteCard);
				mode = Login;
			}

			if(mode == DeleteAllCard && Detect_card){
				/* DeleteAllCard */
				Detect_card=0;
				card_Manage(DeleteAllCard);
				mode = Login;
			}
```
https://techforall.ir/su/VyOQ7n
