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
    CAEN_DGTZ_UINT16_EVENT_t *Event16;
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

struct HistoCollection
{
    std::vector<int> trigger_ch;
    std::vector<int> signal_ch;

    std::map<int, TH1F*> Wfm_trigger;
    std::map<int, TH1F*> Wfm_signal_1;
    // std::map<int, TH1F*> Wfm_signal_2;

    // std::map<int, TH1F*> Ampl_signal_1;
    // std::map<int, TH1F*> Charge_signal_1;

    // std::map<int, TH1F*> Ampl_signal_2;
    // std::map<int, TH1F*> Charge_signal_2;
    
    // std::map<int, TH1F*> Ampl_signal_bsl;
    // std::map<int, TH1F*> Charge_signal_bsl;

    // std::map<int, TH1F*> Ampl_trigger;
    // std::map<int, TH1F*> Charge_trigger;

    // std::map<int, TH1F*> Ampl_trigger_bsl;
    // std::map<int, TH1F*> Charge_trigger_bsl;

};