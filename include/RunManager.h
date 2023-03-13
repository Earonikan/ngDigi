#pragma once

#include "stdafx.h"
#include "ConfigFile.h"
#include "Server.h"
#include "Digitizer.h"
#include "DataFrames.h"
#include "DataManager.h"

// class Config;
class Digitizer;
class Server; 

// template <typename T>
class RunManager
{
public:
    RunManager(std::string cfgfilename); //read all configs from file
    ~RunManager();
    void Configure(); //configure 
    void Run();
    
    void StartRun();
    void StopRun();

    void SetStatus(int status) {runparameters_.run_status = status;}
    int GetStatus() {return runparameters_.run_status;}

    RunParameters GetRunParameters() {return runparameters_;}
    Dconfig GetDconfig() {return dconfig_;}
    Aconfig GetAconfig() {return aconfig_;}
    Rconfig GetRconfig() {return rconfig_;}
    DigiData GetDigiData() {return digidata_;}

    void SetParameter(int arg1, const char *ch);
    void SetVecParameter(int arg1, int arg2, const char *ch);
    void SetCharParameters(const char *arg1, const char *ch);

    void SetRunParameters(RunParameters runparameters) {runparameters_ = runparameters;}
    void GetDconfig(Dconfig dconfig) {dconfig_ = dconfig;}
    void GetAconfig(Aconfig aconfig) {aconfig_ = aconfig;}
    void GetRconfig(Rconfig rconfig) {rconfig_ = rconfig;}
    void GetDigiData(DigiData digidata) {digidata_ = digidata;}

    Digitizer *GetDigitizer() {return digitizer;}
    Server *GetServer() {return server;}
    
private:
    
    Server *server;
    Digitizer *digitizer;
    DataManager *datamanager;

    RunParameters runparameters_;
    Dconfig dconfig_;
    Aconfig aconfig_;
    Rconfig rconfig_;
    DigiData digidata_;

    //helpers
    template <typename T>
    void ReadParameters2Vect(std::string str, std::vector<T> &parameter);
    // template <typename T>
    // int ReturnZeroCh(std::vector<T> &vec);
    
};