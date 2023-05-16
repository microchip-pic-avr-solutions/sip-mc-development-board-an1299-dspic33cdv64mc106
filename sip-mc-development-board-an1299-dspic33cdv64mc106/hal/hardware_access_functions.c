/**
  @File Name:
    hardware_access_functions.c

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
#include <string.h>
#include <stdbool.h>
#include <xc.h>
#include "hardware_access_functions.h"
static bool HAL_GateDriver_Initialize(GATE_DRIVER_OBJ *);
static void HAL_GateDriver_Enable(GATE_DRIVER_OBJ *);
static void HAL_GateDriver_Disable(GATE_DRIVER_OBJ *);
static void HAL_GateDriver_Connect(GATE_DRIVER_HOST_INTERFACE *);
static GATE_DRIVER_CONFIG_STATE HAL_GateDriver_Install(GATE_DRIVER_OBJ *);
static GATE_DRIVER_OPERATION_STATE HAL_GateDriver_ReadStatus(GATE_DRIVER_OBJ *);
static GATE_DRIVER_OPERATION_STATE HAL_GateDriver_AutoBaudSequence(GATE_DRIVER_OBJ *);
static void HAL_GateDriver_Write(GATE_DRIVER_HOST_INTERFACE *,uint16_t );
static uint16_t HAL_GateDriver_Read(GATE_DRIVER_HOST_INTERFACE *);
static bool HAL_GateDriver_SchedulerRun(uint16_t *);
static bool HAL_GateDriver_Initialize_CommChannel(GATE_DRIVER_OBJ *);
static bool HAL_GateDriver_HookUARTFunctions(GATE_DRIVER_HOST_INTERFACE *,uint16_t);
static GATE_DRIVER_HANDLER_STATE HAL_GateDriver_Configure(GATE_DRIVER_OBJ *,
        uint16_t );
static GATE_DRIVER_HANDLER_STATE HAL_GateDriver_Status(GATE_DRIVER_OBJ * );
GATE_DRIVER_OBJ inverterGateDriver[BSP_GATE_DRIVER_INSTANCE_COUNT];
static GATE_DRIVER_HOST_INTERFACE gateDriverInterface[BSP_GATE_DRIVER_INSTANCE_COUNT];

void HAL_GateDriver_FaultClear(GATE_DRIVER_OBJ *);
void HAL_GateDriver_AutoBaudRequest(GATE_DRIVER_OBJ *);

/*****************************************************************************/
/* Section: HAF board functions                                              */
/*****************************************************************************/

HAL_BOARD_STATUS HAL_Board_Configure(void)
{
    /* Configure the PWM driver and collect status information of this task */
    HAL_BOARD_STATUS status = HAL_GateDriver_Configure(&inverterGateDriver[BSP_GATE_DRIVER_A_INDEX],BSP_GATE_DRIVER_A_INDEX);
        
    return status;
}

HAL_BOARD_STATUS HAL_Board_Service(void)
{
    
    HAL_BOARD_STATUS status = HAL_GateDriver_Status(&inverterGateDriver[BSP_GATE_DRIVER_A_INDEX]);
    
    return status;    
}

void HAL_Board_FaultClear(void)
{
    HAL_GateDriver_FaultClear(&inverterGateDriver[BSP_GATE_DRIVER_A_INDEX]);
}

void HAL_Board_AutoBaudRequest(void)
{
    HAL_GateDriver_AutoBaudRequest(&inverterGateDriver[BSP_GATE_DRIVER_A_INDEX]);
}
/*****************************************************************************/
/* Section: Private driver interface functions                               */
/*****************************************************************************/
/**
 * Hardware Access Function to configure the Gate Driver 
 * @param pGateDriver gate driver data
 * @param gateDriverID The instance of the gate Driver on the board
 * @return GATEDRIVER_HANDLER_STATE returns the task status
 * @example
 * <code>
 * status = HAL_GateDriver_Configure(GATEDRIVER_A);
 * </code>
 */
