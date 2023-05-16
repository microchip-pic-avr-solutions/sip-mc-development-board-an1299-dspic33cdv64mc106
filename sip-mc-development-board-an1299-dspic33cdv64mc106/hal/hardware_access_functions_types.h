/**
  @File Name:
    hardware_access_functions_types.h

  @Summary:
    This file includes type defines for the hardware access function module.

  @Description:
    This file includes type defines for the hardware access function module.
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

#ifndef __HAF_TYPES_H
#define __HAF_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


/******************************************************************************/
/* Section: Public type defines                                               */										
/******************************************************************************/

typedef enum tagHAL_BOARD_STATUS
{ 
    /** Board is not ready */
    BOARD_NOT_READY = 0,  
    /** Board error */
    BOARD_ERROR = 1,
    /** Board is busy, try again later */
    BOARD_BUSY = 3,
    /** Board ready/OK */
    BOARD_READY = 4
}HAL_BOARD_STATUS;


/** UART status */
typedef enum tagUART_STATUS
{
    /** Indicates that Receive buffer has data, at least one more character can be read */
    UART_RX_DATA_AVAILABLE = (1 << 0),
    
    /** Indicates that Receive buffer has overflowed */
    UART_RX_OVERRUN_ERROR = (1 << 1),

    /** Indicates that Framing error has been detected for the current character */
    UART_FRAMING_ERROR = (1 << 2),

    /** Indicates that Parity error has been detected for the current character */
    UART_PARITY_ERROR = (1 << 3),

    /** Indicates that Receiver is Idle */
    UART_RECEIVER_IDLE = (1 << 4),

    /** Indicates that the last transmission has completed */
    UART_TX_COMPLETE = (1 << 8),

    /** Indicates that Transmit buffer is full */
    UART_TX_FULL = (1 << 9)
}UART_STATUS;

/******************************************************************************/
/* Section: Private types used within the MCP802x driver                      */
/******************************************************************************/

/** 
 * Gate Driver Communication Interface States 
 */
typedef enum tagGATE_DRIVER_INTERFACE_STATUS
{ 
    /** Gate Driver Communication 
     * Interface is un-initialized.
     * This is the default state after 
     * POR  or BOR or any other reset */
    GATE_DRIVER_INTERFACE_UNINITIALISED  = 0,  
    /** Gate Driver Communication 
     * Interface error */
    GATE_DRIVER_INTERFACE_ERROR = 1,
            
    /** States 2-3 are reserved for future use */
            
    /** Gate Driver Communication 
     * Interface is established  and 
     * communication module is 
     * operational */    
    GATE_DRIVER_INTERFACE_ENABLED   = 4, 
    /** States 5-6 are reserved for future use */
    /** Gate Driver Communication 
    * Interface is initialized but not communication module is not enabled  */     
    GATE_DRIVER_INTERFACE_INITIALIZED = 5           
}GATE_DRIVER_INTERFACE_STATUS;

/** Structure to hold Gate Driver Communication Interface functions
 * and its hardware /firmware attributes */
typedef struct  
{
    GATE_DRIVER_INTERFACE_STATUS status;
    uint32_t baudRatebps;
    uint32_t periphClockHz;
    uint16_t baudRateScaler;
    
    void (*InterruptTransmitFlagClear)(void); 
    void (*InterruptReceiveFlagClear)(void);
    void (*InterruptTransmitEnable)(void);
    void (*InterruptTransmitDisable)(void);
    void (*InterruptReceiveEnable)(void);
    void (*InterruptReceiveDisable)(void);
    void (*Initialize)(void);
    void (*SpeedModeStandard)(void);
    void (*SpeedModeHighSpeed)(void);
    void (*BaudRateDividerSet) (uint16_t);
    void (*ModuleDisable)(void); 
    void (*ModuleEnable)(void); 
    void (*TransmitModeEnable)(void);
    void (*TransmitModeDisable)(void);
    void (*ReceiveBufferEmpty)(void);
    bool (*IsTransmissionComplete)(void);
    void (*TransmitBreakRequestFlagSet)(void);
    void (*AutoBaudEnable)(void);
    void (*AutoBaudDisable)(void);
    bool (*IsAutoBaudComplete)(void);
    bool (*StatusBufferFullTransmitGet)(void);
    bool (*IsReceiveBufferDataReady)(void);
    void (*DataWrite)(uint16_t);
    uint16_t (*DataRead)(void);
    uint16_t (*BaudRateDivisorRead)(void);
    void (*TransmitBufferEmptyFlagClear)(void);
    
} GATE_DRIVER_HOST_INTERFACE;

