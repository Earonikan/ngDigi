#include "CAENDigitizerEmu.h"


CAEN_DGTZ_ErrorCode CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_ConnectionType LinkType, int LinkNum, int ConetNode, uint32_t VMEBaseAddress, int *handle)
{
    std::cout << "Open Digitizer!" << std::endl;
    return CAEN_DGTZ_Success;
}

CAEN_DGTZ_ErrorCode CAEN_DGTZ_CloseDigitizer(int handle)
{
    std::cout << "Close Digitizer!" << std::endl;
    return CAEN_DGTZ_Success;
}

CAEN_DGTZ_ErrorCode CAEN_DGTZ_GetInfo(int handle, CAEN_DGTZ_BoardInfo_t *BoardInfo)
{
    
    strcpy(BoardInfo->ModelName, "fake digi");
    BoardInfo->Model = 8888;
    BoardInfo->Channels = 8;
    BoardInfo->FormFactor = 0;
    BoardInfo->FamilyCode = 0;
    strcpy(BoardInfo->ROC_FirmwareRel, "ROC fake 0");
    strcpy(BoardInfo->AMC_FirmwareRel, "AMC fake 0");
    BoardInfo->SerialNumber = 8888;
    strcpy(BoardInfo->MezzanineSerNum, "fake");       //used only for x743 boards
    BoardInfo->PCB_Revision = 0;
    BoardInfo->ADC_NBits = 0;
    BoardInfo->SAMCorrectionDataLoaded = 0;        //used only for x743 boards
    BoardInfo->CommHandle = 0;
    BoardInfo->VMEHandle = 0;
    strcpy(BoardInfo->License, "licence");
    return CAEN_DGTZ_Success;
}

