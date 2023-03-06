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

    void SetStatus(int status) {rconfig.run_status = status;}
    int GetStatus() {return rconfig.run_status;}

    Dconfig &GetDconfig() {return dconfig;}
    Aconfig &GetAconfig() {return aconfig;}
    RunParameters &GetRunParameters() {return runparameters;}
    Rconfig &GetRconfig() {return rconfig;}

    Digitizer *GetDigitizer() {return digitizer;}
    
private:
    
    Server *server;
    Digitizer *digitizer;

    RunParameters runparameters;
    Dconfig dconfig;
    Aconfig aconfig;
    Rconfig rconfig;
    DigiData digidata;

    //helpers
    void ReadParameters2Vect(std::string str, std::vector<int> &parameter);
    int ReturnZeroCh(std::vector<int> &vec);
    
};