/** Structure holds the Gate Driver STATUS0 Register */
typedef union tagGATE_DRIVER_STATUS0_DATA
{
	uint8_t byte;
	struct
	{
        /** Fault Status bit is '1', if At least one Fault is active */
		uint8_t			fault               : 1; 
        /** Over temperature Protection Warning bit is '1',
            if Device junction temperature is > +115°C */
		uint8_t	overTempWarning     : 1; 
        /** Over temperature Protection Fault bit is '1',if
            Device junction temperature is > +160°C */
		uint8_t	overTempError       : 1;
        /** Input Under voltage Fault if VDD < 5.5V */
		uint8_t	inputUVError        : 1;
        /** Input Over voltage Fault if VDD > 32V */
		uint8_t inputOVError        : 1;
        /** Power Control Status bits (configuration lost if non-zero value) */
		uint8_t	powerContrloStatus  : 3;   /* bit <5:7>  */
	};
}GATE_DRIVER_STATUS0_DATA;

/** Structure holds the MCP8024 STATUS1 Register */
typedef union tagGATE_DRIVER_STATUS1_DATA
{
	uint8_t 	byte;
	struct
	{
        /** VREG LDO Under voltage Fault bit is set to '1' when
            VREG LDO output voltage < 2.9V */
		uint8_t  VREGUVFault                    : 1;
        /** Unimplemented */
		uint8_t                                 : 1;
        /** External MOSFET Under Voltage Lock Out (UVLO) */
		uint8_t  extFETUVFault              	: 1;
        /** External MOSFET Over Current Detection */
		uint8_t  extFETOCFault          		: 1;
        /** Unimplemented (4bits) */
		uint8_t                                 : 4; /* bit7 */
	};
}GATE_DRIVER_STATUS1_DATA;

/** Structure holds the Gate Driver Configuration Register 0 */
typedef union tagGATE_DRIVER_CFG0_DATA
{
	uint8_t 	byte;
	struct
	{
        /** External MOSFET Over Current Limit */
		uint8_t extFETOCLimit                  : 2;
        /** External MOSFET Short Circuit Detect */
		uint8_t extFETSCDetection              : 1;
        /** External MOSFET Under Voltage Lock Out Disable */
		uint8_t extFETUVLockOut                : 1;
        /** Unimplemented */
        uint8_t            	    			   : 1;
        /** SLEEP: Sleep Mode bit  */
        uint8_t sleepMode      				   : 1;
        /** Unimplemented */
        uint8_t            	    			   : 1;
        /** Reserved */
        uint8_t            	    			   : 1;
        /** Unimplemented */
		uint8_t                                : 1; /* bit7 */		
	};
}GATE_DRIVER_CFG0_DATA;

/** Structure holds the Gate Driver Configuration Register 2 */
typedef union tagGATE_DRIVER_CFG2_DATA
{
	uint8_t 	byte;
	struct
	{
        /** Driver Blanking Time - ignore Switching Spikes */
		uint8_t  blankingTime                   : 2;
        /** Driver Dead Time for PWMxH/PWMxL Inputs */
        uint8_t  deadTime                       : 3;
        /** Reserved - three bits */
		uint8_t                                 : 3;
	};
}GATE_DRIVER_CFG2_DATA;

/** 
 * Blanking Time Supported by the Driver  
 */
typedef enum tagGATE_DRIVER_BLANKING_TIME
{ 
    /** Driver Blanking Time is 4000nS (Default) */
    GATE_DRIVER_BLANKING_4000nS  = 0,  
    /** Driver Blanking Time is 2000nS  */            
    GATE_DRIVER_BLANKING_2000nS  = 1,    
    /** Driver Blanking Time is 1000nS  */
    GATE_DRIVER_BLANKING_1000nS  = 2,  
    /** Driver Blanking Time is 500nS */            
    GATE_DRIVER_BLANKING_500nS  = 3            
} GATE_DRIVER_BLANKING_TIME;

/** 
 * Blanking Time Supported by the Driver   
 */
