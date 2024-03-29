#include "Digitizer.h"

void Digitizer::Open(CAEN_DGTZ_ConnectionType LinkType, int LinkNum, int ConetNode, uint32_t VMEBaseAddress)
{
    // std::cout << "HERE!!!!" << std::endl;
    Execute(CAEN_DGTZ_OpenDigitizer(LinkType, LinkNum, ConetNode, VMEBaseAddress, &handle));
    Execute(CAEN_DGTZ_GetInfo(handle, &BoardInfo));
    for (uint32_t i = 0 ; i < BoardInfo.Channels; i++) ChannelTriggerMode.push_back(CAEN_DGTZ_TRGMODE_DISABLED);
    // NTrigChannels = ChannelTriggerMode.size();

    std::cout << "Connected to CAEN Digitizer Model " << BoardInfo.ModelName << std::endl;
    std::cout << "ROC FPGA Release is " << BoardInfo.ROC_FirmwareRel << std::endl;
    std::cout << "AMC FPGA Release is " << BoardInfo.AMC_FirmwareRel << std::endl;

    // Check firmware rivision (DPP firmwares cannot be used with WaveDump
    int MajorNumber;
    sscanf(BoardInfo.AMC_FirmwareRel, "%d", &MajorNumber);
    if (MajorNumber >= 128)
    {
        std::cout << "This digitizer has a DPP firmware!" << std::endl;
    }
}

void Digitizer::Program(Dconfig dconfig)
{
    NTrigChannels = 0;
    EnabledMask = 0;
    Execute(CAEN_DGTZ_Reset(handle));
    Execute(CAEN_DGTZ_SetRecordLength(handle, dconfig.Samples));
    Execute(CAEN_DGTZ_SetPostTriggerSize(handle, dconfig.PostTrigger));
    Execute(CAEN_DGTZ_SetMaxNumEventsBLT(handle, dconfig.EventAggregation));
    Execute(CAEN_DGTZ_SetAcquisitionMode(handle, CAEN_DGTZ_SW_CONTROLLED));
    Execute(CAEN_DGTZ_SetExtTriggerInputMode(handle, CAEN_DGTZ_TRGMODE_ACQ_ONLY));
    Execute(CAEN_DGTZ_WriteRegister(handle, 0x8028, dconfig.Vpp));

    for (int i = 0; i < dconfig.NumChannels; i++)
    {
        switch (dconfig.chtype_db[i])
        {
            case 0:
                // dconfig.SetChThreshold(i, 0);
                std::cout << "Disable channel " << i << std::endl;
                ChannelTriggerMode[i] = CAEN_DGTZ_TRGMODE_DISABLED;
                break;
            case 1:
                EnabledMask += (1 << i);
                std::cout << "Enable channel " << i << " in acquisition mode" << std::endl;
                ChannelTriggerMode[i] = CAEN_DGTZ_TRGMODE_DISABLED;
                Execute(CAEN_DGTZ_SetChannelDCOffset(handle, i, dconfig.dcoffset_db[i]));
                break;
            case 2:
                EnabledMask += (1 << i);
                std::cout << "Enable channel " << i << " in trigger mode" << std::endl;
                ChannelTriggerMode[i] = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
                Execute(CAEN_DGTZ_SetChannelDCOffset(handle, i, dconfig.dcoffset_db[i]));
                Execute(CAEN_DGTZ_SetChannelTriggerThreshold(handle, i, dconfig.thresh_db[i]));//for dpp is limited to ~7000
                if (dconfig.trigpol_db[i] > 0)
                {
                    Execute(CAEN_DGTZ_SetTriggerPolarity(handle, i, CAEN_DGTZ_TriggerOnRisingEdge));
                    Execute(CAEN_DGTZ_SetChannelPulsePolarity(handle, i, CAEN_DGTZ_PulsePolarityPositive));
                }
                else
                {
                    Execute(CAEN_DGTZ_SetTriggerPolarity(handle, i, CAEN_DGTZ_TriggerOnFallingEdge));
                    Execute(CAEN_DGTZ_SetChannelPulsePolarity(handle, i, CAEN_DGTZ_PulsePolarityNegative));
                }
                NTrigChannels++;
                break;
        }
    }

    std::bitset<8> mask(EnabledMask);
    std::cout << "Enabled Channel Mask is "<< mask << std::endl;
    Execute(CAEN_DGTZ_SetChannelEnableMask(handle, EnabledMask));

    if (dconfig.TrigType) AndTriggger(dconfig);
    else OrTriggger(dconfig);	
        
    std::cout << "Programming finished!" << std::endl;

    // SetVerbose(0);
}

Digitizer::~Digitizer()
{
    Execute(CAEN_DGTZ_CloseDigitizer(handle));
}

CAEN_DGTZ_ErrorCode Digitizer::Execute(CAEN_DGTZ_ErrorCode ret)
{
    if (ret)
    {
        std::cout << errors[abs((int)ret)] << " (code " << ret << ")" << std::endl;
    }

    return ret;
}

