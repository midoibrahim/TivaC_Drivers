#include "GPIO_Driver.h"

void Led_On(uint8_t color){ 
   SET(DEREF(PORTF + GPIO_DATA),color);
}
void Led_off(uint8_t color){ 
 CLR(DEREF(PORTF + GPIO_DATA),color);
}

uint32_t SW_Input(void){     
  return (GPIO_PORTF_DATA_R&0x11);  // read PF4,PF0 inputs
}

void delay_ms (int nCount){ /* Wait function */ 
  nCount=nCount*16000; 
  while (nCount--);
}

//***********GPIO PIN DIRECTION MANIPULATION***************
void Pin_Dir_IN(uint32_t PortBaseAdd , uint8_t Pin)
{
    CLR_PIN(DEREF(PortBaseAdd +GPIO_DIR),Pin);
}

void Pin_Dir_OUT(uint32_t  PortBaseAdd , uint8_t Pin)
{
    SET_PIN(DEREF(PortBaseAdd +GPIO_DIR),Pin);
}

//**********GPIO PIN BITWISE MANIPULATION*************************
void PIN_BW_ON(uint32_t PortBaseAdd , uint8_t Pin)
{
  SET_PIN(DEREF(PortBaseAdd + PIN_B_Offset(Pin)),Pin);       
}

void PIN_BW_OFF(uint32_t PortBaseAdd , uint8_t Pin)
{
  CLR_PIN(DEREF(PortBaseAdd + PIN_B_Offset(Pin)),Pin);       
}

bool PIN_READ( uint32_t PortBaseAdd, uint8_t PIN)
{
return(READ_PIN(DEREF(PortBaseAdd + GPIO_DATA),PIN));
}

void PIN_PUR_ON(uint32_t PORTBaseAdd , uint8_t PIN)
{   
SET_PIN(DEREF(PORTBaseAdd + GPIO_PUR), PIN);  
}

void PIN_PUR_OFF(uint32_t PORTBaseAdd , uint8_t PIN)
{   
CLR_PIN(DEREF(PORTBaseAdd + GPIO_PUR), PIN);  
}

//**********GPIO PINS READ & WRITE******************************************
void GPIO_PINS_WRITE(uint32_t PortBaseAdd, uint8_t PINSVAL )
{
   SET(DEREF(PortBaseAdd + GPIO_DATA),PINSVAL);
}

void GPIO_PINS_CLEAR(uint32_t PortBaseAdd, uint8_t PINSVAL )
{
   CLR(DEREF(PortBaseAdd + GPIO_DATA),PINSVAL);
}

uint32_t GPIO_PINS_READ( uint32_t PortBaseAdd)
{
return(DEREF((PortBaseAdd + GPIO_DATA)));
}

//**********PORTF SWITCHES INITIALIZATION********************
void Switches_ON(){
GPIO_PORTF_PUR_R = (SW1 | SW2);
SET(GPIO_PORTF_DEN_R , (SW1 | SW2));
CLR(GPIO_PORTF_DIR_R ,(SW1 | SW2));
}

//**********GPIO PULL UP RESISTOR MANIPULATION********************
void GPIO_PUR_WRITE(uint32_t PORTBaseAdd , uint8_t PURVAL)
{   
SET(DEREF(PORTBaseAdd + GPIO_PUR), PURVAL);  
}

void GPIO_PUR_CLEAR(uint32_t PORTBaseAdd , uint8_t PURVAL)
{   
CLR(DEREF(PORTBaseAdd + GPIO_PUR), PURVAL);  
}

//**********PORT INITIALIZATION FUNCTION (PORT BASE ADDRESS , PINS DIRECTION , PINS DIGITAL ENABLE)****  
void GPIO_PORT_INIT(uint32_t PORTBaseAdd, uint8_t PINDIR, uint8_t PINDEN )
{
SYSCTL_RCGCGPIO_R |=  CLOCK(PORTBaseAdd); 
  while((SYSCTL_PRGPIO_R & CLOCK(PORTBaseAdd)) == 0){};  
  DEREF(PORTBaseAdd + GPIO_LOCK) = LOCK;
  DEREF(PORTBaseAdd + GPIO_CR)= 0x1F;
  DEREF(PORTBaseAdd + GPIO_DIR) = PINDIR;
  DEREF(PORTBaseAdd + GPIO_DEN) = PINDEN; 
}
 
 //**********GPIO PINS DIGITAL ENABLE FUNCTION****************
void GPIO_PINS_DEN(uint32_t PORTBaseAdd , uint8_t DENVAL)           
{
SET(DEREF(PORTBaseAdd + GPIO_DEN),DENVAL);
CLR(DEREF(PORTBaseAdd + GPIO_AMSEL),DENVAL);
}          

//**********GPIO PINS ANALOGUE ENABLE FUNCTION****************
void GPIO_PINS_AM(uint32_t PORTBaseAdd , uint8_t AMVAL)
{
SET(DEREF(PORTBaseAdd + GPIO_AMSEL),AMVAL);
CLR(DEREF(PORTBaseAdd + GPIO_DEN),AMVAL);
}            

//**********GPIO PINS DIRECTION FUNCTIONS**********************
void GPIO_PINS_DIR_WRITE(uint32_t PORTBaseAdd , uint8_t DIRVAL)
{
SET(DEREF(PORTBaseAdd + GPIO_DIR),DIRVAL);  
}

void GPIO_PINS_DIR_CLEAR(uint32_t PORTBaseAdd , uint8_t DIRVAL)
{
CLR(DEREF(PORTBaseAdd + GPIO_DIR),DIRVAL);  
}

uint32_t GPIO_PINS_DIR_READ( uint32_t PortBaseAdd)
{
return(DEREF((PortBaseAdd + GPIO_DIR)));
}