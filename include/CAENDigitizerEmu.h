#pragma once

#include "stdafx.h"

#define MAX_UINT16_CHANNEL_SIZE     (64)
#define MAX_LICENSE_DIGITS          (8)
#define MAX_LICENSE_LENGTH          (MAX_LICENSE_DIGITS * 2 + 1)

#define CAEN_DGTZ_ACQ_STATUS_ADD    (0x8104)

typedef struct 
{
    uint32_t             EventSize;
    uint32_t             BoardId;
    uint32_t             Pattern;
    uint32_t             ChannelMask;
    uint32_t             EventCounter;
    uint32_t             TriggerTimeTag;
} CAEN_DGTZ_EventInfo_t;

typedef struct 
{
    uint32_t            ChSize[MAX_UINT16_CHANNEL_SIZE];       // the number of samples stored in DataChannel array  
    uint16_t            *DataChannel[MAX_UINT16_CHANNEL_SIZE]; // the array of ChSize samples
} CAEN_DGTZ_UINT16_EVENT_t;

typedef struct {
    char                        ModelName[12];
    uint32_t                    Model;
    uint32_t                    Channels;
    uint32_t                    FormFactor;
    uint32_t                    FamilyCode;
    char                        ROC_FirmwareRel[20];
    char                        AMC_FirmwareRel[40];
    uint32_t                    SerialNumber;
    char                        MezzanineSerNum[40];       //used only for x743 boards
    uint32_t                    PCB_Revision;
    uint32_t                    ADC_NBits;
    uint32_t                    SAMCorrectionDataLoaded;        //used only for x743 boards
    int                         CommHandle;
    int                         VMEHandle;
    char                        License[MAX_LICENSE_LENGTH];
} CAEN_DGTZ_BoardInfo_t;

typedef enum 
{
    CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT      = 0L,
    CAEN_DGTZ_SLAVE_TERMINATED_READOUT_2eVME     = 1L,
    CAEN_DGTZ_SLAVE_TERMINATED_READOUT_2eSST     = 2L,
    CAEN_DGTZ_POLLING_MBLT                       = 3L,
    CAEN_DGTZ_POLLING_2eVME                      = 4L,
    CAEN_DGTZ_POLLING_2eSST                      = 5L,
} CAEN_DGTZ_ReadMode_t;

typedef enum
{
    CAEN_DGTZ_Success                       =  0L,    /* Operation completed successfully             */
    CAEN_DGTZ_CommError                     = -1L,    /* Communication error                          */
    CAEN_DGTZ_GenericError                  = -2L,    /* Unspecified error                            */
    CAEN_DGTZ_InvalidParam                  = -3L,    /* Invalid parameter                            */
    CAEN_DGTZ_InvalidLinkType               = -4L,    /* Invalid Link Type                            */
    CAEN_DGTZ_InvalidHandle                 = -5L,    /* Invalid device handle                        */
    CAEN_DGTZ_MaxDevicesError               = -6L,    /* Maximum number of devices exceeded           */
    CAEN_DGTZ_BadBoardType                  = -7L,    /* The operation is not allowed on this type of board           */
    CAEN_DGTZ_BadInterruptLev               = -8L,    /* The interrupt level is not allowed            */
    CAEN_DGTZ_BadEventNumber                = -9L,    /* The event number is bad                          */
    CAEN_DGTZ_ReadDeviceRegisterFail        = -10L,   /* Unable to read the registry                     */
    CAEN_DGTZ_WriteDeviceRegisterFail       = -11L,   /* Unable to write into the registry                */
    CAEN_DGTZ_InvalidChannelNumber          = -13L,   /* The channel number is invalid                 */
    CAEN_DGTZ_ChannelBusy                   = -14L,   /* The Channel is busy                               */
    CAEN_DGTZ_FPIOModeInvalid               = -15L,   /* Invalid FPIO Mode                                */
    CAEN_DGTZ_WrongAcqMode                  = -16L,   /* Wrong acquisition mode                        */
    CAEN_DGTZ_FunctionNotAllowed            = -17L,   /* This function is not allowed for this module    */
    CAEN_DGTZ_Timeout                       = -18L,   /* Communication Timeout                            */
    CAEN_DGTZ_InvalidBuffer                 = -19L,   /* The buffer is invalid                         */
    CAEN_DGTZ_EventNotFound                 = -20L,   /* The event is not found                        */
    CAEN_DGTZ_InvalidEvent                  = -21L,   /* The vent is invalid                            */
    CAEN_DGTZ_OutOfMemory                   = -22L,   /* Out of memory                                    */
    CAEN_DGTZ_CalibrationError              = -23L,   /* Unable to calibrate the board                    */
    CAEN_DGTZ_DigitizerNotFound             = -24L,   /* Unable to open the digitizer                    */
    CAEN_DGTZ_DigitizerAlreadyOpen          = -25L,   /* The Digitizer is already open                    */    
    CAEN_DGTZ_DigitizerNotReady             = -26L,   /* The Digitizer is not ready to operate            */    
    CAEN_DGTZ_InterruptNotConfigured        = -27L,   /* The Digitizer has not the IRQ configured            */
    CAEN_DGTZ_DigitizerMemoryCorrupted      = -28L,   /* The digitizer flash memory is corrupted        */
    CAEN_DGTZ_DPPFirmwareNotSupported       = -29L,   /* The digitizer dpp firmware is not supported in this lib version */
    CAEN_DGTZ_InvalidLicense                = -30L,   /* Invalid Firmware License */
    CAEN_DGTZ_InvalidDigitizerStatus        = -31L,   /* The digitizer is found in a corrupted status */
    CAEN_DGTZ_UnsupportedTrace              = -32L,   /* The given trace is not supported by the digitizer */
    CAEN_DGTZ_InvalidProbe                  = -33L,   /* The given probe is not supported for the given digitizer's trace */
    CAEN_DGTZ_UnsupportedBaseAddress		    = -34L,   /*  The Base Address is not supported, it's a Desktop device?		*/ 
    CAEN_DGTZ_NotYetImplemented             = -99L,   /* The function is not yet implemented            */
} CAEN_DGTZ_ErrorCode;

