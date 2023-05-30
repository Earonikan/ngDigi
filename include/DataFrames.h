#pragma once

#include "stdafx.h"

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
    int NumChannels;
    
    bool TrigType;

    //float test;
    // std::vector<float> test_db;
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
    CAEN_DGTZ_UINT16_EVENT_t *Event16 = nullptr;;
    CAEN_DGTZ_EventInfo_t EventInfo;
    CAEN_DGTZ_ErrorCode ret;
    uint32_t BufferSize;
    uint32_t NEvents;
};

struct Rconfig
{
    int Nevents;
    
    int Actime;
    long StartTime;

    bool ReadTemp = 0;
};

struct RunParameters
{
	int Nbytes;
	int Nevs;
	int nCycles;
    int nevent;
    // int handle;
    int run_status; //0 - finished, 1 - standby, 2 - running
    
	long PrevRateTime;

    bool NeedToUpdate;
};

struct trigger_t
{
    int ch_num;
    TH1F* hWaveform  = nullptr;
    TH1F* hAmplitude = nullptr;
    TH1F* hAmplitude_bsl  = nullptr;
    TH1F* hCharge  = nullptr;
    TH1F* hCharge_bsl  = nullptr;
};
    
struct signal_t
{
    int ch_num;
    TH1F* hWaveform_1 = nullptr;
    TH1F* hWaveform_2 = nullptr;
    TH1F* hAmplitude_1 = nullptr;
    TH1F* hAmplitude_2 = nullptr;        
    TH1F* hAmplitude_bsl = nullptr;
    TH1F* hCharge_1 = nullptr;
    TH1F* hCharge_2 = nullptr;
    TH1F* hCharge_bsl = nullptr;
};

struct HistoCollection
{
    std::vector<trigger_t> trigger_ch;
    std::vector<signal_t> signal_ch;
};