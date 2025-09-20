#include <stdint.h>
#include <stdbool.h>
#include "stm32c0xx.h"


#define STP_Port GPIOB
#define STP_Pin 3        
#define DIR_Port GPIOB
#define DIR_Pin 10        
#define BUT1_Port GPIOB
#define BUT1_Pin 4       
#define BUT2_Port GPIOB
#define BUT2_Pin 5       
#define POT_Port GPIOA
#define POT_Pin 0        


int targetSpeed = 0;
int currentSpeed = 0;
int acceleration = 2;
bool isMoving = false;
int moveDirection = 0; 


void delay_ms(uint32_t ms){
    for(uint32_t i=0; i<ms*4800; i++) __NOP(); 
}


void GPIO_Init(void){
    
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN; 
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN; 
    
    STP_Port->MODER &= ~(3U<<(STP_Pin*2));  
    STP_Port->MODER |= (1U<<(STP_Pin*2));   
    STP_Port->OTYPER &= ~(1U<<STP_Pin);     
    STP_Port->OSPEEDR |= (1U<<(STP_Pin*2)); 
    
    
    DIR_Port->MODER &= ~(3U<<(DIR_Pin*2));  
    DIR_Port->MODER |= (1U<<(DIR_Pin*2));   
    DIR_Port->OTYPER &= ~(1U<<DIR_Pin);     
    DIR_Port->OSPEEDR |= (1U<<(DIR_Pin*2));
    
    BUT1_Port->MODER &= ~(3U<<(BUT1_Pin*2)); 
    BUT1_Port->PUPDR &= ~(3U<<(BUT1_Pin*2)); 
    BUT1_Port->PUPDR |= (1U<<(BUT1_Pin*2));  
    
    BUT2_Port->MODER &= ~(3U<<(BUT2_Pin*2)); 
    BUT2_Port->PUPDR &= ~(3U<<(BUT2_Pin*2)); 
    BUT2_Port->PUPDR |= (1U<<(BUT2_Pin*2)); 
    
    
    POT_Port->MODER |= (3U<<(POT_Pin*2));    
}

void ADC_Init(void){
    RCC->APBENR2 |= RCC_APBENR2_ADCEN;
    
    
    ADC1->CR = 0;                        
    ADC1->CHSELR = (1U<<POT_Pin);        
    ADC1->SMPR = ADC_SMPR_SMP1_2;        
    
   
    ADC1->CR |= ADC_CR_ADEN;
    while(!(ADC1->ISR & ADC_ISR_ADRDY));
}


uint16_t ADC_Read(void){
    ADC1->CR |= ADC_CR_ADSTART;          
    while(!(ADC1->ISR & ADC_ISR_EOC));   
    return ADC1->DR;                    
}


void makeStep(int speedDelay){
    STP_Port->BSRR = (1U<<STP_Pin);         
    delay_ms(speedDelay);
    STP_Port->BSRR = (1U<<(STP_Pin+16));    
    delay_ms(speedDelay);
}


void checkButtons(void){
    static bool lastBut1 = true;
    static bool lastBut2 = true;
    
    bool curBut1 = ((BUT1_Port->IDR >> BUT1_Pin) & 1);
    bool curBut2 = ((BUT2_Port->IDR >> BUT2_Pin) & 1);
    
    if(lastBut1 && !curBut1){
        if(!isMoving || moveDirection != 0){
            moveDirection = 0;
            DIR_Port->BSRR = (1U<<(DIR_Pin+16)); 
            isMoving = true;
        } else {
            isMoving = false;
        }
        delay_ms(50); 
    }
    
    if(lastBut2 && !curBut2){
        if(!isMoving || moveDirection != 1){
            moveDirection = 1;
            DIR_Port->BSRR = (1U<<DIR_Pin);      
            isMoving = true;
        } else {
            isMoving = false;
        }
        delay_ms(50); 
    }
    
    lastBut1 = curBut1;
    lastBut2 = curBut2;
}

void smoothAcceleration(void){
    if(currentSpeed < targetSpeed){
        currentSpeed += acceleration;
        if(currentSpeed > targetSpeed) currentSpeed = targetSpeed;
    } else if(currentSpeed > targetSpeed){
        currentSpeed -= acceleration;
        if(currentSpeed < targetSpeed) currentSpeed = targetSpeed;
    }
}

void smoothDeceleration(void){
    if(currentSpeed > 0){
        currentSpeed -= acceleration;
        if(currentSpeed < 0) currentSpeed = 0;
    }
}

int main(void){
    GPIO_Init();
    ADC_Init();
    while(1){
        uint16_t potValue = ADC_Read();
        targetSpeed = 1000 - (potValue * 999 / 4095);
      
        checkButtons();
        
        if(isMoving) {
            smoothAcceleration();
        } else {
            smoothDeceleration();
        }
        
        if(currentSpeed > 0) {
            makeStep(currentSpeed);
        }
        
        delay_ms(1); 
    }
}

