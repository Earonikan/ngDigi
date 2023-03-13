#include "RunManager.h"

void cmdStop();

RunManager::RunManager(std::string cfgfilename)
{
    ConfigFile config(cfgfilename);

    rconfig_.Nevents = config.read<int>("nevents");
    rconfig_.Actime = config.read<int>("actime");
    dconfig_.Vpp = config.read<bool>("vpp");
    dconfig_.TrigType = config.read<bool>("trtype");
    aconfig_.WindowWidth = config.read<int>("windowwidth");
    aconfig_.CreateFit = config.read<bool>("fit");
    rconfig_.ReadTemp = config.read<bool>("readtemp");    
    dconfig_.Samples = config.read<int>("samples");
    dconfig_.PostTrigger = config.read<int>("posttrigger");
    aconfig_.rmin = config.read<int>("rmin");
    aconfig_.rmax = config.read<int>("rmax");
    dconfig_.CoincidenceWindow = config.read<int>("coincidence_window");
    dconfig_.MajorityLevel = config.read<int>("majority_level");
    dconfig_.EventAggregation = config.read<int>("event_aggreagation");
    // dconfig.test = config.read<float>("test");

    ReadParameters2Vect(config.read<string>("chtype").data(), dconfig_.chtype_db);
    ReadParameters2Vect(config.read<string>("thresh").data(), dconfig_.thresh_db);
    ReadParameters2Vect(config.read<string>("dcoffset").data(), dconfig_.dcoffset_db);
    ReadParameters2Vect(config.read<string>("intsig").data(), aconfig_.intsig_db);
    for (auto i : aconfig_.intsig_db) aconfig_.intbl_db.push_back(i-aconfig_.WindowWidth);
    ReadParameters2Vect(config.read<string>("polarity").data(), dconfig_.trigpol_db);

    // RunManager::ReadParameters2Vect(config.read<string>("test").data(), dconfig.test_db);
    // for (auto i : dconfig.chtype_db) std::cout << i << std::endl;
}

RunManager::~RunManager()
{
    delete digitizer;
    delete datamanager;
    delete server;
}

void RunManager::Configure()
{
    server = new Server();
    digitizer = new Digitizer();
    datamanager = new DataManager();

    runparameters_.nevent = 0;
    runparameters_.Nbytes = 0;
    runparameters_.Nevs = 0;
    runparameters_.nCycles = 0;
    runparameters_.handle = digitizer->GetHandle();
    runparameters_.run_status = 1;
    runparameters_.PrevRateTime = GetCurrentTime();
    runparameters_.NeedToProgram = 1;
}

void RunManager::Run()
{
    while (runparameters_.run_status)
    {
        gSystem->ProcessEvents();
        server->UpdateParametersField(runparameters_, dconfig_, aconfig_, rconfig_, digidata_);
        digitizer->SetRunParameters(runparameters_, dconfig_);
        datamanager->Update(aconfig_);

        if (runparameters_.NeedToProgram == 1)
        {
            digitizer->Program();
            runparameters_.NeedToProgram = 0;
        }

        while (runparameters_.run_status > 1)
        {
            digitizer->SetRunParameters(runparameters_, dconfig_);
            runparameters_ = digitizer->ReadEvent(digidata_);
            datamanager->AnalyzeData(digidata_);
            server->UpdateParametersField(runparameters_, dconfig_, aconfig_, rconfig_, digidata_);

            if ((rconfig_.Nevents != 0) && (runparameters_.nevent > rconfig_.Nevents)) 
            {
                cmdStop();
                std::cout << "Events completed" << std::endl;
            }
            if ((rconfig_.Actime != 0) && ((runparameters_.PrevRateTime-rconfig_.StartTime) > rconfig_.Actime*1000))
            {
                cmdStop();
                std::cout << "Time elapsed" << std::endl;
            }
            sleep(1);
            gSystem->ProcessEvents();
        }
    }
}

