#include "Digitizer.h"

Digitizer::Digitizer()
{
    // Execute(CAEN_DGTZ_Success;
    Execute(CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_USB, 0, 0, 0, &runparameters.handle));
    Execute(CAEN_DGTZ_GetInfo(runparameters.handle, &BoardInfo));
    for (uint32_t i = 0 ; i < BoardInfo.Channels; i++) ChannelTriggerMode.push_back(CAEN_DGTZ_TRGMODE_DISABLED);
    NTrigChannels = ChannelTriggerMode.size();

    std::cout << "Connected to CAEN Digitizer Model " << BoardInfo.ModelName << std::endl;
    std::cout << "ROC FPGA Release is " << BoardInfo.ROC_FirmwareRel << std::endl;
    std::cout << "AMC FPGA Release is " << BoardInfo.AMC_FirmwareRel << std::endl;

    // Check firmware rivision (DPP firmwares cannot be used with WaveDump
    sscanf(BoardInfo.AMC_FirmwareRel, "%d", &MajorNumber);
    if (MajorNumber >= 128)
    {
        std::cout << "This digitizer has a DPP firmware!" << std::endl;
    }
}

void Digitizer::Program(Dconfig &conf)
{
    // SetVerbose(verbose);
    SetDconfig(conf);
    Execute(CAEN_DGTZ_Reset(runparameters.handle));
    Execute(CAEN_DGTZ_SetRecordLength(runparameters.handle, conf.Samples));
    Execute(CAEN_DGTZ_SetPostTriggerSize(runparameters.handle, conf.PostTrigger));
    Execute(CAEN_DGTZ_SetMaxNumEventsBLT(runparameters.handle, conf.EventAggregation));
    Execute(CAEN_DGTZ_SetAcquisitionMode(runparameters.handle, CAEN_DGTZ_SW_CONTROLLED));
    Execute(CAEN_DGTZ_SetExtTriggerInputMode(runparameters.handle, CAEN_DGTZ_TRGMODE_ACQ_ONLY));
    Execute(CAEN_DGTZ_WriteRegister(runparameters.handle, 0x8028, conf.Vpp));

    for (int i = 0; i < dconfig.NumChannels; i++)
    {
        switch (conf.chtype_db[i])
        {
            case 0:
                // conf.SetChThreshold(i, 0);
                std::cout << "Disable channel " << i << std::endl;
                ChannelTriggerMode[i] = CAEN_DGTZ_TRGMODE_DISABLED;
                break;
            case 1:
                EnabledMask += (1 << i);
                std::cout << "Enable channel " << i << " in acquisition mode" << std::endl;
                ChannelTriggerMode[i] = CAEN_DGTZ_TRGMODE_DISABLED;
                Execute(CAEN_DGTZ_SetChannelDCOffset(runparameters.handle, i, conf.dcoffset_db[i]));
                break;
            case 2:
                EnabledMask += (1 << i);
                std::cout << "Enable channel " << i << " in trigger mode" << std::endl;
                ChannelTriggerMode[i] = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
                Execute(CAEN_DGTZ_SetChannelDCOffset(runparameters.handle, i, conf.dcoffset_db[i]));
                Execute(CAEN_DGTZ_SetChannelTriggerThreshold(runparameters.handle, i, conf.thresh_db[i]));//for dpp is limited to ~7000
                if (conf.trigpol_db[i] > 0)
                {
                    Execute(CAEN_DGTZ_SetTriggerPolarity(runparameters.handle, i, CAEN_DGTZ_TriggerOnRisingEdge));
                    Execute(CAEN_DGTZ_SetChannelPulsePolarity(runparameters.handle, i, CAEN_DGTZ_PulsePolarityPositive));
                }
                else
                {
                    Execute(CAEN_DGTZ_SetTriggerPolarity(runparameters.handle, i, CAEN_DGTZ_TriggerOnFallingEdge));
                    Execute(CAEN_DGTZ_SetChannelPulsePolarity(runparameters.handle, i, CAEN_DGTZ_PulsePolarityNegative));
                }
                break;
        }
    }

    std::bitset<8> mask(EnabledMask);
    std::cout << "Enabled Channel Mask is "<< mask << std::endl;
    Execute(CAEN_DGTZ_SetChannelEnableMask(runparameters.handle, EnabledMask));

    if (conf.TrigType) AndTriggger(conf);
    else OrTriggger();	
        
    std::cout << "Programming finished!" << std::endl;

    // SetVerbose(0);
}

Digitizer::~Digitizer()
{
    Execute(CAEN_DGTZ_CloseDigitizer(runparameters.handle));
}

