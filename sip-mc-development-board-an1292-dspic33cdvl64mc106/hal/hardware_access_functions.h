/**
  @File Name:
    hardware_access_functions.h

  @Summary:
    This module provides hardware access function support.

  @Description:
    This module provides hardware access function support.
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
#ifndef __HAF_H
#define __HAF_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "uart2.h"
#include "bsp.h"
#include "delay.h"
#include "hardware_access_functions_types.h"
#include "hardware_access_functions_params.h"

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif


/**
  Section: Hardware Access Functions
 */     

/**
 * Configures the board and initializes any state depenendent data that is used
 * in the HAL. This function needs to be called once every 1ms.
 * Summary: Configures the board and initializes any state depenendent data that is used in the HAL.
 * @example
 * <code>
 * while (HAL_Board_Configure()==BOARD_NOT_READY);
 * <code>
 */
HAL_BOARD_STATUS HAL_Board_Configure(void);

/**
 * Runs the board service routines. This function needs to be called once every 1ms.
 * Summary: Runs the board service routines.
 * @example
 * <code>
 * if (HAL_Board_Service()!=BOARD_READY)
 * {
 *     // do something
 * }
 * <code>
 */
HAL_BOARD_STATUS HAL_Board_Service(void);

/**
 * This function can  be called to clear the fault.
 * Summary: Clears Gate Driver Fault Status
 * @example
 * <code>
 * HAL_Board_Board_FaultClear();
 * <code>
 */
void HAL_Board_FaultClear(void);
/**
 * This function can  be called to clear the fault.
 * Summary: Clears Gate Driver Fault Status
 * @example
 * <code>
 * HAL_Board_Board_FaultClear();
 * <code>
 */
void HAL_Board_FaultSet(void);
/**
 * This function can  be called to request auto baud sequence.
 * Summary: Requests the auto baud sequence 
 * @example
 * <code>
 * HAL_Board_AutoBaudRequest();
 * <code>
 */
void HAL_Board_AutoBaudRequest(void);

extern  GATE_DRIVER_OBJ inverterGateDriver[BSP_GATE_DRIVER_INSTANCE_COUNT];
#ifdef __cplusplus
}
#endif

#endif /* __HAF_H */


/**
 End of File
 */