void RunManager::SetParameter(int arg1, const char *ch)
{
    if (strcmp("num_evs", ch) == 0) rconfig_.Nevents = arg1;
    if (strcmp("num_samps", ch) == 0) dconfig_.Samples = arg1;
    if (strcmp("posttrigger", ch) == 0) dconfig_.PostTrigger = arg1;
    if (strcmp("charge_width", ch) == 0) aconfig_.WindowWidth = arg1;
    if (strcmp("vpp", ch) == 0) dconfig_.Vpp = arg1;
    if (strcmp("run_time", ch) == 0) rconfig_.Actime = arg1;
    if (strcmp("read_temps", ch) == 0) rconfig_.ReadTemp = arg1;
    if (strcmp("create_fit", ch) == 0) aconfig_.CreateFit = arg1;

    if (strcmp("run_status", ch) == 0) runparameters_.run_status = arg1;
    if (strcmp("need_to_program", ch) == 0) runparameters_.NeedToProgram = arg1;
    if (strcmp("start_time", ch) == 0) rconfig_.StartTime = arg1;
    if (strcmp("prev_rate_time", ch) == 0) runparameters_.PrevRateTime = arg1;

    if (strcmp("Nbytes", ch) == 0) runparameters_.Nbytes = arg1;
    if (strcmp("Nevs", ch) == 0) runparameters_.Nevs = arg1;
    if (strcmp("nCycles", ch) == 0) runparameters_.nCycles = arg1;
    if (strcmp("nevts", ch) == 0) runparameters_.nevent = arg1;

    // if (strcmp("Nbytes", ch) == 0) runparameters_.Nbytes = arg1;
    // if (strcmp("Nbytes", ch) == 0) runparameters_.Nbytes = arg1;

    server->UpdateParametersField(runparameters_, dconfig_, aconfig_, rconfig_, digidata_);
}

void RunManager::SetVecParameter(int arg1, int arg2, const char *ch)
{
    if (strcmp("thrs", ch) == 0) 
    {
        dconfig_.thresh_db[arg1] = arg2;
    }
    if (strcmp("dcoff", ch) == 0) dconfig_.dcoffset_db[arg1] = arg2;
    if (strcmp("change_window", ch) == 0)
    {
        aconfig_.rmin = arg1;
        aconfig_.rmax = arg2;
    }
    if (strcmp("change_start", ch) == 0)
    {
        aconfig_.intsig_db[arg1] = arg2;
        aconfig_.intbl_db[arg1] = aconfig_.intsig_db[arg1] - aconfig_.WindowWidth;
    }
    server->UpdateParametersField(runparameters_, dconfig_, aconfig_, rconfig_, digidata_);
}

void RunManager::SetCharParameters(const char *arg1, const char *ch)
{
    if (strcmp("chtype", ch) == 0)
    {
        dconfig_.chtype_db.clear();
        ReadParameters2Vect(arg1, dconfig_.chtype_db);
    }
    server->UpdateParametersField(runparameters_, dconfig_, aconfig_, rconfig_, digidata_);
}

template <typename T>
void RunManager::ReadParameters2Vect(std::string str, std::vector<T> &parameter)
{
    // std::cout << parameter.size() << std::endl;
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
    dconfig_.NumChannels = parameter.size();
    // std::cout << dconfig_.NumChannels << std::endl;
}

void RunManager::StartRun()
{
    SetParameter(2, "run_status");
    SetParameter(1, "need_to_program");
    rconfig_.StartTime = GetCurrentTime();
    runparameters_.PrevRateTime = rconfig_.StartTime;
    // SetParameter(GetCurrentTime(), "start_time");
    // SetParameter(rconfig_.StartTime, "prev_rate_time");
    SetParameter(0, "Nbytes");
    SetParameter(0, "Nevs");
    SetParameter(0, "nCycles");
    SetParameter(0, "nevts");

    server->UpdateParametersField(runparameters_, dconfig_, aconfig_, rconfig_, digidata_);
    // SetParameter(0, "create_fit");

    // resethistos(); //HISTOS
    digitizer->AllocateEvents();
    sleep(2);
    digitizer->StartAquisition();

    
    // if (readtemp!=0)
    // {
    //     wget(temp0, "http://minitrs01.cern.ch", "/", 80);
    //     serv->SetItemField("/Temperatures", "value", temp0);
    // }
}

void RunManager::StopRun()
{
    SetParameter(1, "run_status");
    SetParameter(0, "need_to_program");
    SetParameter(0, "Nbytes");
    SetParameter(0, "Nevs");
    SetParameter(0, "nCycles");
    SetParameter(0, "nevts");

    server->UpdateParametersField(runparameters_, dconfig_, aconfig_, rconfig_, digidata_);

    digitizer->StopAquisition();
        // serv->SetItemField("/Status", "value", "Stop");
    // serv->SetItemField("/Status", "_status", "0");
    // if (readtemp!=0)
    // {
    //     wget(temp1, "http://minitrs01.cern.ch", "/", 80);
    //     serv->SetItemField("/Temperatures", "value", temp0+"\n"+temp1);
    // }
}

// template <typename T>
// int RunManager::ReturnZeroCh(std::vector<T> &vec)
// {
//     std::cout << "Wrong number of channel, it returned zero channel instead" << std::endl;
//     return vec[0];
// }