GATE_DRIVER_HANDLER_STATE HAL_GateDriver_Configure(GATE_DRIVER_OBJ *pGateDriver,
        uint16_t gateDriverID)
{    
    /* Waits for  Gate Driver Scheduler Timeout,if activated ; 
     * This allows wait states between different events */
    if (HAL_GateDriver_SchedulerRun(&pGateDriver->timeout) == true)
    {

        switch(pGateDriver->configState)
        {
            case GATE_DRIVER_CONFIG_UNINITIALISED:
                pGateDriver->gateDriverID = gateDriverID;
                if (HAL_GateDriver_Initialize(pGateDriver))
                {
                    pGateDriver->configState = GATE_DRIVER_CONFIG_DISABLED;
                }
                else
                {
                    pGateDriver->configState = GATE_DRIVER_CONFIG_ERROR;
                }
            break;
            case GATE_DRIVER_CONFIG_DISABLED:
                HAL_GateDriver_Enable(pGateDriver);
                pGateDriver->configState = GATE_DRIVER_CONFIG_DISCONNECTED;
            break;
            case GATE_DRIVER_CONFIG_DISCONNECTED:
                HAL_GateDriver_Connect(pGateDriver->pHostInterface);
                pGateDriver->configState = GATE_DRIVER_CONFIG_AUTOBAUD;
            break;
            case GATE_DRIVER_CONFIG_AUTOBAUD:
                if ((uint16_t)HAL_GateDriver_AutoBaudSequence(pGateDriver)  == 
                                                               GATE_DRIVER_DONE)
                {
                    pGateDriver->configState = GATE_DRIVER_CONFIG_CONNECTED;
                    pGateDriver->tryAgainCount = 0;
                }
            break;
            case GATE_DRIVER_CONFIG_TRYAGAIN:
                if (pGateDriver->tryAgainCount < GATE_DRIVER_TRYCOUNT_MAX)
                {
                    pGateDriver->tryAgainCount++;
                    HAL_GateDriver_Disable(pGateDriver);
                    pGateDriver->configState = GATE_DRIVER_CONFIG_DISABLED;

                }
                else
                {
                    pGateDriver->tryAgainCount = GATE_DRIVER_TRYCOUNT_MAX;
                    pGateDriver->configState = GATE_DRIVER_CONFIG_ERROR;
                }
            break;
            case GATE_DRIVER_CONFIG_CONNECTED:
                pGateDriver->configState = HAL_GateDriver_Install(pGateDriver);
                if (pGateDriver->configState==GATE_DRIVER_CONFIG_INSTALLED)
                {
                    pGateDriver->activeState = GATE_DRIVER_OP_BUSY;
                    pGateDriver->tryAgainCount = 0;
                }
            break;
                /* After configuring/installing the gate driver this function 
                 * (i.e. when configState = GATE_DRIVER_CONFIG_INSTALLED,
                 * does not have any further actions to perform,hence
                 * function exit by returning configState */
            case GATE_DRIVER_CONFIG_INSTALLED:
                /* Similarly ,if an error is encountered ,it 
                 * indicates there is some issue with communication channel,
                 * or the configuration message sent to the gate driver or 
                 * similar.
                 * This (configState = GATE_DRIVER_CONFIG_ERROR) ,requires 
                 * attention from the user,and hence function does nothing and
                 * returns configState to the application indicating the same.*/
            case GATE_DRIVER_CONFIG_ERROR:

            default:
            break;  
        }
    }
    return pGateDriver->configState;
}

/**
 * Hardware Access Function to read the Gate Driver Status 
 * @param pGateDriver gate driver data
 * @return GATEDRIVER_HANDLER_STATE returns the task status
 * @example
 * <code>
 * status = HAF_GateDriver_Status(GATEDRIVER_A);
 * </code>
 */
GATE_DRIVER_HANDLER_STATE HAL_GateDriver_Status(GATE_DRIVER_OBJ *pGateDriver)
{    
    /* Waits for  Gate Driver Scheduler Timeout,if activated ; 
     * This allows wait states between different events */
    switch(pGateDriver->activeState)
    {
        case GATE_DRIVER_CONFIG_TRYAGAIN:
            if ((HAL_GateDriver_SchedulerRun(&pGateDriver->timeout) == true))
            {
                if (pGateDriver->tryAgainCount < GATE_DRIVER_TRYCOUNT_MAX)
                {
                    pGateDriver->tryAgainCount++;
                    pGateDriver->statusRegIndex = 0;
                    pGateDriver->pHostInterface->ReceiveBufferEmpty();
                    pGateDriver->activeState = GATE_DRIVER_OP_DONE;
                }
                else
                {
                    pGateDriver->tryAgainCount = GATE_DRIVER_TRYCOUNT_MAX;
                    pGateDriver->statusRegIndex = 0;
                    pGateDriver->activeState = GATE_DRIVER_OP_ERROR;
                }
            }
        break;
        case GATE_DRIVER_OP_WARNING: 
        case GATE_DRIVER_OP_ERROR:
        case GATE_DRIVER_OP_READY:        
        case GATE_DRIVER_OP_DONE:
            if(pGateDriver->autoBaudRequest == 1)            
            {
                pGateDriver->activeState = GATE_DRIVER_OP_AUTOBAUD;
                pGateDriver->timeoutResidue = pGateDriver->timeout;
                pGateDriver->timeout = 0;
            }
            if ((HAL_GateDriver_SchedulerRun(&pGateDriver->timeout) == true))
            {
                pGateDriver->activeState = GATE_DRIVER_OP_AUTOBAUD;
            }
        break;
        case GATE_DRIVER_OP_AUTOBAUD:
            if ((HAL_GateDriver_SchedulerRun(&pGateDriver->timeout) == true))
            {
                /* Call the Gate Driver read Status Function */
                if ((uint16_t)HAL_GateDriver_AutoBaudSequence(pGateDriver)  == 
                                                               GATE_DRIVER_DONE)
                {
                    if(pGateDriver->autoBaudRequest == 1)
                    {
                        pGateDriver->activeState = GATE_DRIVER_OP_DONE;
                        pGateDriver->autoBaudRequest = 0;
                    }
                    else
                    {
                        pGateDriver->activeState = GATE_DRIVER_OP_BUSY;
                        pGateDriver->timeoutResidue = 0;
                        pGateDriver->timeout = 0;
                    }
                }
            }
        break;
        case GATE_DRIVER_OP_BUSY:
            if ((HAL_GateDriver_SchedulerRun(&pGateDriver->timeout) == true))
            {
                pGateDriver->activeState = HAL_GateDriver_ReadStatus(pGateDriver);
                if(pGateDriver->activeState == GATE_DRIVER_OP_DONE)
                {
                    pGateDriver->autoBaudRequest = 0;
                }
            }
         break;
            
        default:
            pGateDriver->activeState = GATE_DRIVER_OP_DONE;
        break;  
    }
    /* Returns the Gate Driver Operation Status */
    return pGateDriver->activeState;
}
/**
 * Function to Initialize the Gate Driver 
 * @param pGateDriver gate driver data
 * @return true = success false = failure
 * @example
 * <code>
 * HAL_GateDriver_Initialize(&gateDriver);
 * </code>
 */
