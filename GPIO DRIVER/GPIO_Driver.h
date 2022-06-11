#ifndef GPIO_Driver_H
#define GPIO_Driver_H

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>

void Led_On(uint8_t color);
void Led_off(uint8_t color);
uint32_t SW_Input(void);
void delay_ms (int nCount);

#define DARK 	(0U)
#define RED 	(1U << 1)
#define BLUE  	(1U << 2)
#define GREEN 	(1U << 3)
#define YELLOW 	(1U << 1)|(1U << 3)
#define SKYBLUE (1U << 2)|(1U << 3)
#define PINK 	(1U << 1)|(1U << 2)
#define WHITE 	(1U << 1)|(1U << 2)|(1U << 3)

//*********PORTF SWITCHES VALUES**********************************
#define SW1 0x01
#define SW2 0x10
#define SWS 0x00

//*********LEDS DIRECTION VALUE***********************************
#define LED_OUTPUT 0x0e

// ***************GPIO PORTS BASE ADDRESSES***********************
#define PORTA  0x40004000
#define PORTB  0x40005000
#define PORTC  0x40006000
#define PORTD  0x40007000
#define PORTE  0x40024000
#define PORTF  0x40025000

//****************GPIO PINS *************************************
#define GPIO_PIN_0              0x00000001  // GPIO pin 0
#define GPIO_PIN_1              0x00000002  // GPIO pin 1
#define GPIO_PIN_2              0x00000004  // GPIO pin 2
#define GPIO_PIN_3              0x00000008  // GPIO pin 3
#define GPIO_PIN_4              0x00000010  // GPIO pin 4
#define GPIO_PIN_5              0x00000020  // GPIO pin 5
#define GPIO_PIN_6              0x00000040  // GPIO pin 6
#define GPIO_PIN_7              0x00000080  // GPIO pin 7

#define PIN_B_Offset(n)         ( 1U<< ( 2U+(n)) )  //Bitwise Pin manipulation OFFSET
#define OFF        (0U)
#define ON         (1U)
#define GPIO_MODE_IN        0x00000000  // Pin is a GPIO input
#define GPIO_MODE_OUT       0x00000001  // Pin is a GPIO output
#define GPIO_MODE_HW        0x00000002  // Pin is a peripheral function
#define LOCK 				0x4C4F434B

//***************GPIO CLOCK ENABLE VALUE ***************************************
#define CLOCK(PORTA)              0x1
#define CLOCK(PORTB)              0X2
#define CLOCK(PORTC)              0x4
#define CLOCK(PORTD)              0x8
#define CLOCK(PORTE)              0x10
#define CLOCK(PORTF)              0x20

//**************GPIO REGISTERS OFFSET ****************************************** 
#define GPIO_DATA             0x000003FC
#define GPIO_DIR              0x00000400
#define GPIO_DEN              0x0000051C
#define GPIO_PUR              0x00000510
#define GPIO_LOCK             0x00000520
#define GPIO_CR               0x00000524
#define GPIO_AFSEL            0x00000420 
#define GPIO_AMSEL            0x00000528 

//**************SET & CLEAR COMMAND****************************************        
#define SET(REG, PIN)  (REG |= PIN)
#define CLR(REG, PIN)  (REG &= (~PIN))

//**************SET & CLEAR BIT COMMAND****************************************        
#define SET_PIN(REG,PIN)  (REG|=(ON<<PIN))
#define CLR_PIN(REG,PIN)  (REG&=(~(ON<<PIN)))
#define READ_PIN(REG,PIN)  ( 1 & ((REG)>>PIN))
 
#define DEREF(X)            (*((volatile uint32_t *)(X)))

void Switches_ON();
void PIN_BW_ON(uint32_t volatile PortBaseAdd , uint8_t Pin );
void PIN_BW_OFF(uint32_t volatile PortBaseAdd , uint8_t Pin );
void Pin_Dir_OUT(uint32_t volatile PortBaseAdd , uint8_t Pin);
void Pin_Dir_IN(uint32_t volatile PortBaseAdd , uint8_t Pin);
void GPIO_PORT_INIT(uint32_t PORTBaseAdd, uint8_t PINDIR, uint8_t PINDEN );
void GPIO_PIN_DEN(uint32_t PORTBaseAdd , uint8_t DENVAL);
void GPIO_PIN_AM(uint32_t PORTBaseAdd , uint8_t AMVAL);
void GPIO_PINS_WRITE(uint32_t volatile PortBaseAdd, uint8_t PINSVAL );
void GPIO_PINS_CLEAR(uint32_t volatile PortBaseAdd, uint8_t PINSVAL );
uint32_t GPIO_PINS_READ( uint32_t PortBaseAdd);
bool PIN_READ( uint32_t PortBaseAdd, uint8_t PIN);
void GPIO_PUR_WRITE(uint32_t PORTBaseAdd , uint8_t PURVAL);
void GPIO_PUR_CLEAR(uint32_t PORTBaseAdd , uint8_t PURVAL);
void PIN_PUR_ON(uint32_t PORTBaseAdd , uint8_t PIN);
void PIN_PUR_OFF(uint32_t PORTBaseAdd , uint8_t PIN);
void GPIO_PINS_DIR_WRITE(uint32_t PORTBaseAdd , uint8_t DIRVAL);
void GPIO_PINS_DIR_CLEAR(uint32_t PORTBaseAdd , uint8_t DIRVAL);
uint32_t GPIO_PINS_DIR_READ( uint32_t PortBaseAdd);

#endif