typedef enum tagGATE_DRIVER_DEADTIME
{ 
    /** Driver Dead Time is 2us (Default) */
    GATE_DRIVER_DEADTIME_2000nS  = 0,
    /** Driver Dead Time is 1750nS  */
    GATE_DRIVER_DEADTIME_1750nS  = 1,
    /** Driver Dead Time is 1500nS  */
    GATE_DRIVER_DEADTIME_1500nS  = 2,
    /** Driver Dead Time is 1250nS  */
    GATE_DRIVER_DEADTIME_1250nS  = 3,
    /** Driver Dead Time is 1uS  */
    GATE_DRIVER_DEADTIME_1000nS  = 4, 
    /** Driver Dead Time is 750nS  */
    GATE_DRIVER_DEADTIME_750nS  = 5,
    /** Driver Dead Time is 500nS  */
    GATE_DRIVER_DEADTIME_500nS  = 6, 
    /** Driver Dead Time is 250nS */
    GATE_DRIVER_DEADTIME_250nS  = 7            
} GATE_DRIVER_DEADTIME;

/** 
 * External MOSFET Over Current Limits Supported by the Driver   
 */
typedef enum tagGATE_DRIVER_EXTFET_OCLIM
{ 
    /** Driver External MOSFET Over Current Limit 0.250V(Default) */
    GATE_DRIVER_EXTFET_OCLIM_250mV  = 0, 
    /** Driver External MOSFET Over Current Limit 0.500V */
    GATE_DRIVER_EXTFET_OCLIM_500mV  = 1,  
    /** Driver External MOSFET Over Current Limit 0.750V */
    GATE_DRIVER_EXTFET_OCLIM_750mV  = 2,  
    /** Driver External MOSFET Over Current Limit 1.000V */
    GATE_DRIVER_EXTFET_OCLIM_1V  = 3           
} GATE_DRIVER_EXTFET_OCLIM;

/** 
 * Driver Feature - External MOSFET Short Circuit Detection         
 */
typedef enum tagGATE_DRIVER_EXTFET_SCDETECT
{ 
    /** Driver External MOSFET  short circuit Detection Enabled (Default)*/
    GATE_DRIVER_EXTFET_SCDETECT_ENABLED   = 0, 
    /** Driver External MOSFET  short circuit Detection Disabled */
    GATE_DRIVER_EXTFET_SCDETECT_DISABLED  = 1  
} GATE_DRIVER_EXTFET_SCDETECT;

/** 
 * Gate Driver - Under Voltage Lock Out Settings - Enable/Disable         
 */
typedef enum tagGATE_DRIVER_UVLO_FEATURE
{ 
    /** Driver Under Voltage Lock out(UVLO)feature is Enabled (Default)*/
    GATE_DRIVER_EXTFET_UVLO_ENABLED   = 0, 
    /** Driver Under Voltage Lock Out(UVLO) Feature is Disabled */
    GATE_DRIVER_EXTFET_UVLO_DISABLED  = 1     
} GATE_DRIVER_UVLO_FEATURE;

/** 
 * Gate Driver Feature : Sleep Mode settings        
 */
typedef enum tagGATE_DRIVER_SLEEP_MODE
{ 
    /** System enters Standby mode, 1 ms after (OE = 0 and SLEEP = 1)(Default)*/
    GATE_DRIVER_ENTERS_STANDBY_MODE = 0,  
    /** System enters Sleep mode, 1 ms after (OE = 0 and SLEEP = 1) */
    GATE_DRIVER_ENTERS_SLEEP_MODE  = 1 
} GATE_DRIVER_SLEEP_MODE;

/** 
 * Gate Driver Commands from host to Driver 
 */
typedef enum tagGATE_DRIVER_DE2_COMMANDS
{ 
    /** Set Configuration Register 0 */
    GATE_DRIVER_CMD_SET_CFG0 = 0x81,      
    /** Get Configuration Register 0 */
    GATE_DRIVER_CMD_GET_CFG0 = 0x82, 
    /** Set Configuration Register 1(Reserved) */
    GATE_DRIVER_CMD_SET_CFG1 = 0x83, 
    /** Get Configuration Register 1(Reserved) */
    GATE_DRIVER_CMD_GET_CFG1 = 0x84,  
    /** Get Status Register 0 */
    GATE_DRIVER_CMD_GET_STATUS0 = 0x85,  
    /** Get Status Register 1 */
    GATE_DRIVER_CMD_GET_STATUS1 = 0x86,
    /** Set Configuration Register 2 */
    GATE_DRIVER_CMD_SET_CFG2 = 0x87,   
    /** Get Configuration Register 2 */
    GATE_DRIVER_CMD_GET_CFG2 = 0x88            

} GATE_DRIVER_DE2_COMMANDS;