static bool HAL_GateDriver_Initialize(GATE_DRIVER_OBJ *pGateDriver)
{
    /* Resets the Gate Driver Object */
    memset(pGateDriver,0,sizeof(GATE_DRIVER_OBJ));
    
    /* Initialize the CE signal port SFR and bit mask */
    pGateDriver->chipEnablePort = BSP_LATCH_GATE_DRIVER_A_CE_PORT;
    pGateDriver->chipEnableMask = 1<<BSP_LATCH_GATE_DRIVER_A_CE_BIT;
    
    /* Ensures  that the Gate Driver is disabled */
    HAL_GateDriver_Disable(pGateDriver);
    /* Initialize Gate Driver Command Options*/
    pGateDriver->cmd0Data.byte = 0;
    pGateDriver->cmd0Data.sleepMode = GATE_DRIVER_ENTERS_STANDBY_MODE;
    pGateDriver->cmd0Data.extFETUVLockOut = GATE_DRIVER_EXTFET_UVLO_ENABLED;
    pGateDriver->cmd0Data.extFETSCDetection = GATE_DRIVER_EXTFET_SCDETECT_ENABLED;
    pGateDriver->cmd0Data.extFETOCLimit = GATE_DRIVER_EXTFET_OCLIM_1V; 
        
    pGateDriver->cmd2Data.byte = 0;
    pGateDriver->cmd2Data.blankingTime = GATE_DRIVER_BLANKING_4000nS;
    pGateDriver->cmd2Data.deadTime = GATE_DRIVER_DEADTIME_250nS;
    
    /* Store Pointer to Host Interface structure */
    pGateDriver->pHostInterface = &gateDriverInterface[pGateDriver->gateDriverID];
    /* Store Gate Driver ID back in the Gate Driver Structure*/
    /* Function call to initialize Communication Interface between controller 
     and Gate Driver*/
    return HAL_GateDriver_Initialize_CommChannel(pGateDriver);
}

/**
 * Function to enable the Gate Driver. The function sets the chip enable pin 
 * of the gate driver(CE = 1) by holding the port pin 'HIGH'.
 * @param pGateDriver pointer to the gate driver data
 * @return None
 * @example
 * <code>
 * HAL_GateDriver_Enable(pGateDriver);
 * </code>
 */
void HAL_GateDriver_Enable(GATE_DRIVER_OBJ *pGateDriver)
{
   *(pGateDriver->chipEnablePort) |= pGateDriver->chipEnableMask;
}

/**
 * Function to disable the Gate Driver.The function resets the chip enable pin 
 * of the gate driver(CE = 0) by holding the port pin 'LOW'.
 * @param pGateDriver pointer to the gate driver data
 * @return None
 * @example
 * <code>
 * HAL_GateDriver_Disable(pGateDriver);
 * </code>
 */
void HAL_GateDriver_Disable(GATE_DRIVER_OBJ *pGateDriver)
{
   *(pGateDriver->chipEnablePort) &= ~pGateDriver->chipEnableMask;
}

/**
 * Function to establish communication interface between gate driver and 
 * controller 
 * @param pHostInterface - pointer to the Host Interface
 * @return None
 * @example
 * <code>
 * HAL_GateDriver_Connect(&hostInterface);
 * </code>
 */
void HAL_GateDriver_Connect(GATE_DRIVER_HOST_INTERFACE* pHostInterface)
{ 
    uint16_t baudRate = pHostInterface->baudRateScaler;
    pHostInterface->InterruptReceiveDisable();
    pHostInterface->InterruptReceiveFlagClear();
    pHostInterface->InterruptTransmitDisable();
    pHostInterface->InterruptTransmitFlagClear();
    pHostInterface->Initialize();
    pHostInterface->SpeedModeStandard();
    pHostInterface->BaudRateDividerSet(baudRate);
    pHostInterface->ModuleEnable();
    pHostInterface->TransmitModeDisable();
    pHostInterface->status =  GATE_DRIVER_INTERFACE_ENABLED;
}

