#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "ADC.h"
#include "LCD.h"
#include "UART.h"


#define ON 1
#define OFF 0

//Type Defined Struct To Hold The Actual Temprature and Setpoint
	typedef struct Message{
char Txt1[4]; 
char Txt2[4];
}AMessage;

//initialize Queues to Send Message between Tasks
QueueHandle_t xUARTQueue;
QueueHandle_t xLCDQueue;
QueueHandle_t xBuzzerQueue;

//Binary Semaphore to Wake Up the Adc_read_task
SemaphoreHandle_t xSemaphore;

const TickType_t xDelayinf = portMAX_DELAY;


bool User=1;

void ADC_Read_Task(){

	char *on; 
char off=0;
unsigned const char AlarmValue = 27;
	AMessage msg;
	xSemaphoreTake( xSemaphore, 0 );
	unsigned char Temp;
	unsigned char setpoint=30;
	on=ON;
	off=OFF;
	
	while(1){
		
		//Wait For Binary Semaphore from ISR 
		xSemaphoreTake( xSemaphore, portMAX_DELAY );
		
		//Take the SETPOINT From user First Time
		if(User==1)
		{
		xQueueReceive(xUARTQueue,	&setpoint, portMAX_DELAY);		// Receive data
			User= 0;
		}
		
		Temp=LM35_Value(); 							//Read Actual Temprature
		
		
		if(Temp < setpoint){									//If cold		
			GPIO_PORTF_DATA_R |= 0x02;							//Heater LED ON
		}
		else																				//If hot
		{			
			GPIO_PORTF_DATA_R &=~ 0x02;									//Heater Led OFF
		}
		
		
		toString(Temp,msg.Txt1);							//Actual value
		toString(setpoint, msg.Txt2);							//setpoint 
		
		//Send in QUEUES For UART & LCD
		xQueueSendToBack(xUARTQueue,&msg,0);
		xQueueSendToBack(xLCDQueue,&msg,0);
		
			if(Temp > AlarmValue){ 								//Alarm?
					xQueueSend(xBuzzerQueue, &on, 0); 				//buzzer ON
			}
			else{
					xQueueSend(xBuzzerQueue,&off,0);						//Buzzer off
					}
	 }
 }


void UART_TASK(void){

	unsigned N;
	unsigned AdcValue;
	unsigned char Total; 
  while(1){
	  	//Take User Input First Time
		if (User==1)
					{print("\n\r\nEnter Temperature Setpoint (Degrees): ");
						N=0;
						Total=0;
						while(1){
							while((UART0_FR_R&(1<<4))!=0);			// read from the user the setpoint number
							N= UART0_DR_R;
							print(&N);
							if(N=='\r') break;
							N=N-'0';
							Total=10*Total+N;				
						}
					 
					//Send to the ADC read Task
					xQueueSend(xUARTQueue,&Total,pdMS_TO_TICKS(10));
					print("\n\rTemperature Setpoint changed...");	
					}
	  
	  //Display the Actual and Setpoint in Terminal
	  else{
		
				AMessage msg;
				xQueueReceive(xUARTQueue,&msg,xDelayinf);
			  print("\n\r\nTemperature is : ");
				print(msg.Txt1);
				print(" Degree Celsius");			
		print(" Setpoint: ");		
	print(msg.Txt2);		
	}	
}
	}



 //ADC Interrupt Handler to Give Binary Semaphore for ADC_READ_TASK every 1 sec
void ADC0SS3_Handler(void){
	BaseType_t xHigherPriorityTaskWoken= pdFALSE;
	xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken );
	ADC0_ISC_R = 8;          /* clear coversion clear flag bit*/
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);

}



void LCD_TASK(void){
	AMessage msg;
    while(1){	
	    		//receive msg from queue and display 
			xQueueReceive(xLCDQueue,&msg,xDelayinf);
			LCD_line(1); 
			LCD_display("Measure: ");   
			LCD_display(msg.Txt1);  		
			LCD_line(2);
			LCD_display("Setpoint: "); 		
			LCD_display(msg.Txt2); 			
 		
		}	
}


void Buzzer(void *pvParameters){
unsigned char Buzzerstate;
	Buzzerstate=0;
    while(1){
	    		//receive Buzzer State from Queue 
			xQueueReceive(xBuzzerQueue,&Buzzerstate,0);
			if(Buzzerstate==1){
				GPIO_PORTF_DATA_R&=~0x02;
				GPIO_PORTF_DATA_R|=0x04;			//Buzzer & Buzzer Led ON 
			}
			else{
				GPIO_PORTF_DATA_R&=~0x04;			//Buzzer & Buzzer Led OFF
			}
    }
}





int main(void)
{	
	//Initialization Functions
	LEDS_Init();
	UART_Init();
	LM35_Init();	
	LCD_start();
	LCD_clear();
	
	//Create Queues 
	xUARTQueue = xQueueCreate( 10,sizeof(AMessage) );
	xLCDQueue= xQueueCreate(10,sizeof(AMessage) );
	xBuzzerQueue= xQueueCreate(10,sizeof(int) );
	
	//Create Binary Semaphore
	vSemaphoreCreateBinary(xSemaphore);
	
	//Create FreeRTOS Tasks
	xTaskCreate(ADC_Read_Task,"ADC_Read_Task",100,NULL,3,0);
	xTaskCreate(LCD_TASK,"LCD_TASK",100,NULL,2,0);
	xTaskCreate(Buzzer,"Buzzer_TASK",100,NULL,2,0);
	xTaskCreate(UART_TASK,"UART_TASK",100,NULL,2,0);
	
	//Start Scheduler
	vTaskStartScheduler();

}