/** 
 * Gate Driver responses from driver to host acknowledging commands
 */
typedef enum tagGATE_DRIVER_DE2_ACK_RESPONSES
{ 
    /** Set Configuration Register 0 acknowledgment response */
    GATE_DRIVER_ACK_SET_CFG0 = 0x41,            
    /** Get Configuration Register 0 acknowledgment response */
    GATE_DRIVER_ACK_GET_CFG0 = 0x42,            
    /** Set Configuration Register 1 acknowledgment response(Reserved) */
    GATE_DRIVER_ACK_SET_CFG1 = 0x43,            
    /** Get Configuration Register 1 acknowledgment response(Reserved)*/
    GATE_DRIVER_ACK_GET_CFG1 = 0x44,            
    /** Set Status Register 0 acknowledgment response */
    GATE_DRIVER_ACK_GET_STATUS0 = 0x45,         
    /** Set Status Register 1 acknowledgment response */
    GATE_DRIVER_ACK_GET_STATUS1 = 0x46,         
    /** Set Configuration Register 2 acknowledgment response */
    GATE_DRIVER_ACK_SET_CFG2 = 0x47,            
    /** Get Configuration Register 2 acknowledgment response */
    GATE_DRIVER_ACK_GET_CFG2 = 0x48            
} GATE_DRIVER_DE2_ACK_RESPONSES;

/** 
 * Gate Driver responses from driver to host NOT acknowledging commands 
 */
typedef enum tagGATE_DRIVER_DE2_NACK_RESPONSES
{ 
    /** Set Configuration Register 0 not acknowledged response */
    GATE_DRIVER_NACK_SET_CFG0 = 0x01, 
    /** Get Configuration Register 0 not acknowledged response */
    GATE_DRIVER_NACK_GET_CFG0 = 0x02, 
    /** Set Configuration Register 1 not acknowledged response(Reserved) */
    GATE_DRIVER_NACK_SET_CFG1 = 0x03, 
    /** Get Configuration Register 1 not acknowledged response(Reserved) */
    GATE_DRIVER_NACK_GET_CFG1 = 0x04,
    /** Set Status Register 0 not acknowledged response */
    GATE_DRIVER_NACK_GET_STATUS0 = 0x05, 
    /** Set Status Register 1 not acknowledged response */
    GATE_DRIVER_NACK_GET_STATUS1 = 0x06,  
    /** Set Configuration Register 2 not acknowledged response */
    GATE_DRIVER_NACK_SET_CFG2 = 0x07, 
    /** Get Configuration Register 2 not acknowledged response */
    GATE_DRIVER_NACK_GET_CFG2 = 0x08,           
} GATE_DRIVER_DE2_NACK_RESPONSES;

/** 
 * Gate Driver Internal Operation States - In this case status enquiry
 */
typedef enum tagGATE_DRIVER_OPERATION_STATE
{ 
    /** Gate Driver is not installed or not ready for operation */
    GATE_DRIVER_OP_NOT_READY   = 0,    
    /* Gate Driver encountered error */
    GATE_DRIVER_OP_ERROR = 1,                   
    /* Gate Driver encountered warning */ 
    GATE_DRIVER_OP_WARNING = 2,                 
    /* Gate Driver is Busy doing some try again after some time */
    GATE_DRIVER_OP_TRYAGAIN = 3,                
    /* Gate Driver Handler had completed the Requested Operation */
    GATE_DRIVER_OP_DONE = 4,                    
    /* Gate Driver Handler is performing the Requested Operation */       
    GATE_DRIVER_OP_BUSY = 5,               
    /* Gate Driver is installed and is ready for operation */   
    GATE_DRIVER_OP_READY   = 6, 
    /* Gate Driver performing Auto Baud Sequence */ 
    GATE_DRIVER_OP_AUTOBAUD = 7
}GATE_DRIVER_OPERATION_STATE;

/** 
 * Gate Driver Configuration States (internal)
 */ 
