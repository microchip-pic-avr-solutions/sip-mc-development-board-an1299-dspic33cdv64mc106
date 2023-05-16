/**
  @File Name:
    hardware_access_functions_params.h

  @Summary:
    This file includes parameter constants for the hardware access function 
    module.

  @Description:
    This file includes parameter constants for the hardware access function 
    module.
 */
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

#ifndef __HAF_PARAMS_H
#define __HAF_PARAMS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


/******************************************************************************/
/* Section: Private constants used by MCP802x driver                          */
/******************************************************************************/

/** 
 * Specify the Gate Driver Instances in a Board.Mention the index of the 
 * all the gate driver instances being used.Preferably in order for the ease 
 * of interpretation.
 * 
 * For example Microchip Low Voltage Motor Control Bundle has two gate drivers 
 * namely Gate Driver A and Gate Driver B.Here are two use case examples that 
 * initializes macros BSP_GATE_DRIVER_INSTANCE_COUNT and 
 * BSP_GATE_DRIVER_x_INDEX(x = A,B,.. etc.),
 * 
 * 
 * Case 1: If both gate drivers are used in the application initialize macros as
 * #define BSP_GATE_DRIVER_INSTANCE_COUNT      2
 * #define BSP_GATE_DRIVER_B_INDEX             1
 * #define BSP_GATE_DRIVER_A_INDEX             0
 * 
 * Case 2:If only Gate Driver A is used in the application initialize macros as
 * #define BSP_GATE_DRIVER_INSTANCE_COUNT      1
 * #define BSP_GATE_DRIVER_A_INDEX             0
 * 
 * Case 3:If only Gate Driver B is used in the application initialize macros as
 * #define BSP_GATE_DRIVER_INSTANCE_COUNT      1
 * #define BSP_GATE_DRIVER_B_INDEX             0
 * 
 * Similarly the macros can be extended to create multiple gate 
 * gate driver instances in a board 
 */
#define BSP_GATE_DRIVER_INSTANCE_COUNT      1
#define BSP_GATE_DRIVER_A_INDEX             0
/*Scaler for Baud Rate 9600 bps*/
#define GATE_DRIVER_COMM_BAUDRATE_SCALER   650
/*Scaler for Baud Rate 10170 bps(High Limit)*/
#define GATE_DRIVER_COMM_BAUDRATE_SCALER_MIN   613
/*Scaler for Baud Rate 9030 bps(Low Limit)*/
#define GATE_DRIVER_COMM_BAUDRATE_SCALER_MAX   691
/*Scaler for Baud Rate 7880 bps*/
#define GATE_DRIVER_COMM_ABAUD_BREAK_WINDOW_SCALER  792

#define GATE_DRIVER_FAULT_CLEARING_PULSE_WIDTH 5

/** Specify the number of times gate driver handler function has to re-try 
 * to configure Gate Driver in case of any failure  */  
#define GATE_DRIVER_TRYCOUNT_MAX             5
/** Specify interval or rate at which the gate driver handler must read 
 * the gate driver status in number of 1 mSec
 * Example: if gate driver status has to be read every 
 * 1 second then specify GATEDRIVER_STATUS_READ_INTERVAL as  2000 
 * i.e. GATEDRIVER_STATUS_READ_INTERVAL =  1000 mSec / 0.25mSec = 4000 */
#define GATEDRIVER_STATUS_READ_INTERVAL 1000
/** Marks the there are no data to be read from data buffer or indicates
 invalid data */
#define GATE_DRIVER_INVALID_DATA        0x200

/** The The MCP8024 may send an 0x86 0x01, 0x86 0x02 or
0x86 0x03 message when accelerating a high-current
motor. The messages are overcurrent warnings for the
5V and 12V regulators. The warnings have no effect on
the actual regulator operation, they are only indicators
of the status of the regulator.   */
#define GATE_DRIVER_STATUS1_ERRATA_MASK    0x03


/** The time duration to wait when SET_CONFIG_x command is used for configuring 
    Gate driver in mSec .
    SET_CONFIG_x command send by the host is two bytes long;
    Gate Driver acknowledges SET_CONFIG_x command by sending two bytes 
    Hence four bytes of data is exchanged at 9600baud between Host and Gate 
    Driver.This takes ~4ms. Also a delay of ~0.5ms is observed between 
    the Command transmission from Host and acknowledgment from Driver.  
    Hence total time required is ~4.5ms. 
    Considering Gate Driver routines are called from Application at every 0.25ms,
    Timeout is initialized as 20 (corresponds to 5ms)
*/    
#define GATE_DRIVER_SETCONFIG_TIMEOUT       2
/** The time duration to wait when GET_STATUS_x command is used for configuring 
    Gate driver in mSec .
    GET_STATUS_x command send by the host is one byte long;
    Gate Driver acknowledges GET_STATUS_x command by sending two bytes 
    Hence three bytes of data is exchanged at 9600baud between Host and Gate 
    Driver.This takes ~3ms. Also a delay of ~0.5ms is observed between 
    the Command transmission from Host and acknowledgment from Driver.  
    Hence total time required is ~3.5ms. 
    Considering Gate Driver routines are called from Application at every 0.25ms,
    Timeout is initialized as 16 (corresponds to 4ms)
*/
#define GATE_DRIVER_SETCONFIG_SETRXPIN_TIMEOUT  1
      
/** The time duration to wait when GET_STATUS_x command is used for configuring 
    Gate driver in mSec .
    GET_STATUS_x command send by the host is one byte long;
    Gate Driver acknowledges GET_STATUS_x command by sending two bytes 
    Hence three bytes of data is exchanged at 9600baud between Host and Gate 
    Driver.This takes ~3ms. Also a delay of ~0.5ms is observed between 
    the Command transmission from Host and acknowledgment from Driver.  
    Hence total time required is ~3.5ms. 
    Considering Gate Driver routines are called from Application at every 0.25ms,
    Timeout is initialized as 16 (corresponds to 4ms)
*/
#define GATE_DRIVER_READSTATUS_TIMEOUT       2

/** The time duration to wait when GET_STATUS_x command is used for configuring 
    Gate driver in mSec .
    GET_STATUS_x command send by the host is one byte long;
    Gate Driver acknowledges GET_STATUS_x command by sending two bytes 
    Hence three bytes of data is exchanged at 9600baud between Host and Gate 
    Driver.This takes ~3ms. Also a delay of ~0.5ms is observed between 
    the Command transmission from Host and acknowledgment from Driver.  
    Hence total time required is ~3.5ms. 
    Considering Gate Driver routines are called from Application at every 0.25ms,
    Timeout is initialized as 16 (corresponds to 4ms)
*/
#define GATE_DRIVER_READSTATUS_SETRXPIN_TIMEOUT 0

/** The time duration to wait after initiating Break Character Sequence 
* in mSec */    
#define GATE_DRIVER_ABAUD_BREAK_SEQUENCE_TIMEOUT       1
/** The time duration to wait after transmitting the Break Character for 
* Auto Baud in mSec */    
#define GATE_DRIVER_ABAUD_CHARACTER_RECIEVE_TIMEOUT    4 
/** Dummy Data to be sent to initiate Break Character Transmit Sequence */
#define GATE_DRIVER_ABAUD_BREAK_SEQUENCE_DUMMY_DATA    0x00

#endif /* __HAF_PARAMS_H */