/**
 * Function configure the gate driver by writing to Driver Configuration 
 * Registers 0-2.This is achieved by sending commands SET_CONFIG_0(0x81),
 * SET_CONFIG_1(0x83),SET_CONFIG_2(0X87) along with value/option to be written 
 * in to driver configuration registers.in response to command sent by the host.
 * Also the HAL_GateDriver_Install() validates the acknowledgment message from 
 * driver to confirm the successful receipt of commands and  options.
 * 
 * @param pGateDriver gate driver data
 * @return GATE_DRIVER_CONFIG_STATE status of the Gate Driver configuration
 * @example
 * <code>
 * HAL_GateDriver_Install(&gateDriver);
 * </code>
 */
GATE_DRIVER_CONFIG_STATE HAL_GateDriver_Install(GATE_DRIVER_OBJ* pGateDriver)
{ 
    uint16_t ackData[4];
    GATE_DRIVER_CONFIG_STATE state = GATE_DRIVER_CONFIG_CONNECTED; 
    switch(pGateDriver->installState)
    {
        case GATE_DRIVER_STATE_CMD_SET_CFG0:
            pGateDriver->pHostInterface->TransmitModeEnable();
            pGateDriver->pHostInterface->ReceiveBufferEmpty();
            HAL_GateDriver_Write(pGateDriver->pHostInterface,GATE_DRIVER_CMD_SET_CFG0);
            HAL_GateDriver_Write(pGateDriver->pHostInterface,pGateDriver->cmd0Data.byte);
            pGateDriver->timeout = GATE_DRIVER_SETCONFIG_SETRXPIN_TIMEOUT;
            pGateDriver->installState = GATE_DRIVER_STATE_ACK_RX_CFG0;
        break;
        case GATE_DRIVER_STATE_ACK_RX_CFG0:
            pGateDriver->pHostInterface->TransmitModeDisable();
            pGateDriver->timeout = GATE_DRIVER_SETCONFIG_TIMEOUT;
            pGateDriver->installState = GATE_DRIVER_STATE_ACK_SET_CFG0;

        break;
        case GATE_DRIVER_STATE_ACK_SET_CFG0:
            ackData[0] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            ackData[1] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            ackData[2] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            ackData[3] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            pGateDriver->installState = GATE_DRIVER_STATE_CMD_SET_CFG2;
            if (ackData[0] != GATE_DRIVER_CMD_SET_CFG0)
            {
                state = GATE_DRIVER_CONFIG_TRYAGAIN;
                pGateDriver->timeout = GATE_DRIVER_SETCONFIG_TIMEOUT;
                pGateDriver->installState = GATE_DRIVER_STATE_CMD_SET_CFG0;
            }
            if (ackData[1] != pGateDriver->cmd0Data.byte)
            {
                state = GATE_DRIVER_CONFIG_TRYAGAIN; 
                pGateDriver->timeout = GATE_DRIVER_SETCONFIG_TIMEOUT;
                pGateDriver->installState = GATE_DRIVER_STATE_CMD_SET_CFG0;
            }
            if (ackData[2] != GATE_DRIVER_ACK_SET_CFG0)
            {
                state = GATE_DRIVER_CONFIG_TRYAGAIN;
                pGateDriver->timeout = GATE_DRIVER_SETCONFIG_TIMEOUT;
                pGateDriver->installState = GATE_DRIVER_STATE_CMD_SET_CFG0;
            }
            if (ackData[3] != pGateDriver->cmd0Data.byte)
            {
                state = GATE_DRIVER_CONFIG_TRYAGAIN;
                pGateDriver->timeout = GATE_DRIVER_SETCONFIG_TIMEOUT;
                pGateDriver->installState = GATE_DRIVER_STATE_CMD_SET_CFG0;
            }
        break;
        case GATE_DRIVER_STATE_CMD_SET_CFG2:
            pGateDriver->pHostInterface->TransmitModeEnable();
            pGateDriver->pHostInterface->ReceiveBufferEmpty();
            HAL_GateDriver_Write(pGateDriver->pHostInterface,GATE_DRIVER_CMD_SET_CFG2);
            HAL_GateDriver_Write(pGateDriver->pHostInterface,pGateDriver->cmd2Data.byte);
            pGateDriver->timeout = GATE_DRIVER_SETCONFIG_SETRXPIN_TIMEOUT;
            pGateDriver->installState = GATE_DRIVER_STATE_ACK_RX_CFG2;
        break;
        case GATE_DRIVER_STATE_ACK_RX_CFG2:
            pGateDriver->pHostInterface->TransmitModeDisable();
            pGateDriver->timeout = GATE_DRIVER_SETCONFIG_TIMEOUT;
            pGateDriver->installState = GATE_DRIVER_STATE_ACK_SET_CFG2;

        break;
        case GATE_DRIVER_STATE_ACK_SET_CFG2:
            ackData[0] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            ackData[1] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            ackData[2] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            ackData[3] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            pGateDriver->installState = GATE_DRIVER_STATE_CMD_SET_CFG0;
            state = GATE_DRIVER_CONFIG_INSTALLED;
            if (ackData[0] != GATE_DRIVER_CMD_SET_CFG2)
            {
                state = GATE_DRIVER_CONFIG_TRYAGAIN;
                pGateDriver->timeout = GATE_DRIVER_SETCONFIG_TIMEOUT;
                pGateDriver->installState = GATE_DRIVER_STATE_CMD_SET_CFG0;
            }
            if (ackData[1] != pGateDriver->cmd2Data.byte)
            {
                state = GATE_DRIVER_CONFIG_TRYAGAIN; 
                pGateDriver->timeout = GATE_DRIVER_SETCONFIG_TIMEOUT;
                pGateDriver->installState = GATE_DRIVER_STATE_CMD_SET_CFG0;
            }
            if (ackData[2] != GATE_DRIVER_ACK_SET_CFG2)
            {
                state = GATE_DRIVER_CONFIG_TRYAGAIN;
                pGateDriver->timeout = GATE_DRIVER_SETCONFIG_TIMEOUT;
                pGateDriver->installState = GATE_DRIVER_STATE_CMD_SET_CFG0;
            }
            if (ackData[3] != pGateDriver->cmd2Data.byte)
            {
                state = GATE_DRIVER_CONFIG_TRYAGAIN;
                pGateDriver->timeout = GATE_DRIVER_SETCONFIG_TIMEOUT;
                pGateDriver->installState = GATE_DRIVER_STATE_CMD_SET_CFG0;
            }

        break;
        default:
            state = GATE_DRIVER_CONFIG_TRYAGAIN;
            pGateDriver->timeout = GATE_DRIVER_SETCONFIG_TIMEOUT;
            pGateDriver->installState = GATE_DRIVER_STATE_CMD_SET_CFG0;
        break;
    }
    return state;
}

