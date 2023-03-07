#include "RunManager.h"

void cmdStop();

void RunManager::Configure(Server *server, Digitizer *digitizer)
{
    this->server = server;
    this->digitizer = digitizer;

    runparameters.run_status = 1;
    runparameters.NeedToProgram = 1;
    runparameters.nevent = 0;
}

void RunManager::Run()
{
    // digitizer->Program(dconfig);
    // server->UpdateParametersField();
    while (runparameters.run_status)
    {
        gSystem->ProcessEvents();
        server->UpdateParametersField();
        digitizer->SetRunParameters(runparameters);

        // std::cout << digitizer->GetRunStatus() << std::endl;
        
        if (runparameters.NeedToProgram == 1)
        {
            digitizer->Program(dconfig);
            runparameters.NeedToProgram = 0;
        }

        while (runparameters.run_status > 1)
        {
            digitizer->ReadEvent();
            server->UpdateParametersField();
            sleep(5);
            std::cout << runparameters.nevent << " " << digitizer->GetRunStatus() << std::endl;

            if ((rconfig.Nevents != 0) && (runparameters.nevent > rconfig.Nevents)) 
            {
                std::cout << "HERE" << std::endl;
                cmdStop();
            }
            // std::cout << runparameters.PrevRateTime << "  " << rconfig.StartTime << std::endl;
            if ((rconfig.Actime != 0) && ((runparameters.PrevRateTime-rconfig.StartTime) > rconfig.Actime*1000))
            {
                std::cout << runparameters.PrevRateTime << "  " << rconfig.StartTime << std::endl;
                cmdStop();
            }
            gSystem->ProcessEvents();
        }
    }
}

void RunManager::ReadAllConfigsFromFile(std::string cfgfilename)
{
    ConfigFile config(cfgfilename);

    rconfig.Nevents = config.read<int>("nevents");
    rconfig.Actime = config.read<int>("actime");
    dconfig.Vpp = config.read<bool>("vpp");
    dconfig.TrigType = config.read<bool>("trtype");
    aconfig.WindowWidth = config.read<int>("windowwidth");
    aconfig.CreateFit = config.read<bool>("fit");
    rconfig.ReadTemp = config.read<bool>("readtemp");    
    dconfig.Samples = config.read<int>("samples");
    dconfig.PostTrigger = config.read<int>("posttrigger");
    aconfig.rmin = config.read<int>("rmin");
    aconfig.rmax = config.read<int>("rmax");
    dconfig.CoincidenceWindow = config.read<int>("coincidence_window");
    dconfig.MajorityLevel = config.read<int>("majority_level");
    dconfig.EventAggregation = config.read<int>("event_aggreagation");
    // dconfig.test = config.read<float>("test");

    RunManager::ReadParameters2Vect(config.read<string>("chtype").data(), dconfig.chtype_db);
    RunManager::ReadParameters2Vect(config.read<string>("thresh").data(), dconfig.thresh_db);
    RunManager::ReadParameters2Vect(config.read<string>("dcoffset").data(), dconfig.dcoffset_db);
    RunManager::ReadParameters2Vect(config.read<string>("intsig").data(), aconfig.intsig_db);
    for (auto i : aconfig.intsig_db) aconfig.intbl_db.push_back(i-aconfig.WindowWidth);
    RunManager::ReadParameters2Vect(config.read<string>("polarity").data(), dconfig.trigpol_db);

    // RunManager::ReadParameters2Vect(config.read<string>("test").data(), dconfig.test_db);
    // for (auto i : dconfig.chtype_db) std::cout << i << std::endl;
}

template <typename T>
void RunManager::ReadParameters2Vect(std::string str, std::vector<T> &parameter)
{
    str +=",";
    std::string num;
    for (auto i = 0; i < int(str.length()); i++)
    {
        if (str[i] != ',') num.append(1, str[i]);
        else
        {
            if (typeid(T) == typeid(int)) parameter.push_back(std::stoi(num));
            if (typeid(T) == typeid(float)) parameter.push_back(std::stof(num));
            num.clear();
        }
    }
    dconfig.NumChannels = parameter.size();
}

template <typename T>
int RunManager::ReturnZeroCh(std::vector<T> &vec)
{
    std::cout << "Wrong number of channel, it returned zero channel instead" << std::endl;
    return vec[0];
}