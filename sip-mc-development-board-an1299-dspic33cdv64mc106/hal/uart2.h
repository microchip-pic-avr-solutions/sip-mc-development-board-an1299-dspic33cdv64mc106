// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * UART2.h
 *
 * This header file lists interface functions - to configure and enable UART2 
 * module and its features
 * 
 * Definitions in this file are for dsPIC33CK64MC105 MC PIM plugged onto 
 * Motor Control Development board from Microchip.
 * 
 * Component: HAL - UART2
 * 
 */
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Disclaimer ">
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
// </editor-fold>

#ifndef __UART2_H
#define __UART2_H

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">
    
#include <xc.h>

#include <stdint.h>
#include <stdbool.h>

// </editor-fold> 

#ifdef __cplusplus  // Provide C++ Compatability
    extern "C" {
#endif
                
// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">
     
        
/**
 * Enables and initializes UART2 with a default configuration:
 * - Continues module operation in Idle mode
 * - IrDA encoder and decoder are disabled
 * - UxRTS pin is in Flow Control mode
 * - UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLKx pins are
 *   controlled by PORT latches
 * - No wake-up is enabled
 * - Loop back mode is disabled
 * - Baud rate measurement is disabled
 * - UxRX Idle state is '1'
 * - BRG generates 16 clocks per bit period (16x baud clock, Standard mode)
 * - 8-bit data, no parity
 * - One Stop bit.
 * This function does not configure the baud rate for UART2, however, it does
 * clear interrupt flags and disable interrupts for UART2 before initializing.
 * Summary: Enables and initializes UART2 with a default configuration.
 * @example
 * <code>
 * UART2_Initialize(void);
 * </code>
 */        
extern void UART2_Initialize(void);

/**
  Section: Driver Interface
 */

/**
 * Clears UART2 transmit interrupt request flag.
 * Summary: Clears UART2 transmit interrupt request flag.
 * @example
 * <code>
 * UART2_InterruptTransmitFlagClear();
 * </code>
 */
inline static void UART2_InterruptTransmitFlagClear(void) {_U2TXIF = 0; }

/**
 * Clears UART2 receive interrupt request flag.
 * Summary: Clears UART2 receive interrupt request flag.
 * @example
 * <code>
 * UART2_InterruptReceiveFlagClear();
 * </code>
 */
inline static void UART2_InterruptReceiveFlagClear(void) {_U2RXIF = 0; }

/**
 * Enables UART2 transmit interrupt.
 * Summary: Enables UART2 transmit interrupt.
 * @example
 * <code>
 * UART2_InterruptTransmitEnable();
 * </code>
 */
inline static void UART2_InterruptTransmitEnable(void) {_U2TXIE = 1; }

/**
 * Disables UART2 transmit interrupt.
 * Summary: Disables UART2 transmit interrupt.
 * @example
 * <code>
 * UART2_InterruptTransmitDisable();
 * </code>
 */
inline static void UART2_InterruptTransmitDisable(void) {_U2TXIE = 0; }

/**
 * Enables UART2 receive interrupt.
 * Summary: Enables UART2 receive interrupt.
 * @example
 * <code>
 * UART2_InterruptReceiveEnable();
 * </code>
 */
inline static void UART2_InterruptReceiveEnable(void) {_U2RXIE = 1; }

/**
 * Disables UART2 receive interrupt.
 * Summary: Disables UART2 receive interrupt.
 * @example
 * <code>
 * UART2_InterruptReceiveDisable();
 * </code>
 */
inline static void UART2_InterruptReceiveDisable(void) {_U2RXIE = 0; }

/**
 * Configures UART2 module to operate in standard baud rate mode 
 *(i.e. 16x baud clock). 
 * Baud Rate = FREQ_UART_CLK (BCLKSEL)/ (16*(BRG +1))
 * @example
 * <code>
 * UART2_SpeedModeStandard();
 * </code>
 */
inline static void UART2_SpeedModeStandard(void) {U2MODEbits.BRGH = 0; }

/**
 * Configures UART2 module to operate in High-speed baud rate mode 
 * (i.e. 4x baud clock).
 * Baud Rate = FREQ_UART_CLK (BCLKSEL) / (4*(BRG +1))
 * @example
 * <code>
 * UART2_SpeedModeHighSpeed();
 * </code>
 */
inline static void UART2_SpeedModeHighSpeed(void) {U2MODEbits.BRGH = 1; }

/**
 * Configures the baud rate divider for UART2 module.
 * Baud Rate = FREQ_UART_CLK (BCLKSEL)/ (16*(BRG +1)),if BRGH = 0, BCLKMOD = 0
 * Baud Rate = FREQ_UART_CLK (BCLKSEL)/ (4*(BRG +1)),if BRGH = 0, BCLKMOD = 0
 * @example
 * <code>
 * UART2_BaudRateDividerSet(500);
 * </code>
 */
inline static void UART2_BaudRateDividerSet(uint16_t baudRateDivider)
{
    U2BRG = baudRateDivider;
}

/**
 * Disables UART2 module.
 * Summary: Disables UART2 module.
 * @example
 * <code>
 * UART2_ModuleDisable();
 * </code>
 */
inline static void UART2_ModuleDisable(void) 
{
    U2MODEbits.UARTEN = 0;
}

/**
 * Enables UART2 module.
 * Summary: Enables UART2 module.
 * @example
 * <code>
 * UART2_ModuleEnable();
 * </code>
 */
inline static void UART2_ModuleEnable(void) 
{
    U2MODEbits.UARTEN = 1;
}

/**
 * Enables UART2 module transmit mode.
 * @example
 * <code>
 * UART2_TransmitModeEnable();
 * </code>
 */
inline static void UART2_TransmitModeEnable(void) {U2MODEbits.UTXEN = 1; }

/**
 * Disables UART2 module transmit mode.
 * @example
 * <code>
 * UART2_TransmitModeDisable();
 * </code>
 */
inline static void UART2_TransmitModeDisable(void) {U2MODEbits.UTXEN= 0; }

/**
 * Gets the status of UART2 Receive Buffer Data Available flag
 * @return status of UART2 Receive Buffer Data Available flag; 
 * 1 = Receive buffer has data, 0 = Receive buffer is empty 
 * @example
 * <code>
 * status = UART2_IsReceiveBufferDataReady();
 * </code>
 */

inline static bool UART2_IsReceiveBufferDataReady(void)
{
    return(!U2STAHbits.URXBE) ;
}

/**
 * Gets the status of UART2 Receive Buffer Overrun Error Status flag
 * @return status of UART2 Receive Buffer Overrun Error Status flag; 
 * 1 = Receive buffer has overflowed, 0 = Receive buffer has not overflowed
 * @example
 * <code>
 * status = UART2_IsReceiveBufferOverFlowDetected();
 * </code>
 */
inline static bool UART2_IsReceiveBufferOverFlowDetected(void)
{
    return(U2STAbits.OERR) ;
}

/**
 * Gets the status of UART2 Framing Error Status flag
 * @return status of UART2 Framing Error Status flag; 
 * 1 = Framing error has been detected for the character at the top of 
 *     the receive FIFO
 * 0 = Framing error has not been detected
 * @example
 * <code>
 * status = UART2_IsFrameErrorDetected();
 * </code>
 */
inline static bool UART2_IsFrameErrorDetected(void)
{
    return(U2STAbits.FERR) ;
}
/**
 * Gets the status of UART2 Parity Error Status flag
 * @return status of UART2 Parity Error Status flag; 
 * 1 = Parity error has been detected for the character 
 *     at the top of the receive FIFO
 * 0 = Parity error has not been detected
 * @example
 * <code>
 * status = UART2_IsParityErrorDetected();
 * </code>
 */
inline static bool UART2_IsParityErrorDetected(void)
{
    return(U2STAbits.PERR) ;
}
/**
 * Gets the status of UART2 Receiver is Idle Status Flag
 * @return status of UART2 Receiver is Idle Status flag; 
 * 1 = Receiver is Idle
 * 0 = Receiver is Active
 * @example
 * <code>
 * status = UART2_IsReceiverIdle();
 * </code>
 */
inline static bool UART2_IsReceiverIdle(void)
{
    return(U2STAHbits.RIDLE) ;
}
/**
 * Gets the status of UART2 Transmit Shift Register Empty Status Flag
 * @return status of UART2 Transmit Shift Register Empty Status flag; 
 * 1 = Transmit Shift Register is empty and transmit buffer is empty
 * 0 = Transmit Shift Register is not empty, a transmission is in progress 
 *     or queued
 * @example
 * <code>
 * status = UART2_IsTransmissionComplete();
 * </code>
 */
inline static bool UART2_IsTransmissionComplete(void)
{
    return(U2STAbits.TRMT) ;
}

/**
 * Gets the status of UART2 transmit buffer full flag
 * @return status of UART2 transmit buffer full flag; 1 = Transmit buffer is
 *  full, 0 = Transmit buffer is not full
 * @example
 * <code>
 * status = UART2_StatusBufferFullTransmitGet();
 * </code>
 */
inline static bool UART2_StatusBufferFullTransmitGet(void)
{
    return U2STAHbits.UTXBF;
}

/**
 * Gets the transmitter and receiver status of UART2. The returned status may
 * contain a value with more than one of the bits specified in the
 * UART2_STATUS enumeration set. The caller should perform an "AND" with the
 * bit of interest and verify if the result is non-zero to verify the 
 * desired status bit.
 * @return UART2_STATUS value describing the current status of the transfer.
 * @example
 * <code>
 * status = UART2_StatusGet();
 * </code>
 */
inline static uint16_t UART2_StatusGet(void)
{
    return U2STA;
}

/**
 * Clears the Receive Buffer Overrun Error Status bit for UART2. If this bit
 * was previously set, then calling this function will reset the receiver buffer
 * and the U2RSR to an empty state.
 * @example
 * <code>
 * UART2_ReceiveBufferOverrunErrorFlagClear();
 * </code>
 */
inline static void UART2_ReceiveBufferOverrunErrorFlagClear(void)
{
    U2STAbits.OERR = 0;
}

/**
 * Sets the UART2 Transmit Break bit.UxTX pin is driven low regardless of 
 * the transmitter state.A Break character transmit consists of a Start bit, 
 * followed by twelve bits of ?0? and a Stop bit.
 * @example
 * <code>
 * UART2_TransmitBreakRequestFlagSet();
 * </code>
 */
inline static void UART2_TransmitBreakRequestFlagSet(void)
{
    U2MODEbits.UTXBRK = 1;
}
/**
 * Enables UART2 module Auto baud rate measurement on the next character, 
 * requires reception of a Sync field (0x55);cleared in hardware upon completion
 * @example
 * <code>
 * UART2_AutoBaudEnable();
 * </code>
 */
inline static void UART2_AutoBaudEnable(void) 
{
    U2MODEbits.ABAUD = 1; 
}
/**
 * Disables UART2 module Auto Baud Measurement or it is complete.
 * @example
 * <code>
 * UART2_AutoBaudDisable();
 * </code>
 */
inline static void UART2_AutoBaudDisable(void) 
{
    U2MODEbits.ABAUD = 0; 
}
/**
 * Gets the status of Auto Baud Measurement
 * @return Inverse of Auto Baud Enable Bit; 1 = Auto Baud Measurement Requested, 
 * 0 = Auto Baud Measurement is completed or disabled.
 * @example
 * <code>
 * status = UART2_UART2_IsAutoBaudComplete();
 * </code>
 */
inline static bool UART2_IsAutoBaudComplete(void) 
{
    return !U2MODEbits.ABAUD; 
}
/**
 * Writes a 16-bit data word to UART2 transmit register.
 * @param data data to be transmitted on UART2
 * @example
 * <code>
 * UART2_DataWrite(txdata);
 * </code>
 */
inline static void UART2_DataWrite(uint16_t data)
{
    U2TXREGbits.TXREG =(uint8_t)data;
}

/**
 * Reads a 16-bit data word from the UART2 receive register.
 * @return data read from the UART2 receive register
 * @example
 * <code>
 * rxdata = UART2_DataRead();
 * </code>
 */
inline static uint16_t UART2_DataRead(void)
{
    return U2RXREG;
}
/**
 * Clears the Receive Buffer Empty Status bit for UART2. If this bit
 * was previously reset, then calling this function will set the receiver buffer
 * and the U2RSR to an empty state.
 * @example
 * <code>
 * UART2_ReceiveBufferEmpty();
 * </code>
 */
inline static void UART2_ReceiveBufferEmpty(void)
{
    uint16_t data;
    if(U2STAHbits.URXBE == 0)
    {
        while(UART2_IsReceiveBufferDataReady()) 
        {
            data = UART2_DataRead() ;
        }
        U2STAHbits.URXBE = 1;
    }
}
/**
 * Reads a 16-bit data word from the UART2 Baud Rate Divisor Register.
 * @return data read from the  UART2 Baud Rate Divisor Register
 * @example
 * <code>
 * rxdata = UART2_BaudRateDivisorRead();
 * </code>
 */
inline static uint16_t UART2_BaudRateDivisorRead(void)
{
    return U2BRG;
}

/**
 * Reset the pointer for transmit buffer empty status
 * @return None
 * @example
 * <code>
 * UART2_TransmitBufferEmptyReset();
 * </code>
 */
inline static void UART2_TransmitBufferEmptyFlagClear(void)
{
    U2STAHbits.UTXBE = 1;
}
// </editor-fold> 

#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
#endif      // end of __UART2_H
    