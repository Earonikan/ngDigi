#pragma once

#include "stdafx.h"
#include "ConfigFile.h"
#include "Server.h"
#include "Digitizer.h"
#include "DataFrames.h"

// class Config;
class Digitizer;
class Server; 

class RunManager
{
public:
    RunManager() {;} //ini run parameters
    void Configure(Server *server, Digitizer *digitizer); //configure 
    void Run();
    void ReadAllConfigsFromFile(std::string cfgfilename);

    void SetStatus(int status) {runparameters.run_status = status;}
    int GetStatus() {return runparameters.run_status;}

    void SetCurrentTime();//

    Dconfig &GetDconfig() {return dconfig;}
private:
    
    Server *server;
    Digitizer *digitizer;
    // Config *config;

    RunParameters runparameters;
    Dconfig dconfig;
    Aconfig aconfig;
    DigiData digidata;

    //helpers
    void ReadParameters2Vect(std::string str, std::vector<int> &parameter);
    int ReturnZeroCh(std::vector<int> &vec);
    
};