#pragma once

#include "stdafx.h"
#include "RunManager.h"
#include "DataFrames.h"

// class RunManager;

class Server
{
public:
    Server();
    ~Server();

    void UpdateParametersField(int nevent);
    // void Connect2RunManager(RunManager *rman) {runmanager = rman;}

    friend void cmdStart();
    friend void cmdStop();
    friend void ChangeParameter(int arg1, int arg2, const char *ch);
    // void ProcessRequests() {serv->ProcessRequest();}
private:
    THttpServer *serv;
    RunParameters runparameters;
	DigiData digidata;
    // RunManager *runmanager;
    const char *str; // text field about current parameters
};