/**
 * Function to read the status from Gate Driver 
 * @param pGateDriver gate driver data
 * @return GATE_DRIVER_OPERATION_STATE status of the Gate Driver Status Read
 * @example
 * <code>
 * HAL_GateDriver_ReadStatus(&gateDriver);
 * </code>
 */
GATE_DRIVER_OPERATION_STATE HAL_GateDriver_ReadStatus(GATE_DRIVER_OBJ* pGateDriver)
{ 
    GATE_DRIVER_OPERATION_STATE state = GATE_DRIVER_OP_BUSY;
    uint16_t ackData[3];
    switch(pGateDriver->statusRegIndex)
    {
        case GATE_DRIVER_STATE_CMD_READ_STATUS0:
            pGateDriver->pHostInterface->TransmitModeEnable();
            pGateDriver->pHostInterface->ReceiveBufferEmpty();
            HAL_GateDriver_Write(pGateDriver->pHostInterface,GATE_DRIVER_CMD_GET_STATUS0);
            pGateDriver->timeout = GATE_DRIVER_READSTATUS_SETRXPIN_TIMEOUT;
            pGateDriver->statusRegIndex = GATE_DRIVER_STATE_ACK_RX_STATUS0;
        break;
        case GATE_DRIVER_STATE_ACK_RX_STATUS0:
            pGateDriver->pHostInterface->TransmitModeDisable();
            pGateDriver->timeout = GATE_DRIVER_READSTATUS_TIMEOUT;
            pGateDriver->statusRegIndex = GATE_DRIVER_STATE_ACK_READ_STATUS0;
        break;
        case GATE_DRIVER_STATE_ACK_READ_STATUS0:
            ackData[0] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            ackData[1] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            ackData[2] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            pGateDriver->statusRegIndex = GATE_DRIVER_STATE_CMD_READ_STATUS1;
            if (ackData[0] != GATE_DRIVER_CMD_GET_STATUS0)
            {
                state = GATE_DRIVER_OP_TRYAGAIN;
                pGateDriver->timeout = GATE_DRIVER_READSTATUS_TIMEOUT;
                pGateDriver->statusRegIndex = GATE_DRIVER_STATE_CMD_READ_STATUS0;
            }
            if (ackData[1] != GATE_DRIVER_ACK_GET_STATUS0)
            {
                state = GATE_DRIVER_OP_TRYAGAIN; 
                pGateDriver->timeout = GATE_DRIVER_READSTATUS_TIMEOUT;
                pGateDriver->statusRegIndex = GATE_DRIVER_STATE_CMD_READ_STATUS0;
            }
            pGateDriver->status0Data.byte = (uint8_t)ackData[2];

        break;
        case GATE_DRIVER_STATE_CMD_READ_STATUS1:
            pGateDriver->pHostInterface->TransmitModeEnable();
            pGateDriver->pHostInterface->ReceiveBufferEmpty();
            HAL_GateDriver_Write(pGateDriver->pHostInterface,GATE_DRIVER_CMD_GET_STATUS1);
            pGateDriver->timeout = GATE_DRIVER_READSTATUS_SETRXPIN_TIMEOUT;
            pGateDriver->statusRegIndex = GATE_DRIVER_STATE_ACK_RX_STATUS1;
        break;
        case GATE_DRIVER_STATE_ACK_RX_STATUS1:
            pGateDriver->pHostInterface->TransmitModeDisable();
            pGateDriver->timeout = GATE_DRIVER_READSTATUS_TIMEOUT;
            pGateDriver->statusRegIndex = GATE_DRIVER_STATE_ACK_READ_STATUS1;
        break;
        case GATE_DRIVER_STATE_ACK_READ_STATUS1:
            ackData[0] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            ackData[1] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            ackData[2] = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            pGateDriver->statusRegIndex = GATE_DRIVER_STATE_CMD_READ_STATUS0;
            if (ackData[0] != GATE_DRIVER_CMD_GET_STATUS1)
            {
                state = GATE_DRIVER_OP_TRYAGAIN;
                pGateDriver->timeout = GATE_DRIVER_READSTATUS_TIMEOUT;
                pGateDriver->statusRegIndex = GATE_DRIVER_STATE_CMD_READ_STATUS0;
            }
            if (ackData[1] != GATE_DRIVER_ACK_GET_STATUS1)
            {
                state = GATE_DRIVER_OP_TRYAGAIN;
                pGateDriver->timeout = GATE_DRIVER_READSTATUS_TIMEOUT;
                pGateDriver->statusRegIndex = GATE_DRIVER_STATE_CMD_READ_STATUS0;
            }
            pGateDriver->status1Data.byte = (uint8_t)ackData[2];
            
            state = GATE_DRIVER_OP_DONE;
            if (pGateDriver->status0Data.byte)
            {
                state = GATE_DRIVER_OP_ERROR;
                pGateDriver->statusRegIndex = GATE_DRIVER_STATE_CMD_READ_STATUS0;
            }
            if (pGateDriver->status1Data.byte > GATE_DRIVER_STATUS1_ERRATA_MASK)
            {
                state = GATE_DRIVER_OP_ERROR;
                pGateDriver->statusRegIndex = GATE_DRIVER_STATE_CMD_READ_STATUS0;
            }
            pGateDriver->timeout = GATEDRIVER_STATUS_READ_INTERVAL;
            if (state == GATE_DRIVER_OP_DONE)
            {
                pGateDriver->tryAgainCount = 0;
            }
        break;
        default:
            state = GATE_DRIVER_OP_TRYAGAIN;
            pGateDriver->timeout = GATE_DRIVER_READSTATUS_TIMEOUT;
            pGateDriver->statusRegIndex = GATE_DRIVER_STATE_CMD_READ_STATUS0;
        break;
    }
    return state;
}
/**
 * Function to execute Auto Baud Sequence
 * @param pGateDriver gate driver data
 * @return GATE_DRIVER_OPERATION_STATE status of the Auto Baud Sequence
 * @example
 * <code>
 * HAL_GateDriver_AutoBaudSequence();
 * </code>
 */