typedef enum tagGATE_DRIVER_CONFIG_STATE
{ 
    /** Gate Driver is un-initialized to its reset state after POR or BOR*/
    GATE_DRIVER_CONFIG_UNINITIALISED  = 0,    
    /* Gate Driver encountered an error*/
    GATE_DRIVER_CONFIG_ERROR = 1,               
            
    /* VALUE 2 is Reserved for future use 
     * Value  2 of all the internal and external ENUM variables is used as
     * warning.Hence it is reserved for future purposes */
            
    /* Gate Driver configuration had some issues ,indicates application
       retry configuring gate driver */     
    GATE_DRIVER_CONFIG_TRYAGAIN = 3,       
    /** Auto Baud sequence is being executed */
    GATE_DRIVER_CONFIG_AUTOBAUD   = 8,            
    /** Gate Driver is installed and ready for operation */  
    GATE_DRIVER_CONFIG_INSTALLED   = 4, 
    /** Communication interface between Gate Driver and Controller is
     * established */
    GATE_DRIVER_CONFIG_CONNECTED   = 5, 
    /** Gate Driver is enabled but communication link is not established */
    GATE_DRIVER_CONFIG_DISCONNECTED   = 6,      
    /** Gate Driver is disabled  */
    GATE_DRIVER_CONFIG_DISABLED = 7           
                        
}GATE_DRIVER_CONFIG_STATE;

/** 
 * Gate Driver Configuration States (internal)
 */ 
typedef enum tagGATE_DRIVER_STATE_CMD_SET_CONFIG
{ 
    /** Gate Driver sends Command to write to Gate Driver Configuration 
    * Register 0 */
    GATE_DRIVER_STATE_CMD_SET_CFG0  = 0, 
    /** The RX pin is set to receive the response from gate driver  
     * corresponding to command CMD_SET_CFG0 and its option */
    GATE_DRIVER_STATE_ACK_RX_CFG0  = 1,
    /** the response received from gate driver to confirm the reception of 
     * command CMD_SET_CFG0 and its option */ 
    GATE_DRIVER_STATE_ACK_SET_CFG0 = 2,
    /** Gate Driver sends Command to write to Gate Driver Configuration 
     * Register 2 */
    GATE_DRIVER_STATE_CMD_SET_CFG2  = 3, 
    /** The RX pin is set to receive the response from gate driver  
     * corresponding to command CMD_SET_CFG0 and its option */
    GATE_DRIVER_STATE_ACK_RX_CFG2  = 4,
    /** the response received from gate driver to confirm the reception of 
     * command CMD_SET_CFG2 and its option */ 
    GATE_DRIVER_STATE_ACK_SET_CFG2 = 5                              
}GATE_DRIVER_STATE_CMD_SET_CONFIG;

/** 
 * Gate Driver Configuration States (internal)
 */ 
typedef enum tagGATE_DRIVER_STATE_CMD_READ_STATUS
{ 
    /** Gate Driver sends Command to read Gate Driver Status Register 0 */
    GATE_DRIVER_STATE_CMD_READ_STATUS0  = 0, 
    /** The RX pin is set to receive the response from gate driver  
     * corresponding to command CMD_READ_STATUS0 */
    GATE_DRIVER_STATE_ACK_RX_STATUS0  = 1,
    /** the response received from gate driver to confirm the reception of 
     * command CMD_READ_STATUS0 and content of Status Register 0*/
    GATE_DRIVER_STATE_ACK_READ_STATUS0 = 2, 
    /** Gate Driver sends Command to read Gate Driver Status Register 1 */
    GATE_DRIVER_STATE_CMD_READ_STATUS1  = 3, 
    /** The RX pin is set to receive the response from gate driver  
     * corresponding to command CMD_READ_STATUS1 */
    GATE_DRIVER_STATE_ACK_RX_STATUS1  = 4,
    /** the response received from gate driver to confirm the reception of 
     * command CMD_READ_STATUS1 and content of Status Register 1*/ 
    GATE_DRIVER_STATE_ACK_READ_STATUS1 = 5                               
}GATE_DRIVER_STATE_CMD_READ_STATUS;

/** 
 * Gate Driver Configuration States (internal)
 */ 
