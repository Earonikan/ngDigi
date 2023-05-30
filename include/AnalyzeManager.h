#pragma once

#include "stdafx.h"
#include "DataFrames.h"

class AnalyzeManager
{
public:
    AnalyzeManager();
    ~AnalyzeManager();
    void CreateHistos(Aconfig aconfig, Dconfig dconfig, CAEN_DGTZ_BoardInfo_t BoardInfo);
    HistoCollection GetHistoCollection() {return histocollection_;}
    // void SetData(DigiData digidata_);
    void AnalyzeData();
    void DeleteHistos();
private:
    Aconfig aconfig_;
    Dconfig dconfig_;
    DigiData digidata_;
    HistoCollection histocollection_;
    CAEN_DGTZ_BoardInfo_t BoardInfo_;
};