GATE_DRIVER_OPERATION_STATE HAL_GateDriver_AutoBaudSequence(GATE_DRIVER_OBJ* pGateDriver)
{ 
    uint16_t interimData = 0;
    GATE_DRIVER_OPERATION_STATE state = GATE_DRIVER_OP_BUSY;

    switch(pGateDriver->autoBaudStateIndex)
    {
        case GATE_DRIVER_STATE_AUTOBAUD_REQUEST:
            pGateDriver->pHostInterface->TransmitModeEnable();
            pGateDriver->pHostInterface->ReceiveBufferEmpty();
            pGateDriver->de2BaudRateData = 
                        pGateDriver->pHostInterface->BaudRateDivisorRead();
            if(pGateDriver->pHostInterface->IsTransmissionComplete() == true)
            {
                pGateDriver->pHostInterface->BaudRateDividerSet
                        (GATE_DRIVER_COMM_ABAUD_BREAK_WINDOW_SCALER);        
                pGateDriver->pHostInterface->TransmitBreakRequestFlagSet();     
                HAL_GateDriver_Write(pGateDriver->pHostInterface,
                                   GATE_DRIVER_ABAUD_BREAK_SEQUENCE_DUMMY_DATA);
                pGateDriver->timeout = GATE_DRIVER_ABAUD_BREAK_SEQUENCE_TIMEOUT;
                pGateDriver->pHostInterface->TransmitModeDisable();
                pGateDriver->pHostInterface->TransmitBufferEmptyFlagClear();
                pGateDriver->autoBaudStateIndex = 
                        GATE_DRIVER_STATE_AUTOBAUD_CHARACTER_RECIEVE;
            }
            else
            {
                state = GATE_DRIVER_OP_TRYAGAIN;
                pGateDriver->autoBaudStateIndex = 
                        GATE_DRIVER_STATE_AUTOBAUD_REQUEST;
                pGateDriver->timeout = GATEDRIVER_STATUS_READ_INTERVAL;
                
            }
        break;
        case GATE_DRIVER_STATE_AUTOBAUD_CHARACTER_RECIEVE:
            pGateDriver->pHostInterface->AutoBaudEnable();
            pGateDriver->timeout = GATE_DRIVER_ABAUD_CHARACTER_RECIEVE_TIMEOUT;
            pGateDriver->autoBaudStateIndex = GATE_DRIVER_STATE_AUTOBAUD_VERIFY;
        break;
        case GATE_DRIVER_STATE_AUTOBAUD_VERIFY:
            if(pGateDriver->pHostInterface->IsAutoBaudComplete() == true)
            {
                interimData = pGateDriver->pHostInterface->BaudRateDivisorRead();
                if((interimData < GATE_DRIVER_COMM_BAUDRATE_SCALER_MAX) &&
                   (interimData > GATE_DRIVER_COMM_BAUDRATE_SCALER_MIN))
                {
                    /* New Baud Rate within the Limit*/
                    pGateDriver->de2BaudRateData = interimData;
                }
                else
                {
                    /* New Baud Rate out of Limit, hence use previous rate*/
                    pGateDriver->pHostInterface->BaudRateDividerSet
                                                 (pGateDriver->de2BaudRateData); 
                }
                state = GATE_DRIVER_OP_DONE;
            }
            else
            {
                pGateDriver->pHostInterface->BaudRateDividerSet
                        (GATE_DRIVER_COMM_BAUDRATE_SCALER);
                pGateDriver->pHostInterface->AutoBaudDisable();
                state = GATE_DRIVER_OP_TRYAGAIN;
            }
            interimData = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            interimData = HAL_GateDriver_Read(pGateDriver->pHostInterface);
            pGateDriver->pHostInterface->ReceiveBufferEmpty();
            pGateDriver->autoBaudStateIndex= GATE_DRIVER_STATE_AUTOBAUD_REQUEST;       
            pGateDriver->timeout = pGateDriver->timeoutResidue;
            pGateDriver->timeoutResidue = GATEDRIVER_STATUS_READ_INTERVAL;
            if (state == GATE_DRIVER_OP_DONE)
            {
                pGateDriver->tryAgainCount = 0;
            }
        break;
        default:
            state = GATE_DRIVER_OP_TRYAGAIN;
            pGateDriver->autoBaudStateIndex = GATE_DRIVER_STATE_AUTOBAUD_REQUEST;
        break;
    }
    return state;
}

