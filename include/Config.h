#pragma once

#include "stdafx.h"
#include "ConfigFile.h"

class Config
{
public:
    Config() {;}
    ~Config() {;}
    void ReadAllConfigsFromFile(std::string cfgfilename);

    void SetNevents(uint32_t nevs) {Nevents = nevs;}
    uint32_t GetNevents() {return Nevents;}
    
    void SetActime(int time) {Actime = time;}
    int GetActime() {return Actime;}

    void SetVpp(int vpp) {Vpp = vpp;}
    uint32_t GetVpp() {return Vpp;}

    void SetTrigType(bool trigtype) {TrigType = trigtype;}
    bool GetTrigType() {return TrigType;}

    void SetWindowWidth(int windowwidth) {WindowWidth = windowwidth;}
    int GetWindowWidth() {return WindowWidth;}
    
    void SetCreateFit(bool createfit) {CreateFit = createfit;}
    bool GetCreateFit() {return CreateFit;}

    void SetReadTemp(bool readtemp) {ReadTemp = readtemp;}
    bool GetReadTemp() {return ReadTemp;}

    void SetRmin(int rmin) {this->rmin = rmin;}
    int GetRmin() {return rmin;}

    void SetRmax(int rmax) {this->rmax = rmax;}
    int GetRmax() {return rmax;}

    void SetCoincidenceWindow(int cwindow) {CoincidenceWindow = cwindow;}
    int GetCoincidenceWindow() {return CoincidenceWindow;}

    void SetPostTrigger(uint32_t posttrigger) {PostTrigger = posttrigger;}
    uint32_t GetPostTrigger() {return PostTrigger;}

    void SetNsamples(uint32_t samps) {Samples = samps;}
    uint32_t GetNsamples() {return Samples;}

    void SetNumChannels(int nchs) {NumChannels = nchs;}
    int GetNumChannels() {return NumChannels;}

    void SetMajorityLevel(int maj) {MajorityLevel = maj;}
    int GetMajorityLevel() {return MajorityLevel;}

    void SetEventAggregation(int agg) {EventAggregation = agg;}
    int GetEventAggregation() {return EventAggregation;}

    void SetChType(int ch, int type) {if (ch <= NumChannels) chtype_db[ch] = type; else std::cout << " Wrong channel number" << std::endl;}
    int GetChType(int ch) {if (ch <= NumChannels) return chtype_db[ch]; else return ReturnZeroCh(chtype_db);}
    std::vector<int> &GetChTypeVec() {return chtype_db;}

    void SetChThreshold(int ch, int trsh) {if (ch <= NumChannels) thresh_db[ch] = trsh; else std::cout << " Wrong channel number" << std::endl;}
    int GetChThreshold(int ch) {if (ch <= NumChannels) return thresh_db[ch]; else return ReturnZeroCh(chtype_db);}

    void SetChDCoffset(int ch, int offset) {if (ch <= NumChannels) dcoffset_db[ch] = offset; else std::cout << " Wrong channel number" << std::endl;}
    int GetChDCoffset(int ch) {if (ch <= NumChannels) return dcoffset_db[ch]; else return ReturnZeroCh(chtype_db);}

    void SetChTrigPolarity(int ch, int pol) {if (ch <= NumChannels) trigpol_db[ch] = pol; else std::cout << " Wrong channel number" << std::endl;}
    int GetChTrigPolarity(int ch) {if (ch <= NumChannels) return trigpol_db[ch]; else return ReturnZeroCh(chtype_db);}

    void SetChIntSig(int ch, int intsig) {if (ch <= NumChannels) intsig_db[ch] = intsig; else std::cout << " Wrong channel number" << std::endl;}
    int GetChIntSig(int ch) {if (ch <= NumChannels) return intsig_db[ch]; else return ReturnZeroCh(intsig_db);}
private:
    uint32_t Nevents;
    int Actime;
    uint32_t Vpp;
    bool TrigType; //0 - OR, 1 - AND
    int WindowWidth;
    bool CreateFit = 0;
    bool ReadTemp = 0;
    int rmin; //range for charge histos
    int rmax; 
    uint32_t PostTrigger;
    uint32_t Samples;
    int NumChannels;
    uint32_t CoincidenceWindow;
    uint32_t MajorityLevel;
    int EventAggregation;
    std::vector<int> chtype_db; //0 - disabled, 1 - enabled, 2 - trigger
    std::vector<int> thresh_db;
    std::vector<int> dcoffset_db;
    std::vector<int> trigpol_db; //0 - negative, 1 - positive
    std::vector<int> intbl_db;
    std::vector<int> intsig_db;
        
    void ReadParameters2Vect(std::string str, std::vector<int> &parameter);
    int ReturnZeroCh(std::vector<int> &vec);


};