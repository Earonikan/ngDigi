#include "RunManager.h"

void RunManager::Configure(Server *server, Digitizer *digitizer)
{
    this->server = server;
    this->digitizer = digitizer;
    // this->config = config;

    // server->Connect2RunManager(this);
}

void RunManager::Run()
{
    runparameters.run_status = 1;
    digitizer->Program(dconfig);
    server->UpdateParametersField(0);
    while (runparameters.run_status)
    {
        int nevent = 0;//HERE!!!!!!!!!!!!
        while (runparameters.run_status > 1)
        {
    //         readdigitizer();
            digitizer->ReadEvent(nevent);
            // nevent++;
            server->UpdateParametersField(nevent);
    //         if ((Nevents!=0)&&(nevent>Nevents)) cmdStop();
    //         if((Actime!=0)&&((CurrentTime-StartTime)>Actime*1000)) {cout<<CurrentTime<<"  "<<StartTime<<endl;cmdStop();}
            gSystem->ProcessEvents();
        }
    //     server.ProcessRequests();
        gSystem->ProcessEvents();
    }
}

void RunManager::ReadAllConfigsFromFile(std::string cfgfilename)
{
    ConfigFile config(cfgfilename);

    runparameters.Nevents = config.read<int>("nevents");
    runparameters.Actime = config.read<int>("actime");
    dconfig.Vpp = config.read<bool>("vpp");
    dconfig.TrigType = config.read<bool>("trtype");
    aconfig.WindowWidth = config.read<int>("windowwidth");
    aconfig.CreateFit = config.read<bool>("fit");
    runparameters.ReadTemp = config.read<bool>("readtemp");    
    dconfig.Samples = config.read<int>("samples");
    dconfig.PostTrigger = config.read<int>("posttrigger");
    aconfig.rmin = config.read<int>("rmin");
    aconfig.rmax = config.read<int>("rmax");
    dconfig.CoincidenceWindow = config.read<int>("coincidence_window");
    dconfig.MajorityLevel = config.read<int>("majority_level");
    dconfig.EventAggregation = config.read<int>("event_aggreagation");

    RunManager::ReadParameters2Vect(config.read<string>("chtype").data(), dconfig.chtype_db);
    RunManager::ReadParameters2Vect(config.read<string>("thresh").data(), dconfig.thresh_db);
    RunManager::ReadParameters2Vect(config.read<string>("dcoffset").data(), dconfig.dcoffset_db);
    RunManager::ReadParameters2Vect(config.read<string>("intsig").data(), aconfig.intsig_db);
    for (auto i : aconfig.intsig_db) aconfig.intbl_db.push_back(i-aconfig.WindowWidth);
    RunManager::ReadParameters2Vect(config.read<string>("polarity").data(), dconfig.trigpol_db);

    // for (auto i : intbl) std::cout << i << std::endl;
}

void RunManager::ReadParameters2Vect(std::string str, std::vector<int> &parameter)
{
    str +=",";
    std::string num;
    for (auto i = 0; i < int(str.length()); i++)
    {
        if (str[i] != ',') num.append(1, str[i]);
        else
        {
            parameter.push_back(std::stoi(num));
            num.clear();
        }
    }
    runparameters.NumChannels = parameter.size();
}

int RunManager::ReturnZeroCh(std::vector<int> &vec)
{
    std::cout << "Wrong number of channel, it returned zero channel instead" << std::endl;
    return vec[0];
}

void SetCurrentTime()
{
    struct timeval t1;
    gettimeofday(&t1, NULL);
    runparameters.CurrentTime = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
}