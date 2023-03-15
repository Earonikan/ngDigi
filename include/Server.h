#pragma once

#include "stdafx.h"
#include "RunManager.h"
#include "AnalyzeManager.h"
#include "DataFrames.h"
#include "GlobalWrapper.h"
#include "utils.h"

// class RunManager;

class Server
{
public:
    Server();
    ~Server();

    void UpdateParametersField(RunParameters runparameters, Dconfig dconfig, Aconfig aconfig, Rconfig rconfig, DigiData digidata, HistoCollection histocollection);
    // void Connect2RunManager(RunManager *rman) {runmanager = rman;}

    friend void cmdStart();
    friend void cmdStop();
    // friend void cmdUpdate();
    friend void ChangeParameterOne(int arg1, const char *ch);
    friend void ChangeParameterTwo(int arg1, int arg2, const char *ch);
    
    // void ProcessRequests() {serv->ProcessRequest();}
private:
    THttpServer *serv;
    // RunParameters runparameters;
    RunParameters runparameters_;
    Dconfig dconfig_;
    Aconfig aconfig_;
    Rconfig rconfig_;
    DigiData digidata_;
    HistoCollection histocollection_;
    const char *str; // text field about current parameters
};