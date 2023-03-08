#include "Server.h"

// extern RunManager runmanager;

Server::Server()
{
    serv = new THttpServer("http:9000;rw");
    serv->SetItemField("/Status", "value", "Stop");
    serv->SetItemField("/Status", "_status", "0");

    gInterpreter->Declare("void cmdStart();"); 
    gInterpreter->Declare("void cmdStop();");
    gInterpreter->Declare("void ChangeParameterOne(int arg1, const char *ch);");
    gInterpreter->Declare("void ChangeParameterTwo(int arg1, int arg2, const char *ch);");

    // gInterpreter->Declare("int programdigitizer();");
    // gInterpreter->Declare("void resethistos();");
    // gInterpreter->Declare("void savefile(TString arg1);");
    // gInterpreter->Declare("void chcreatedir(TString arg1);");
    
    serv->RegisterCommand("/commands/Start", "cmdStart();", "button;rootsys/icons/ed_execute.png");
    serv->RegisterCommand("/commands/Stop", "cmdStop();", "button;rootsys/icons/ed_interrupt.png");
    serv->RegisterCommand("/commands/ReadTemperatures", "ChangeParameterOne(%arg1%,\"read_temps\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/CreateFitAfterRun", "ChangeParameterOne(%arg1%,\"create_fit\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/RunTime", "ChangeParameterOne(%arg1%,\"run_time\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_number_of_events", "ChangeParameterOne(%arg1%,\"num_evs\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_number_of_samples", "ChangeParameterOne(%arg1%,\"num_samps\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_posttrigger", "ChangeParameterOne(%arg1%,\"posttrigger\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_thresholds", "ChangeParameterTwo(%arg1%,%arg2%,\"thrs\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_dcoffset", "ChangeParameterTwo(%arg1%,%arg2%,\"dcoff\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_window", "ChangeParameterTwo(%arg1%,%arg2%,\"change_window\")", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_integration_width", "ChangeParameterOne(%arg1%,\"charge_width\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_integration_starting_point", "ChangeParameterTwo(%arg1%,%arg2%,\"charge_start\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_Vpp", "ChangeParameterOne(%arg1%,\"vpp\");", "rootsys/icons/right_arrow_cursor.png");

    // serv->RegisterCommand("/commands/Program_digitizer", "programdigitizer();", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/Reset_histos", "resethistos();", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/Fit_histos", "fithistos();", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/Save2file", "savefile(\"%arg1%\");", "button;rootsys/icons/bld_save.png");
    // serv->RegisterCommand("/commands/createdir", "chcreatedir(\"%arg1%\");", "rootsys/icons/bld_save.png");

    serv->CreateItem("/Parameters","parameters output");
    serv->SetItemField("/Parameters", "_kind", "Text");
    serv->CreateItem("/Status","status output");
    serv->SetItemField("/Status", "_kind", "Text");
    serv->SetItemField("/Status", "_size", "24");
    serv->SetItemField("/Status", "_font", "24");
    serv->SetItemField("/Status", "value", "0");
    serv->CreateItem("/Fits","fits output");
    serv->SetItemField("/Fits", "_kind", "Text");
    serv->CreateItem("/Temperatures","Temperatures output");
    serv->SetItemField("/Temperatures", "_kind", "Text");

    serv->SetItemField("/", "_layout", "vert2222_8441");
    serv->SetItemField("/","_monitoring","10");
    //sprintf(sname,"[trigger_ch%d/common_spectr_%d,trigger_ch%d/common_spectr_%d,trigger_ch%d/pave_%d,trigger_ch%d/pave_%d,Parameters,trigger_ch%d/bl_spectr_%dtrigger_ch%d/signalwf_%d+trigger_ch%d/TBox,trigger_ch%d/signalwf_%d+trigger_ch%d/TBox]",chtr[0],chtr[0],chtr[1],chtr[1],chtr[0],chtr[0],chtr[1],chtr[1],chtr[0],chtr[0],chtr[0],chtr[1],chtr[1],chtr[1]);
    // sprintf(sname,"[trigwf_%d]",chtr[0]);
    // char sname[1000]
    // sprintf(sname,"[trigger_ch%d/common_spectr_%d,trigger_ch%d/common_spectr_%d,Parameters,trigger_ch%d/bl_spectr_%d+trigger_ch%d/bl_spectr_%d+trigger_ch%d/signal_spectr_%d+trigger_ch%d/signal_spectr_%d,trigger_ch%d/signalwf_%d+trigger_ch%d/TBox,trigger_ch%d/signalwf_%d+trigger_ch%d/TBox,Status,Temperatures]",chtr[0],chtr[0],chtr[1],chtr[1],chtr[0],chtr[0],chtr[1],chtr[1],chtr[0],chtr[0],chtr[1],chtr[1],chtr[0],chtr[0],chtr[0],chtr[1],chtr[1],chtr[1]);
    // printf("%s\n",sname);
    // serv->SetItemField("/","_drawitem",sname);
}

Server::~Server()
{
    std::cout << "Deleting Server!" << std::endl;
    delete serv;
}

void ChangeParameterOne(int arg1, const char *ch)
{
    GlobalWrapper<RunManager>::GetInstance().Getter()->SetParameter(arg1, ch);
}

void ChangeParameterTwo(int arg1, int arg2, const char *ch)
{
    GlobalWrapper<RunManager>::GetInstance().Getter()->SetVecParameter(arg1, arg2, ch);
}

void cmdStart()
{
    GlobalWrapper<RunManager>::GetInstance().Getter()->StartRun();
    std::cout << "cmdStart()" << std::endl;
}

void cmdStop()
{
    GlobalWrapper<RunManager>::GetInstance().Getter()->StopRun();
    std::cout << "cmdStop()" << std::endl;
}

void Server::UpdateParametersField(RunParameters runparameters, Dconfig dconfig, Aconfig aconfig, Rconfig rconfig, DigiData digidata)
{
    runparameters_ = runparameters;
    dconfig_ = dconfig;
    rconfig_ = rconfig;
    aconfig_ = aconfig;
    digidata_ = digidata;

    std::string temp_string;
    char temp_char[200];

    if (runparameters_.run_status == 0)
    {
        serv->SetItemField("/Status", "value", "Stop");
        serv->SetItemField("/Status", "_status", "0"); 
    }
    if (runparameters_.run_status == 2)
    {
        sprintf(temp_char,"Running... \n%d / %d\n", runparameters_.nevent, rconfig_.Nevents);
        serv->SetItemField("/Status", "value", temp_char);
        serv->SetItemField("/Status", "_status", "1");
    }
    if (runparameters_.run_status == 1)
    {
        sprintf(temp_char,"Idling... \n%d / %d\n", runparameters_.nevent, rconfig_.Nevents);
        serv->SetItemField("/Status", "value", temp_char);
        serv->SetItemField("/Status", "_status", "1");
    }
    
    serv->SetItemField("/Fits", "value", "Undefined");

    sprintf(temp_char, "Run will stop after %d events or %d seconds", rconfig_.Nevents, rconfig_.Actime);
    temp_string += temp_char;
    sprintf(temp_char, "\nNumber of samples = %d", dconfig_.Samples);
    temp_string += temp_char;
    sprintf(temp_char, "\nPostTrigger = %d ", dconfig_.PostTrigger);
    temp_string += temp_char;
    sprintf(temp_char, "\nTrigger channels: ");
    temp_string += temp_char;
    for (int i = 0; i < dconfig_.NumChannels; i++) if (dconfig_.chtype_db[i] == 2)
    {
        sprintf(temp_char,"%d ", i);
        temp_string += temp_char;
    }
    sprintf(temp_char, "\n");
    temp_string += temp_char;
    for (int i = 0; i < dconfig_.NumChannels; i++) if (dconfig_.chtype_db[i] == 2)
    {
        sprintf(temp_char, "Channel %d threshold = %d\n", i, dconfig_.thresh_db[i]);
        temp_string += temp_char;
        
        // std::cout << "HERE" << std::endl;
    }
    sprintf(temp_char, "\n");
    for (int i = 0; i < dconfig_.NumChannels; i++) if (dconfig_.chtype_db[i] > 0)
    {
        sprintf(temp_char, "Channel %d DCoffset = %d\n", i, dconfig_.dcoffset_db[i]);
        temp_string += temp_char;
    }
    sprintf (temp_char,"\nIntegration window width = %d\n", aconfig_.WindowWidth);
    temp_string += temp_char;
    for (int i = 0; i < dconfig_.NumChannels; i++) if (dconfig_.chtype_db[i] > 0)
    {
        sprintf(temp_char, "Channel %d trigger window = (%d, %d)\n", i, aconfig_.intsig_db[i], aconfig_.intbl_db[i] - aconfig_.WindowWidth);
        temp_string += temp_char;
    }
    sprintf (temp_char,"Charge Histos Range = (%d, %d)\n", aconfig_.rmin, aconfig_.rmax);
    temp_string += temp_char;
    sprintf (temp_char,"Vpp = %d\n", dconfig_.Vpp);
    temp_string += temp_char;
    sprintf (temp_char,"CreateFit = %d\n", aconfig_.CreateFit);
    temp_string += temp_char;
    sprintf (temp_char,"Read Temperatures = %d\n", rconfig_.ReadTemp);
    temp_string += temp_char;
    str = temp_string.c_str();
    // str = "Hello";
    serv->SetItemField("/Parameters", "value", str);
    
}