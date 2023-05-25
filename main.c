int main(void)
{
	volatile unsigned char highDgt=0, lowDgt=0; //holds the high/low digits of the voltage
	int cmpVal = 0; //comparator value
	char LCDString[15]; //stores the LCD display message
	int loadRes = 0; //calculated resistance
	LCD_init(); //initialize LCD
	SSI1DAC_Init(); //initialize SSI
	SW1_init(); //initialize dip switch
	comInit(); //initialize communication
	while(1){
		
	if(GPIOF->DATA == 0){ //if pressed
		
		for(int digVolt = 1; digVolt <= 3600; digVolt++){
			
			GPIOD->DATA |= 0x02; //set PD2
			GPIOD->DATA &= ~0x02; //clear PD2
			lowDgt = (unsigned char)digVolt; //get low digit of voltage
			highDgt = (unsigned char)(digVolt>>8); //get high digit of voltage
			//write high/low voltage digits to SSI for DAC
			SSI1DAC_Write(highDgt);
			SSI1DAC_Write(lowDgt);
			delayMs(2);	
			cmpVal = GPIOC->DATA; //get voltage from comparator
			if((cmpVal & 0x10) == 0){ 
				loadRes = 1.72*236* 8192/(5 * digVolt); //calculate resistance
				LCD_command(0x01); //clear LCD display
				sprintf(LCDString, "%u", loadRes); //convert resistance to string
				LCD_str(LCDString); //display resistance
				LCD_str(" ohms");
				delayMs(10000); //wait 10 seconds
				break;
			}	
	}
		
		delayMs(1000); //wait 1 second
	}

	
}
}
