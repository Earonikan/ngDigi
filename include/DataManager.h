#pragma once

#include "stdafx.h"
#include "DataFrames.h"

class DataManager
{
public:
    DataManager();
    void Update(Aconfig aconfig);
    void AnalyzeData(DigiData digidata_);
private:
    Aconfig aconfig_;
    DigiData digidata_;
};