CAEN_DGTZ_ErrorCode Digitizer::Execute(CAEN_DGTZ_ErrorCode ret)
{
    if (ret)
    {
        std::cout << errors[abs((int)ret)] << " (code " << ret << ")" << std::endl;
    }

    return ret;
}

void Digitizer::AndTriggger(Dconfig &conf)
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
    uint32_t MajLvl = coincidenceBitset.count() - 1 - conf.MajorityLevel*((conf.MajorityLevel + 1) < NTrigChannels);
    std::cout << "Coincidence Enable Mask = " << coincidenceEnableMask << "  MajorityLevel = " << MajLvl <<"  Coincidence Window = "<< conf.CoincidenceWindow << std::endl;

    uint32_t message = coincidenceEnableMask + (MajLvl << 24) + (conf.CoincidenceWindow << 20);
    uint32_t coincAddress = 0x810C;
    uint32_t mask = 0xFFFFFFFF;    
    WriteRegisterBitmask(coincAddress, message, mask);
}

void Digitizer::OrTriggger()
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
        Execute(CAEN_DGTZ_SetChannelSelfTrigger(runparameters.handle, mode, pair_chmask));
    }
    std::cout << std::endl;
}

void Digitizer::WriteRegisterBitmask(uint32_t address, uint32_t data, uint32_t mask)
{
    uint32_t d32 = 0xFFFFFFFF;
    Execute(CAEN_DGTZ_ReadRegister(runparameters.handle, address, &d32));
    data &= mask;
    d32 &= ~mask;
    d32 |= data;
    Execute(CAEN_DGTZ_WriteRegister(runparameters.handle, address, d32));
}

DigiData &Digitizer::ReadEvent()
{
     // printf("read\n");
    
    uint32_t NEvents = 0;
    uint32_t lstatus;
    uint32_t ElapsedTime, CurrentTime;
    CAEN_DGTZ_ErrorCode ret;

    Execute(CAEN_DGTZ_ReadData(runparameters.handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize));

    if (BufferSize != 0) {ret = Execute(CAEN_DGTZ_GetNumEvents(runparameters.handle, buffer, BufferSize, &NEvents));}
    else {ret = Execute(CAEN_DGTZ_ReadRegister(runparameters.handle, CAEN_DGTZ_ACQ_STATUS_ADD, &lstatus));}
	  // if (lstatus & (0x1 << 19)) {
	  //   exit(0);}
    
    runparameters.Nbytes += BufferSize;
    runparameters.Nevs += NEvents;
    runparameters.nevent += NEvents;
    CurrentTime = GetCurrentTime();
    ElapsedTime = CurrentTime - runparameters.PrevRateTime;
    runparameters.nCycles++;

    if (ElapsedTime > 1000)
    {
        if (runparameters.Nbytes == 0)
        {
	        if (ret == CAEN_DGTZ_Timeout) std::cout << "Timeout...\n" << std::endl;
            else printf("No data...\n");
        }
        else printf("Reading at %.2f MB/s (Trg Rate: %.2f Hz, %d events)\n", (float)runparameters.Nbytes/((float)ElapsedTime*1048.576f), (float)runparameters.Nevs*1000.0f/(float)ElapsedTime, runparameters.nevent);
        runparameters.nCycles= 0;
        runparameters.Nbytes = 0;
        runparameters.Nevs = 0;
        runparameters.PrevRateTime = CurrentTime;
    }	

    /* Analyze data */
    //if (NEvents==0) return 0;
    for(int j = 0; j < (int)NEvents; j++) 
    {
      /* Get one event from the readout buffer */
      Execute(CAEN_DGTZ_GetEventInfo(runparameters.handle, buffer, BufferSize, j, &digidata.EventInfo, &EventPtr));

      /* decode the event */
      Execute(CAEN_DGTZ_DecodeEvent(runparameters.handle, EventPtr, (void**)&digidata.Data));

    }
    return digidata;
}

void Digitizer::AllocateEvents()
{
  buffer = NULL;
  EventPtr = NULL;
  
  digidata.Data = NULL; /* generic event struct with 16 bit data (10, 12, 14 and 16 bit digitizers */
  Execute(CAEN_DGTZ_AllocateEvent(runparameters.handle, (void**)&digidata.Data));
  Execute(CAEN_DGTZ_MallocReadoutBuffer(runparameters.handle, &buffer, &AllocatedSize)); /* WARNING: This malloc must be done after the digitizer programming */
  printf("Buffer for events is allocated\n");

}