/**
 * Function to write to the Gate Driver
 * @param pHostInterface - pointer to the Host Interface
 * @param data - the data to be send out
 * @return None
 * @example
 * <code>
 * HAL_GateDriver_Write(&hostInterface,data);
 * </code>
 */
void HAL_GateDriver_Write(GATE_DRIVER_HOST_INTERFACE* pHostInterface,uint16_t data)
{ 
    if (pHostInterface->StatusBufferFullTransmitGet() == 0)
    {
        pHostInterface->DataWrite(data);
    }
}

/**
 * Function to Read Data from Gate Driver 
 * @param pHostInterface - pointer to the Host Interface
 * @return the data read from the Driver ; 
 *         Function returns GATE_DRIVER_INVALID_DATA if data is not available
 * @example
 * <code>
 * HAL_GateDriver_Read(&hostInterface);
 * </code>
 */
uint16_t HAL_GateDriver_Read(GATE_DRIVER_HOST_INTERFACE* pHostInterface)
{ 
    /** Function returns invalid data,if buffer is empty*/
    uint16_t data = GATE_DRIVER_INVALID_DATA;
    if (pHostInterface->IsReceiveBufferDataReady())
    {
        data = pHostInterface->DataRead();
    }
    return data;
}

/**
 * Implements scheduler and returns the status 
 * @param pTimeout - pointer to the scheduler counter
 * @return true =  timeout expired  false = timer is on and live
 * @example
 * <code>
 * HAL_GateDriver_SchedulerRun();
 * </code>
 */
static bool HAL_GateDriver_SchedulerRun(uint16_t *pTimeout) 
{ 
    if (*pTimeout == 0)
    {
        return true;
    }
    else
    {
        *pTimeout = *pTimeout - 1;
        return false;
    }
}

/**
 * Function to Initialize Communication  Channel .
 * @param pGateDriver - Gate Driver Data
 * @return true =  success  false = failed
 * @example
 * <code>
 * HAL_GateDriver_Initialize_CommunicationCHL(&inverterGateDriver[gateDriverID]);
 * </code>
 */