typedef enum tagGATE_DRIVER_STATE_CMD_AUTOBAUD
{ 
    /** Gate Driver sends Command to read Gate Driver Status Register 0 */
    GATE_DRIVER_STATE_AUTOBAUD_REQUEST  = 0, 
    /** The RX pin is set to receive the response from gate driver  
     * corresponding to command CMD_READ_STATUS0 */
    GATE_DRIVER_STATE_AUTOBAUD_CHARACTER_RECIEVE  = 1,
    /** the response received from gate driver to confirm the reception of 
     * command CMD_READ_STATUS0 and content of Status Register 0*/
    GATE_DRIVER_STATE_AUTOBAUD_VERIFY = 2                               
}GATE_DRIVER_STATE_CMD_AUTOBAUD;

/** Structure to hold internal state variables of the Gate Driver*/
typedef struct tagGATE_DRIVER_OBJ
{   
    /** Stores Gate Driver Instance for which structure is us */
    uint16_t gateDriverID;
    /** Has the Communication Interface channel ID (like UART1 or SPI 1 etc */
    uint16_t interfaceChannelID;
    /** Enumeration Constant - Gate Drive Operation State (Internal)*/
    GATE_DRIVER_OPERATION_STATE activeState;
    /** Enumeration constant - Gate Driver COnfiguration State */
    GATE_DRIVER_CONFIG_STATE    configState;
    /** Gate Driver scheduler Timer variable */
    uint16_t timeout;
    /** Gate Driver scheduler Timer Residual */
    uint16_t timeoutResidue;
    /**  This variable keeps track of the installation stages*/
    uint16_t installState;
    /** Variable to track number of times Gate Driver Handler retried to operate
     * a specific operation  */
    uint16_t tryAgainCount;
    /** Index to readConfigurationStatus Array   */
    uint16_t configRegIndex;
    /** Index to the Status Register GetStatus Function */
    uint16_t statusRegIndex;
    /** Index to the Auto Baud Synchronize Function */
    uint16_t autoBaudStateIndex;
    /** Holds the Auto baud Request from the application*/
    uint16_t autoBaudRequest;
    /** Holds the Auto baud Initiate from the application*/
    uint16_t autoBaudInitiate;

    /** Holds the Gate Driver Status 0 Register Value */
    GATE_DRIVER_STATUS0_DATA status0Data ;
    /** Holds the Gate Driver Status 1 Register Value */
    GATE_DRIVER_STATUS1_DATA status1Data ;
    /** Holds the Gate Driver Option to send with Gate Driver COMMAND 0  */
    GATE_DRIVER_CFG0_DATA cmd0Data;
    /** Holds the Gate Driver Option to send with Gate Driver COMMAND 2  */
    GATE_DRIVER_CFG2_DATA cmd2Data;
    /** Holds the Baud Rate Value Obtained after the Auto Baud Sequence  */
    uint16_t de2BaudRateData;
    GATE_DRIVER_HOST_INTERFACE* pHostInterface;
    /** pointer to the GPIO port that controls chip-enable pin for the given
     * instance of PWM driver */
    volatile uint16_t *chipEnablePort;
    /** bit mask for the GPIO port pin that controls chip-enable pin for the
     * given instance of PWM driver */
    uint16_t chipEnableMask;
    /** Variable to track Fault Clear Timeout */
    uint16_t faultClearTimeout;
    /** Variable to track Fault Clear Timeout */
    uint16_t faultActive;
} GATE_DRIVER_OBJ;

typedef enum tagGATE_DRIVER_HANDLER_STATE
{ 
    /** Driver is in reset state after POR  or BOR or Controller reset */
    GATE_DRIVER_NOT_CONFIGURED = 0,  
    /** Reserved for future use,can be returned from Gate Driver handler function
     *  for Inverter with Gate Driver that does need  any initialization*/
    GATE_DRIVER_NOT_APPLICABLE = 0,  
    /** Gate Driver encountered an error */
    GATE_DRIVER_ERROR = 1,           
    /** Gate Driver encountered warning */
    GATE_DRIVER_WARNING = 2,         
    /** Gate Driver Handler could not complete process the requested Operation.
     * Application can request Gate Driver Handler to  perform the same 
     * operation after some time */
    GATE_DRIVER_TRYAGAIN = 3,          
    /** Gate Driver Handler has completed the requested operation */  
    GATE_DRIVER_DONE = 4,              
    /** Gate Driver is executing the requested operation and is progressing  */    
    GATE_DRIVER_BUSY = 5             
}GATE_DRIVER_HANDLER_STATE;

#endif /* __HAF_TYPES_H */