typedef enum
{
    CAEN_DGTZ_USB                        = 0L,
    CAEN_DGTZ_OpticalLink                = 1L,
    CAEN_DGTZ_USB_A4818_V2718            = 2L,
    CAEN_DGTZ_USB_A4818_V3718            = 3L,
    CAEN_DGTZ_USB_A4818_V4718            = 4L,
    CAEN_DGTZ_USB_A4818                  = 5L,
    CAEN_DGTZ_ETH_V4718                  = 6L,
    CAEN_DGTZ_USB_V4718                  = 7L,
} CAEN_DGTZ_ConnectionType;

typedef enum 
{
    CAEN_DGTZ_TRGMODE_DISABLED         = 0L,
    CAEN_DGTZ_TRGMODE_EXTOUT_ONLY      = 2L,
    CAEN_DGTZ_TRGMODE_ACQ_ONLY         = 1L,
    CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT   = 3L,
} CAEN_DGTZ_TriggerMode_t;

typedef enum {
    CAEN_DGTZ_TriggerOnRisingEdge        = 0L,
    CAEN_DGTZ_TriggerOnFallingEdge       = 1L,
} CAEN_DGTZ_TriggerPolarity_t;


typedef enum {
    CAEN_DGTZ_PulsePolarityPositive        = 0,
    CAEN_DGTZ_PulsePolarityNegative        = 1,
} CAEN_DGTZ_PulsePolarity_t;

typedef enum
{
    CAEN_DGTZ_SW_CONTROLLED            = 0L,
    CAEN_DGTZ_S_IN_CONTROLLED          = 1L,
    CAEN_DGTZ_FIRST_TRG_CONTROLLED     = 2L,
	CAEN_DGTZ_LVDS_CONTROLLED          = 3L,
} CAEN_DGTZ_AcqMode_t;

CAEN_DGTZ_ErrorCode CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_ConnectionType LinkType, int LinkNum, int ConetNode, uint32_t VMEBaseAddress, int *handle);

CAEN_DGTZ_ErrorCode CAEN_DGTZ_CloseDigitizer(int handle);

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_DecodeEvent(int handle, char *evtPtr, void **Evt){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_SWStartAcquisition(int handle){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_SWStopAcquisition(int handle){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_AllocateEvent(int handle, void **Evt){return CAEN_DGTZ_Success;}

CAEN_DGTZ_ErrorCode CAEN_DGTZ_ReadData(int handle, CAEN_DGTZ_ReadMode_t mode, char *buffer, uint32_t *bufferSize); //{return CAEN_DGTZ_Success;}

CAEN_DGTZ_ErrorCode CAEN_DGTZ_GetInfo(int handle, CAEN_DGTZ_BoardInfo_t *BoardInfo);

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_Reset(int handle){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_SetChannelSelfTrigger(int handle, CAEN_DGTZ_TriggerMode_t mode, uint32_t channelmask){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_SetRecordLength(int handle, uint32_t size){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_GetRecordLength(int handle, uint32_t *size){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_SetChannelEnableMask(int handle, uint32_t mask){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_SetPostTriggerSize(int handle, uint32_t percent){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_SetMaxNumEventsBLT(int handle, uint32_t numEvents){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_SetAcquisitionMode(int handle, CAEN_DGTZ_AcqMode_t mode){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_SetExtTriggerInputMode(int handle, CAEN_DGTZ_TriggerMode_t mode){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_WriteRegister(int handle, uint32_t Address, uint32_t Data){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_SetChannelDCOffset(int handle, uint32_t channel, uint32_t Tvalue){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_SetChannelTriggerThreshold(int handle, uint32_t channel, uint32_t Tvalue){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_SetTriggerPolarity(int handle, uint32_t channel, CAEN_DGTZ_TriggerPolarity_t Polarity){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_SetChannelPulsePolarity(int handle, uint32_t channel, CAEN_DGTZ_PulsePolarity_t pol){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_ReadRegister(int handle, uint32_t Address, uint32_t *Data){return CAEN_DGTZ_Success;}

CAEN_DGTZ_ErrorCode CAEN_DGTZ_GetNumEvents(int handle, char *buffer, uint32_t buffsize, uint32_t *numEvents);// {return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_GetEventInfo(int handle, char *buffer, uint32_t buffsize, int32_t numEvent, CAEN_DGTZ_EventInfo_t *eventInfo, char **EventPtr){return CAEN_DGTZ_Success;}

inline CAEN_DGTZ_ErrorCode CAEN_DGTZ_MallocReadoutBuffer(int handle, char **buffer, uint32_t *size){return CAEN_DGTZ_Success;}