static bool HAL_GateDriver_Initialize_CommChannel(GATE_DRIVER_OBJ *pGateDriver)
{
    GATE_DRIVER_HOST_INTERFACE *pHostInterface = pGateDriver->pHostInterface;
    
    /* Resets Gate Driver Communication Interface Object */
    memset(pHostInterface,0,sizeof(GATE_DRIVER_HOST_INTERFACE));
    
    /* Identify and initialize Communication peripheral used for establishing
     * communication interface between gate driver and controller */
    pGateDriver->interfaceChannelID = BSP_GATE_DRIVER_INTERFACE_A_UARTMODULE;
    
    /* Initialize function pointer based on the communication module number.*/
    return HAL_GateDriver_HookUARTFunctions(pHostInterface,pGateDriver->interfaceChannelID);
}
/**
 * Function to initialize function pointer .The function assumes that 
 * appropriate UART header is included.
 * @param pHostInterface - pointer to host interface
 * @return true =  success  false = failed
 * @example
 * <code>
 * HAL_GateDriver_HookUARTFunctions(pHostInterface);
 * </code>
 */
static bool HAL_GateDriver_HookUARTFunctions(GATE_DRIVER_HOST_INTERFACE *pHostInterface,uint16_t interfaceChannelID)
{
    if (interfaceChannelID == BSP_GATE_DRIVER_INTERFACE_A_UARTMODULE)
    {
        /* Initialize function pointer based on the communication module number.*/
        pHostInterface->InterruptTransmitFlagClear = 
                UART2_InterruptTransmitFlagClear;
        pHostInterface->InterruptReceiveFlagClear = 
                UART2_InterruptReceiveFlagClear;
        pHostInterface->InterruptTransmitEnable = 
                UART2_InterruptTransmitEnable;
        pHostInterface->InterruptTransmitDisable = 
                UART2_InterruptTransmitDisable;
        pHostInterface->InterruptReceiveEnable = 
                UART2_InterruptReceiveEnable;
        pHostInterface->InterruptReceiveDisable = 
                UART2_InterruptReceiveDisable;
        pHostInterface->Initialize = 
                UART2_Initialize;
        pHostInterface->SpeedModeStandard = 
                UART2_SpeedModeStandard;
        pHostInterface->SpeedModeHighSpeed = 
                UART2_SpeedModeHighSpeed;
        pHostInterface->BaudRateDividerSet = 
                UART2_BaudRateDividerSet;
        pHostInterface->ModuleDisable = 
                UART2_ModuleDisable;
        pHostInterface->ModuleEnable = 
                UART2_ModuleEnable;
        pHostInterface->TransmitModeEnable = 
                UART2_TransmitModeEnable;
        pHostInterface->TransmitModeDisable = 
                UART2_TransmitModeDisable;
        pHostInterface->StatusBufferFullTransmitGet = 
                UART2_StatusBufferFullTransmitGet;
        pHostInterface->DataWrite = UART2_DataWrite;
        pHostInterface->DataRead = UART2_DataRead;
        pHostInterface->IsReceiveBufferDataReady = 
                UART2_IsReceiveBufferDataReady;
        pHostInterface->ReceiveBufferEmpty = 
                UART2_ReceiveBufferEmpty;
        pHostInterface->IsTransmissionComplete = 
                UART2_IsTransmissionComplete;
        pHostInterface->TransmitBreakRequestFlagSet = 
                UART2_TransmitBreakRequestFlagSet;
        pHostInterface->AutoBaudEnable = 
                UART2_AutoBaudEnable;
        pHostInterface->AutoBaudDisable = 
                UART2_AutoBaudDisable;
        pHostInterface->IsAutoBaudComplete = 
                UART2_IsAutoBaudComplete;
        pHostInterface->BaudRateDivisorRead = 
                UART2_BaudRateDivisorRead;
        pHostInterface->TransmitBufferEmptyFlagClear = 
                UART2_TransmitBufferEmptyFlagClear;
        pHostInterface->baudRatebps = 0;
        pHostInterface->periphClockHz = 0;
        pHostInterface->baudRateScaler = 
                GATE_DRIVER_COMM_BAUDRATE_SCALER;
        pHostInterface->status =
                GATE_DRIVER_INTERFACE_INITIALIZED;
        return true;
    }
    else
    {
        return false;
    }
}
/**
 * Function to clear the Fault. The function resets the chip enable pin 
 * of the gate driver(CE = 0) by holding the port pin 'LOW'.Then re enables 
 * after specific interval.
 * @param pGateDriver pointer to the gate driver data
 * @return None
 * @example
 * <code>
 * HAL_GateDriver_Enable(pGateDriver);
 * </code>
 */
void HAL_GateDriver_FaultClear(GATE_DRIVER_OBJ *pGateDriver)
{
    *(pGateDriver->chipEnablePort) &= ~pGateDriver->chipEnableMask; 
   __delay_us(GATE_DRIVER_FAULT_CLEARING_PULSE_WIDTH);
    *(pGateDriver->chipEnablePort) |= pGateDriver->chipEnableMask;
}
/**
 * Function to hold the auto baud request.
 * @param pGateDriver pointer to the gate driver data
 * @return None
 * @example
 * <code>
 * HAL_GateDriver_AutoBaudRequest(pGateDriver);
 * </code>
 */
void HAL_GateDriver_AutoBaudRequest(GATE_DRIVER_OBJ *pGateDriver)
{
    if (pGateDriver->autoBaudRequest == 0)
    {
        pGateDriver->autoBaudRequest = 1 ; 
    }
}
