/* UNIVERSIDAD DEL VALLE DE GUATEMALA
 * DEPARTAMENTO DE INGENIERIA ELCTRONICA & MECATRONICA
 * CURSO DE DIGITAL II
 * LABORATORIO No.1
 * 
 * File:   main1.c
 * Author: Selvin E. Peralta Cifuentes 
 *
 * Created on 15 de julio de 2021, 21:40 AM
 * Ultima Actualizacion  
 */
//-----------------------------------------------------------------------------------------------------------------------------------
// CONFIG1
//-----------------------------------------------------------------------------------------------------------------------------------
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)
//-----------------------------------------------------------------------------------------------------------------------------------
// CONFIG2
//-----------------------------------------------------------------------------------------------------------------------------------
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include <stdint.h>             // TIPOS DE DATOS ESTANDAR Y OTROS.
#define _XTAL_FREQ 4000000
#include <pic16f887.h>
#include "ADC.h"
//__________
// DECLARACION DE VARIABLES
uint8_t V;
uint8_t V1;
uint8_t Unidad;
uint8_t Decena;
uint8_t Centena;
uint8_t Nibble;
uint8_t Nibble2;
uint8_t Suitw;
//__________
// PROTOTIPOS DE FUNCIONES
void Setup(void);       // Llamamos las configuraciones de los pines 
int  Tabla(int V1);    // Llamamos la funcion tabla
//__________
// FUNCION DE INTERRUPCIONES
void __interrupt() isr(void){
//----------------------------- INTERRUPCION TMR0 ------------------------------    
    if(T0IF == 1) {           //Verificamos si la bandera del TMR0 es 1
        TMR0  = 236;          //Colocamos el valor a TMR0 para que funcione la interrupcion cada 5ms
        T0IF  = 0;            //Reiniciamos la bandera del TMR0
        PORTE = 0x00;         //Encendemos el Puerto E
        if(Suitw == 0){       //Verificamos la variable para poder realizar el suicheo 
            V1    = Nibble;   //Movemos el valor de la variable unidad a V
            PORTD = Tabla(V1);//Movemos el valor de la tabla a PORTD
            RE0   = 1;        //Volvemos 0 el pin del puerto E
            Suitw = 1;        //Cambiamos el valor de Suitw para poder controlar el suicheo 
            return;
        }    
        if(Suitw == 1){
            V1    = Nibble2;
            PORTD = Tabla(V1);
            RE1   = 1;
            Suitw = 0;
            return;
        }    
    }
//----------------------------- INTERRUPCION RBIF ------------------------------     
    if(RBIF == 1){           //Verificamos si la bandera del IoCH es 1 
        if(RB0 == 0){        //Verificamos si el pin esta precionado 
            PORTC++;         //Aumentamos el Puerto C cuando se preciona el boton 
        }
        if(RB1 == 0){        //Verificamos si el pin esta precionado 
            PORTC--;         //Decrementamos el Puerto C cuando se preciona el boton
        }
        RBIF = 0;            //Limpiamos la bandera del IoCH
    }

//----------------------------- INTERRUPCION Y LIBRERIA DEL  ADC ----------------------------
    Recib_ADC();             //LLamamos la libreria para recibir datos del ADC
}
//__________
// FUNCION PRINCIPAL (MAIN)
void main(void){
     Setup();
    __delay_us(50);
    ADCON0bits.GO = 1;       
     while(1){
         Nibble  = V & 0x0F;      //Tomamos los 4 bits menos signficativos del ADRESH
         Nibble2 = (V>>4) & 0x0F; //Tomamos los 4 bits mas signficativos del ADRESH
         
         Stard_ADC();             //Llamamos la libreria para comensar al conversion 
    
        if(V > PORTC){            //Verificamos si el valor del Adresh es mayor que el contador 
            RA2 = 1;              //Encendemos la alarma
        }
        if(V < PORTC || V == PORTC ){//Verificamos si el valor del Adresh es menor o igual al valor del contador
            RA2 = 0;              //Apagamos la alarma 
        }
        
     }
     
}
//__________
//TABLA DE VALORES DE LOS DISPLAYS
int Tabla(int V1){
    int w;
    switch (V1){
        case 0:    //#0/
        w = 0b00111111;
        break;
        case 1:   //#1/
        w = 0b00000110;
        break;
        case 2:   //#2/
        w = 0b01011011;
        break;
        case 3:  //#3/
        w = 0b01001111;
        break;
        case 4: //#4/
        w = 0b01100110;
        break;
        case 5: //#5/
        w = 0b01101101;
        break;
        case 6: //#6/
        w = 0b01111101;
        break;
        case 7: //#7/
        w = 0b00000111;
        break;
        case 8: //#8/
        w = 0b01111111;
        break;
        case 9: //#9/
        w = 0b01101111;
        break;
        case 10: //A/
        w = 0b01110111;
        break;
        case 11: //B/
        w = 0b01111100;
        break;
        case 12: //C/
        w = 0b00111001;
        break;
        case 13: //D/
        w = 0b01011110;
        break;
        case 14: //E/
        w = 0b01111001;
        break;
        case 15: //F/
        w = 0b01110001;
        break;
        default:
        w = 0;    
    }
    return w;
}
//__________
// FUNCION DE CONFIGURACION

void Setup(void){
//------------------- CONFIGURACION DE ENTRADAS Y SALIDAS ----------------------
    ANSEL  = 0b00000011;    // Configuracion a pines Digitale 
    ANSELH = 0x00;
 //Configuramos que pines queremos como salidas o entradas 
    TRISB = 0b00000011; 
    TRISC = 0x00;
    TRISA = 0b00000001;
    TRISD = 0x00;
    TRISE = 0x00;
 //Limpiamos todos los puertes 
    PORTB = 0x00;
    PORTC = 0x00;
    PORTA = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
//---------------------- CONFIGURACION DE RELOJ A 4MHZ -------------------------
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS   = 1;
//---------------------- CONFIGURACION DE ADC ------- -------------------------
    ADCON1bits.ADFM  = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    ADCON0bits.ADCS  = 1;   //Configuracion del reloj del modulo 
    ADCON0bits.CHS   = 0;   //Seleccionamos el canal donde querramos empezar
    __delay_us(100);
    ADCON0bits.ADON  = 1;   //Encendemos el modulo ADC
    __delay_us(50);
//------------------------- CONFIGURACION DE BOTONES ---------------------------
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b00000011;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
//-------------------------- CONFIGURACION DEL TMRO ----------------------------
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA  = 0;
    OPTION_REGbits.PS2  = 1;
    OPTION_REGbits.PS1  = 1;
    OPTION_REGbits.PS0  = 1;
  
//---------------------- CONFIGURACION DE INTERRUPCIONES -----------------------
    INTCONbits.GIE  = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE   = 1;
    INTCONbits.T0IE = 1;
    PIR1bits.ADIF   = 1;
}
