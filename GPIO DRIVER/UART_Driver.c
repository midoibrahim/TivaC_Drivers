#include "UART_Driver.h"


void LEDS_Init(){
	SYSCTL_RCGCGPIO_R |= 0x00000020;      //Initialize clock to PORTF
	while((SYSCTL_PRGPIO_R&0x00000020) == 0){}  //safety for clock initialization
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x1F;       //Enable change to PORTF
	GPIO_PORTF_DIR_R = 0x0E;      //Make led ports as output
	GPIO_PORTF_DEN_R = 0x1F;      // digital enable to pins
	GPIO_PORTF_PUR_R = 0x11;

}

void UART_Init(){

	SYSCTL_RCGCUART_R|=0X0001;
	SYSCTL_RCGCGPIO_R |= 0x00000001;
	UART0_CTL_R &= ~0x0001;
	UART0_CC_R=0X0;
	UART0_IBRD_R=104;
	UART0_FBRD_R=11;
	UART0_LCRH_R=(0x3<<5);
	GPIO_PORTA_AFSEL_R|=0X03;
	GPIO_PORTA_PCTL_R=0X011;
	GPIO_PORTA_DEN_R|=0X03;
	UART0_CTL_R=0x0301;//9600 1 stopbit no parity no fifo
}


void toString (int tim, char text []){
		int number=tim;
	int digits=0;
	while (number) {
        number /= 10;
        digits++;
    }
	for (int j =0; j<digits; j++){
		text[j]='0';
	}
	//put numbers in char array
        int i = digits;
	while (tim != 0){
   i--;		
		int rem=tim%10;
		text[i]=rem+'0';
		tim=(tim/10)|0;
	}
	text[digits]='\0';//add null terminator
}

void printchar(char c){
while((UART0_FR_R&(1<<5))!=0);
UART0_DR_R=c;
}

void print(char *string){
  while(*string){
  printchar(*(string++));
  }
}