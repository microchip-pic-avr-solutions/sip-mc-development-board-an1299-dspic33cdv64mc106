/*******************************************************************************
  ADC Configuration Routine source File

  File Name:
    adc.c

  Summary:
    This file includes subroutine for initializing ADC Cores of Controller

  Description:
    Definitions in the file are for dsPIC33CK256MP508 on Motor Control 
    Development board from Microchip

*******************************************************************************/
/*******************************************************************************
* Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.
*
* SOFTWARE LICENSE AGREEMENT:
* 
* Microchip Technology Incorporated ("Microchip") retains all ownership and
* intellectual property rights in the code accompanying this message and in all
* derivatives hereto.  You may use this code, and any derivatives created by
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE,
* WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF
* STATUTORY DUTY),STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE,
* FOR ANY INDIRECT, SPECIAL,PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL
* LOSS, DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE,
* HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR
* THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW,
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS CODE,
* SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify, test,
* certify, or support the code.
*
*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "adc.h"
#include "userparms.h"

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
void InitializeADCs(void);
// *****************************************************************************
/* Function:
    void InitializeADCs (void)

  Summary:
    Routine to configure ADC Module

  Description:
    Function to configure ADC Cores
  
  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */
void InitializeADCs (void)
{
    ADCON1L = 0;
    /* ADC Module is turned OFF before configuring it */
    /* ADC Enable bit 1 = ADC module is enabled 0 = ADC module is off         */
    ADCON1Lbits.ADON = 0;
    /* ADC Stop in Idle Mode bit 0 = Continues module operation in Idle mode  */
    ADCON1Lbits.ADSIDL = 0;

    ADCON1H = 0;
    /* Shared ADC Core Resolution Selection bits
       0b11 = 12-bit ; 0b10 = 10-bit ; 0b01 = 8-bit ; 0b00 = 6-bit resolution */
    ADCON1Hbits.SHRRES = 3;
    /* Fractional Data Output Format bit 1 = Fractional ; 0 = Integer         */
    ADCON1Hbits.FORM = 1;

    ADCON2L = 0;
    /* Shared ADC Core Input Clock Divider bits
       These bits determine the number of TCORESRC (Source Clock Periods) 
       for one shared TADCORE (Core Clock Period).
       1111111 = 254 Source Clock Periods
        ???
       0000010 = 4 Source Clock Periods
       0000001 = 2 Source Clock Periods
       0000000 = 2 Source Clock Periods
     */
    ADCON2Lbits.SHRADCS = 0;
    /* EIEN: Early Interrupts Enable bit
       1 = The early interrupt feature is enabled for the input channel 
           interrupts ,when the EISTATx flag is set
       0 = The individual interrupts are generated when conversion is done ,
           when the ANxRDY flag is set*/
    ADCON2Lbits.EIEN = 0;

    ADCON2H = 0;
    /* Shared ADC Core Sample Time Selection bits 
       These bits specify the number of shared ADC Core Clock Periods (TADCORE) 
       for the shared ADC core sample time.
       Ranges from 2 to 1025 TADCORE
       if SHRSAMC = 15 ,then Sampling time is 17 TADCORE */
    ADCON2Hbits.SHRSAMC = 15;

    ADCON3L  = 0;
    /* ADC Reference Voltage Selection bits 
       0b000 = VREFH is AVDD and VREFL is  AVSS */
    ADCON3Lbits.REFSEL = 0;

    ADCON3H = 0;

    /* Shared ADC Core Enable bit 1 = Shared ADC core is enabled 
       0 = Shared ADC core is disabled  */
    /* Shared ADC Core is Disabled prior to configuration */
    ADCON3Hbits.SHREN = 0;
    /* ADC Module Clock Source Selection bits 
       0b11 = FVCO/4;0b10 = AFVCODIV ;0b01 = FOSC ; 0b00 = FP(Peripheral Clock) */
    ADCON3Hbits.CLKSEL = 0;
    /* ADC Module Clock Source Divider bits (1 to 64)
       The divider forms a TCORESRC clock used by all ADC cores (shared and 
       dedicated) from the TSRC ADC module clock selected by the CLKSEL<2:0> .
       000000 = 1 Source Clock Periods */
    ADCON3Hbits.CLKDIV = 0;
    


    /* Configuring ADC INPUT MODE CONTROL REGISTER bits 
       Output Data Sign for Corresponding Analog Inputs bits
       1 = Channel output data is signed
       0 = Channel output data is unsigned    */
    /*ADMOD0L configures Output Data Sign for Analog inputs  AN0 to AN7 */
    ADMOD0L = 0x0000;
    ADMOD0Lbits.SIGN0 = 1;
    ADMOD0Lbits.SIGN1 = 1;
    ADMOD0Lbits.SIGN4 = 1;
   
    /*ADMOD0H configures Output Data Sign for Analog inputs  AN8 to AN15 */
    ADMOD0H = 0;   
    ADMOD0Hbits.SIGN15 = 0;
    ADMOD0Hbits.SIGN12 = 0;
    ADMOD1L = 0;
    

    
    /* Ensuring all interrupts are disabled and Status Flags are cleared */
    ADIEL = 0;
    ADIEH = 0;
    ADSTATL = 0;
    ADSTATH = 0;
    ADEIEL  = 0;
    ADEIEH  = 0;
    ADEISTATL = 0;
    ADEISTATH = 0;
 
    ADCON5H = 0;
    /* Shared ADC Core Ready Common Interrupt Enable bit
       0 = Common interrupt is disabled for an ADC core ready event*/
    ADCON5Hbits.SHRCIE = 0;
    /* ADC Dedicated Core x Power-up Delay bits
       These bits determine the power-up delay in the number of the 
       Core Source Clock Periods (TCORESRC) for all ADC cores.
       0b1011 = 2048 Source Clock Periods */
    ADCON5Hbits.WARMTIME  = 0b1111 ;                                         
    
    /* ADC Enable bit 1 = ADC module is enabled 0 = ADC module is off         */
    /* Turn on ADC Module */
    ADCON1Lbits.ADON      = 1 ;  
    
    ADCON5L = 0;

    /* Turn on analog power for shared core */
    ADCON5Lbits.SHRPWR    = 1 ;
    while(ADCON5Lbits.SHRRDY == 0);
    /* Shared ADC Core Enable bit 1 = Shared ADC core is enabled 
       0 = Shared ADC core is disabled  */
    /* Shared ADC Core is Enabled  */
    ADCON3Hbits.SHREN     = 1 ;
    
    /* Setup ADC Interrupts for reading and processing converted results */
    /* Common Interrupt Enable bits
       1 = Common and individual interrupts are enabled for analog channel
       0 = Common and individual interrupts are disabled for analog channel*/
   
 
    
#ifdef SINGLE_SHUNT    
     _IE1        = 1 ;
    /* Clear ADC interrupt flag */
    _ADCAN1IF    = 0 ;  
    /* Set ADC interrupt priority IPL 7  */ 
    _ADCAN1IP   = 7 ;  
    /* Disable the AN4 interrupt  */
    _ADCAN1IE    = 0 ;
     
#else         
    _IE15        = 1 ;
    /* Clear ADC interrupt flag */
    _ADCAN15IF    = 0 ;  
    /* Set ADC interrupt priority IPL 7  */ 
    _ADCAN15IP   = 7 ;  
    /* Disable the AN1 interrupt  */
    _ADCAN15IE    = 0 ; 
#endif
    
    /* Trigger Source Selection for Corresponding Analog Inputs bits 
     *  00101 = PMW1 Trigger 2
        00100 = PMW1 Trigger 1
        00001 = Common software trigger
        00000 = No trigger is enabled  */
    

#ifdef SINGLE_SHUNT
    /* Trigger Source for Analog Input #0  = 0b0101 */
    ADTRIG0Lbits.TRGSRC1 = 0x5;
#else
      /* Trigger Source for Analog Input #1  = 0b0100 */
    ADTRIG0Lbits.TRGSRC0 = 0x4;
    /* Trigger Source for Analog Input #4  = 0b0100 */
    ADTRIG1Lbits.TRGSRC4 = 0x4;  
#endif
    ADTRIG3Lbits.TRGSRC12 = 0x4;
    /* Trigger Source for Analog Input #15  = 0b0100 */
    ADTRIG3Hbits.TRGSRC15 = 0x4;
   
}
