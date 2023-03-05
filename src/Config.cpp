#include "Config.h"

void Config::ReadAllConfigsFromFile(std::string cfgfilename)
{
    ConfigFile config(cfgfilename);

    Nevents = config.read<int>("nevents");
    Actime = config.read<int>("actime");
    Vpp = config.read<int>("vpp");
    TrigType = config.read<bool>("trtype");
    WindowWidth = config.read<int>("windowwidth");
    CreateFit = config.read<bool>("fit");
    ReadTemp = config.read<bool>("readtemp");    
    Samples = config.read<int>("samples");
    PostTrigger = config.read<int>("posttrigger");
    rmin = config.read<int>("rmin");
    rmax = config.read<int>("rmax");
    CoincidenceWindow = config.read<int>("coincidence_window");
    MajorityLevel = config.read<int>("majority_level");
    EventAggregation = config.read<int>("event_aggreagation");

    Config::ReadParameters2Vect(config.read<string>("chtype").data(), chtype_db);
    Config::ReadParameters2Vect(config.read<string>("thresh").data(), thresh_db);
    Config::ReadParameters2Vect(config.read<string>("dcoffset").data(), dcoffset_db);
    Config::ReadParameters2Vect(config.read<string>("intsig").data(), intsig_db);
    for (auto i : intsig_db) intbl_db.push_back(i-WindowWidth);
    Config::ReadParameters2Vect(config.read<string>("polarity").data(), trigpol_db);

    // for (auto i : intbl) std::cout << i << std::endl;
}

void Config::ReadParameters2Vect(std::string str, std::vector<int> &parameter)
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
    NumChannels = parameter.size();
}

int Config::ReturnZeroCh(std::vector<int> &vec)
{
    std::cout << "Wrong number of channel, it returned zero channel instead" << std::endl;
    return vec[0];
}