#pragma once
#include "stdafx.h"
#include "CAENDigitizerEmu.h"

struct Dconfig
{
    std::vector<int> chtype_db;
    std::vector<int> trigpol_db;
    std::vector<int> thresh_db;
    std::vector<int> dcoffset_db;

    uint32_t MajorityLevel;
    uint32_t Vpp;
    uint32_t PostTrigger;
    uint32_t Samples;

    int CoincidenceWindow;
    int EventAggregation;
    
    bool TrigType;
};

struct Aconfig
{
    std::vector<int> intbl_db;
    std::vector<int> intsig_db;

    int rmin; //range for charge histos
    int rmax;
    int WindowWidth;

    bool CreateFit = 0;
};

struct DigiData
{
    CAEN_DGTZ_UINT16_EVENT_t *Data;
    CAEN_DGTZ_EventInfo_t EventInfo;
};

struct RunParameters
{
    uint32_t Nevents;
    int run_status; //0 - finished, 1 - standby, 2 - running
    int Actime;
    long StartTime;

    int NumChannels;

    bool ReadTemp = 0;
};
