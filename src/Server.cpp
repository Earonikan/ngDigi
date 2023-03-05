#include "Server.h"

extern RunManager runmanager;

Server::Server()
{
    serv = new THttpServer("http:9000;rw");
    serv->SetItemField("/Status", "value","Stop");
    serv->SetItemField("/Status", "_status", "0");

    gInterpreter->Declare("void cmdStart();"); 
    gInterpreter->Declare("void cmdStop();");
    gInterpreter->Declare("void ChangeParameter(int arg1, int arg2, const char *ch);");
    // gInterpreter->Declare("void chActime(int arg);");
    // gInterpreter->Declare("void chfithistos(int arg);");
    // gInterpreter->Declare("void chreadtemperatures(int arg);");
    // gInterpreter->Declare("void chsamples(int arg1);");
    // gInterpreter->Declare("void chposttrigger(int arg);");
    // gInterpreter->Declare("void chthreshold(int arg1,int arg2);");
    // gInterpreter->Declare("void chdcoffset(int arg1,int arg2);");
    // gInterpreter->Declare("void chwindow(int arg1,int arg2);");
    // gInterpreter->Declare("void chintwindow(int arg1);");
    // gInterpreter->Declare("void chintsig(int arg1,int arg2);");
    // gInterpreter->Declare("void chdcoffset2(int arg1);");
    // gInterpreter->Declare("int programdigitizer();");
    // gInterpreter->Declare("void resethistos();");
    // gInterpreter->Declare("void fithistos();");
    // gInterpreter->Declare("void chvpp(int arg1);");
    // gInterpreter->Declare("void savefile(TString arg1);");
    // gInterpreter->Declare("void chcreatedir(TString arg1);");
    // gInterpreter->Declare("void testsignal(int arg1, int arg2);");
    
    serv->RegisterCommand("/commands/Start", "cmdStart();", "button;rootsys/icons/ed_execute.png");
    serv->RegisterCommand("/commands/Stop", "cmdStop();", "button;rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/FitAfterRun", "chfithistos(%arg1%);", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/ReadTemperatures", "chreadtemperatures(%arg1%);", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/RunTime", "chActime(%arg1%);", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/Program_digitizer", "programdigitizer();", "rootsys/icons/ed_interrupt.png");
    serv->RegisterCommand("/commands/Change_number_of_events", "ChangeParameter(%arg1%,%arg2%,\"num_evs\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_number_of_samples", "ChangeParameter(%arg1%,%arg2%,\"num_samps\");", "rootsys/icons/right_arrow_cursor.png");
    // serv->RegisterCommand("/commands/Change_samples", "chsamples(%arg1%);cout<<%arg1%<<endl;", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/Change_posttrigger", "chposttrigger(%arg1%);cout<<%arg1%<<endl;", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/Change_thresholds", "chthreshold(%arg1%,%arg2%);", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/Change_dcoffset_trigger", "chdcoffset(%arg1%,%arg2%);", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/Change_dcoffset_signal", "chdcoffset2(%arg1%);", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/Change_window", "chwindow(%arg1%,%arg2%);", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/Change_integration_width", "chintwindow(%arg1%);", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/Change_integration_starting_point", "chintsig(%arg1%,%arg2%);", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/Reset_histos", "resethistos();", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/Fit_histos", "fithistos();", "rootsys/icons/ed_interrupt.png");
    // serv->RegisterCommand("/commands/chvpp", "chvpp(%arg1%);", "rootsys/icons/ed_interrupt.png");
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
}

Server::~Server()
{
    delete serv;
}

void cmdStart()
{
    runmanager.SetStatus(2);
    
    PrevRateTime = util::get_time();
    StartTime=PrevRateTime;
    Nb=0;
    Ne=0;
    nCycles= 0;
    nevent=0;
    resethistos();
    programdigitizer();
    allocateevents();

    sleep(2);
    CAEN_DGTZ_SWStartAcquisition(handle);
    onrun=kTRUE;
    runfitted=false;
    std::cout<<"cmdStart()"<<std::endl;
    if (readtemp!=0)
    {
        wget(temp0, "http://minitrs01.cern.ch", "/", 80);
        serv->SetItemField("/Temperatures", "value", temp0);
    }
}

void cmdStop()
{
    runmanager.SetStatus(1);
}

void ChangeParameter(int arg1, int arg2, const char *ch)
{
    if (strcmp("num_evs", ch) == 0) runmanager.GetConfig()->SetNevents(arg1);
    if (strcmp("num_samps", ch) == 0) runmanager.GetConfig()->SetNsamples(arg1);
}

void Server::UpdateParametersField(int nevent)
{
    std::string temp_string;
    char temp_char[200];

    sprintf(temp_char,"Running \n%d / %d\n", nevent, runmanager.GetConfig()->GetNevents());
    serv->SetItemField("/Status", "value", temp_char);
    serv->SetItemField("/Status", "_status", "1");	 
    serv->SetItemField("/Fits", "value", "Undefined");

    sprintf(temp_char, "Run will stop after %d events or %d seconds", runmanager.GetConfig()->GetNevents(), runmanager.GetConfig()->GetActime());
    temp_string += temp_char;
    sprintf(temp_char, "\nNumber of samples = %d", runmanager.GetConfig()->GetNsamples());
    temp_string += temp_char;
    sprintf(temp_char, "\nPostTrigger = %d ", runmanager.GetConfig()->GetPostTrigger());
    temp_string += temp_char;
    sprintf(temp_char, "\nTrigger channels: ");
    temp_string += temp_char;
    for (int i = 0; i < runmanager.GetConfig()->GetNumChannels(); i++) if (runmanager.GetConfig()->GetChType(i) == 2)
    {
        sprintf(temp_char,"%d ", i);
        temp_string += temp_char;
    }
    sprintf(temp_char, "\n");
    temp_string += temp_char;
    for (int i = 0; i < runmanager.GetConfig()->GetNumChannels(); i++) if (runmanager.GetConfig()->GetChType(i) == 2)
    {
        sprintf(temp_char, "Channel %d threshold = %d\n", i, runmanager.GetConfig()->GetChThreshold(i));
        temp_string += temp_char;
    }
    sprintf(temp_char, "\n");
    for (int i = 0; i < runmanager.GetConfig()->GetNumChannels(); i++) if (runmanager.GetConfig()->GetChType(i) > 0)
    {
        sprintf(temp_char, "Channel %d DCoffset = %d\n", i, runmanager.GetConfig()->GetChDCoffset(i));
        temp_string += temp_char;
    }
    sprintf (temp_char,"\nIntegration window width = %d\n", runmanager.GetConfig()->GetWindowWidth());
    temp_string += temp_char;
    for (int i = 0; i < runmanager.GetConfig()->GetNumChannels(); i++) if (runmanager.GetConfig()->GetChType(i) > 0)
    {
        sprintf(temp_char, "Channel %d trigger window = (%d, %d)\n", i, runmanager.GetConfig()->GetChIntSig(i), runmanager.GetConfig()->GetChIntSig(i) - runmanager.GetConfig()->GetWindowWidth());
        temp_string += temp_char;
    }
    sprintf (temp_char,"Charge Histos Range = (%d, %d)\n", runmanager.GetConfig()->GetRmin(), runmanager.GetConfig()->GetRmax());
    temp_string += temp_char;
    sprintf (temp_char,"Vpp = %d\n", runmanager.GetConfig()->GetVpp());
    temp_string += temp_char;
    sprintf (temp_char,"CreateFit = %d\n", runmanager.GetConfig()->GetCreateFit());
    temp_string += temp_char;
    sprintf (temp_char,"Read Temperatures = %d\n", runmanager.GetConfig()->GetReadTemp());
    temp_string += temp_char;
    str = temp_string.c_str();
    // str = "Hello";
    serv->SetItemField("/Parameters", "value", str);
}