void Digitizer::AndTriggger(Dconfig dconfig)
{
    int coincidenceEnableMask = 0;

    uint32_t triggerLogicBaseAddress = 0x1084;

    for (int i = 0; i < dconfig.NumChannels; i = i + 2)
    {
        if (ChannelTriggerMode[i] != CAEN_DGTZ_TRGMODE_DISABLED)
        {
            WriteRegisterBitmask(triggerLogicBaseAddress + 256*i, 0x1, 0xFFFFFFFF);
            coincidenceEnableMask += (1 << (i/2));
        }
        else if (ChannelTriggerMode[i+1] != CAEN_DGTZ_TRGMODE_DISABLED)
        {
            WriteRegisterBitmask(triggerLogicBaseAddress + 256*i, 0x2, 0xFFFFFFFF);
            coincidenceEnableMask += (1 << (i/2));
        }
    }
    
    std::bitset<8> coincidenceBitset(coincidenceEnableMask);
    // std::cout << "Coincidence Bitset count = " << coincidenceBitset.count() << "  DMajorityLevel = " << dconfig.MajorityLevel <<"  NTrigChannels = "<< NTrigChannels << std::endl;
    uint32_t MajLvl = coincidenceBitset.count() - 1 - dconfig.MajorityLevel*((dconfig.MajorityLevel + 1) <= NTrigChannels);
    std::cout << "Coincidence Enable Mask = " << coincidenceEnableMask << "  MajorityLevel = " << MajLvl <<"  Coincidence Window = "<< dconfig.CoincidenceWindow << std::endl;

    uint32_t message = coincidenceEnableMask + (MajLvl << 24) + (dconfig.CoincidenceWindow << 20);
    uint32_t coincAddress = 0x810C;
    uint32_t mask = 0xFFFFFFFF;    
    WriteRegisterBitmask(coincAddress, message, mask);
}

void Digitizer::OrTriggger(Dconfig dconfig)
{ 
    int triggerLogicBaseAddress = 0x1084;
    CAEN_DGTZ_TriggerMode_t mode = CAEN_DGTZ_TRGMODE_DISABLED;
    uint32_t pair_chmask = 0;

    std::cout << "Next channels included in or-logic: ";
  
    for (int i = 0; i < dconfig.NumChannels; i = i + 2)
    {
        if (ChannelTriggerMode[i] != CAEN_DGTZ_TRGMODE_DISABLED)
        {
            WriteRegisterBitmask(triggerLogicBaseAddress + 256*i, 0x1, 0xFFFFFFFF);
            pair_chmask = (0x1 << i);
            mode = ChannelTriggerMode[i];
            std::cout << i << " ";
        }
        else if (ChannelTriggerMode[i+1] != CAEN_DGTZ_TRGMODE_DISABLED)
        {
            WriteRegisterBitmask(triggerLogicBaseAddress + 256*i, 0x2, 0xFFFFFFFF);
            pair_chmask = (0x2 << i);
            mode = ChannelTriggerMode[i+1];
            std::cout << i + 1 << " ";
        }
        pair_chmask &= EnabledMask;
        Execute(CAEN_DGTZ_SetChannelSelfTrigger(handle, mode, pair_chmask));
    }
    std::cout << std::endl;
}

void Digitizer::WriteRegisterBitmask(uint32_t address, uint32_t data, uint32_t mask)
{
    uint32_t d32 = 0xFFFFFFFF;
    Execute(CAEN_DGTZ_ReadRegister(handle, address, &d32));
    data &= mask;
    d32 &= ~mask;
    d32 |= data;
    Execute(CAEN_DGTZ_WriteRegister(handle, address, d32));
}

DigiData Digitizer::ReadEvent()
{   
    DigiData digidata;
    uint32_t lstatus;

    Execute(CAEN_DGTZ_ReadData(handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &digidata.BufferSize));

    if (digidata.BufferSize != 0) {digidata.ret = Execute(CAEN_DGTZ_GetNumEvents(handle, buffer, digidata.BufferSize, &digidata.NEvents));}
    else {digidata.ret = Execute(CAEN_DGTZ_ReadRegister(handle, CAEN_DGTZ_ACQ_STATUS_ADD, &lstatus));}
	  // if (lstatus & (0x1 << 19)) {
	  //   exit(0);}

    /* Analyze data */
    //if (NEvents==0) return 0;
    for(int j = 0; j < (int)digidata.NEvents; j++) 
    {
      /* Get one event from the readout buffer */
      Execute(CAEN_DGTZ_GetEventInfo(handle, buffer, digidata.BufferSize, j, &digidata.EventInfo, &EventPtr));

      /* decode the event */
      Execute(CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&digidata.Event16));

    }

    return digidata;
}

void Digitizer::AllocateEvents()
{
  buffer = NULL;
  EventPtr = NULL;
  
  CAEN_DGTZ_UINT16_EVENT_t *Event16;
  
//   digidata_.Data = NULL; /* generic event struct with 16 bit data (10, 12, 14 and 16 bit digitizers */
  Execute(CAEN_DGTZ_AllocateEvent(handle, (void**)&Event16));
  Execute(CAEN_DGTZ_MallocReadoutBuffer(handle, &buffer, &AllocatedSize)); /* WARNING: This malloc must be done after the digitizer programming */
  printf("Buffer for events is allocated\n");

}

void Digitizer::StartAquisition() {Execute(CAEN_DGTZ_SWStartAcquisition(handle));}

void Digitizer::StopAquisition() {Execute(CAEN_DGTZ_SWStopAcquisition(handle));}