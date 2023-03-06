#include "Server.h"

extern RunManager runmanager;

Server::Server()
{
    serv = new THttpServer("http:9000;rw");
    serv->SetItemField("/Status", "value", "Stop");
    serv->SetItemField("/Status", "_status", "0");

    gInterpreter->Declare("void cmdStart();"); 
    gInterpreter->Declare("void cmdStop();");
    gInterpreter->Declare("void ChangeParameterOneArg(int arg1, const char *ch);");
    gInterpreter->Declare("void ChangeParameterTwoArg(int arg1, int arg2, const char *ch);");
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
    serv->RegisterCommand("/commands/Change_number_of_events", "ChangeParameterOne(%arg1%,\"num_evs\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_number_of_samples", "ChangeParameterOne(%arg1%,\"num_samps\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_posttrigger", "ChangeParameterOne(%arg1%,\"posttrigger\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_thresholds", "ChangeParameterTwo(%arg1%,%arg2%,\"thrs\");", "rootsys/icons/right_arrow_cursor.png");
    serv->RegisterCommand("/commands/Change_dcoffset_trigger", "ChangeParameterTwo(%arg1%,%arg2%,\"dcoff\");", "rootsys/icons/right_arrow_cursor.png");
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

void ChangeParameterOneArg(int arg1, const char *ch)
{
    if (strcmp("num_evs", ch) == 0) runmanager.GetRconfig().Nevents = arg1;
    if (strcmp("num_samps", ch) == 0) runmanager.GetDconfig().Samples = arg1;
    if (strcmp("posttrigger", ch) == 0) runmanager.GetDconfig().PostTrigger = arg1;
}

void ChangeParameterTwoArg(int arg1, int arg2, const char *ch)
{
    if (strcmp("thrs", ch) == 0) runmanager.GetDconfig().thresh_db[arg1] = arg2;
    if (strcmp("dcoff", ch) == 0) runmanager.GetDconfig().dcoffset_db[arg1] = arg2;
}

Server::~Server()
{
    delete serv;
}

void cmdStart()
{
    runmanager.SetStatus(2);    
    runmanager.GetRconfig().StartTime = GetCurrentTime();
    runmanager.GetRunParameters().PrevRateTime = runmanager.GetRconfig().StartTime;

    runmanager.GetRunParameters().Nbytes = 0;
    runmanager.GetRunParameters().Nevs = 0;
    runmanager.GetRunParameters().nCycles= 0;
    runmanager.GetRunParameters().nevent = 0;

    // resethistos(); //HISTOS
    runmanager.GetDigitizer()->Program(runmanager.GetDconfig());
    runmanager.GetDigitizer()->AllocateEvents();
    sleep(2);
    runmanager.GetDigitizer()->Execute(CAEN_DGTZ_SWStartAcquisition(runmanager.GetRunParameters().handle));
    runmanager.GetRconfig().run_status = 2;
    runmanager.GetAconfig().CreateFit = 0;

    std::cout << "cmdStart()" << std::endl;
    // if (readtemp!=0)
    // {
    //     wget(temp0, "http://minitrs01.cern.ch", "/", 80);
    //     serv->SetItemField("/Temperatures", "value", temp0);
    // }
}

void cmdStop()
{
    runmanager.SetStatus(1);

    CAEN_DGTZ_SWStopAcquisition(runmanager.GetRunParameters().handle);
    std::cout << "cmdStop()" << std::endl;
    // serv->SetItemField("/Status", "value", "Stop");
    // serv->SetItemField("/Status", "_status", "0");
    // if (readtemp!=0)
    // {
    //     wget(temp1, "http://minitrs01.cern.ch", "/", 80);
    //     serv->SetItemField("/Temperatures", "value", temp0+"\n"+temp1);
    // }
}

void Server::UpdateParametersField()
{
    std::string temp_string;
    char temp_char[200];

    if (runmanager.GetRconfig().run_status == 0)
    {
        serv->SetItemField("/Status", "value", "Stop");
        serv->SetItemField("/Status", "_status", "0"); 
    }
    else
    {
        sprintf(temp_char,"Running \n%d / %d\n", runmanager.GetRunParameters().nevent, runmanager.GetRconfig().Nevents);
        serv->SetItemField("/Status", "value", temp_char);
        serv->SetItemField("/Status", "_status", "1");
    } 
    
    serv->SetItemField("/Fits", "value", "Undefined");

    sprintf(temp_char, "Run will stop after %d events or %d seconds", runmanager.GetRconfig().Nevents, runmanager.GetRconfig().Nevents);
    temp_string += temp_char;
    sprintf(temp_char, "\nNumber of samples = %d", runmanager.GetDconfig().Samples);
    temp_string += temp_char;
    sprintf(temp_char, "\nPostTrigger = %d ", runmanager.GetDconfig().PostTrigger);
    temp_string += temp_char;
    sprintf(temp_char, "\nTrigger channels: ");
    temp_string += temp_char;
    for (int i = 0; i < runmanager.GetDconfig().NumChannels; i++) if (runmanager.GetDconfig().chtype_db[i] == 2)
    {
        sprintf(temp_char,"%d ", i);
        temp_string += temp_char;
    }
    sprintf(temp_char, "\n");
    temp_string += temp_char;
    for (int i = 0; i < runmanager.GetDconfig().NumChannels; i++) if (runmanager.GetDconfig().chtype_db[i] == 2)
    {
        sprintf(temp_char, "Channel %d threshold = %d\n", i, runmanager.GetDconfig().thresh_db[i]);
        temp_string += temp_char;
    }
    sprintf(temp_char, "\n");
    for (int i = 0; i < runmanager.GetDconfig().NumChannels; i++) if (runmanager.GetDconfig().chtype_db[i] > 0)
    {
        sprintf(temp_char, "Channel %d DCoffset = %d\n", i, runmanager.GetDconfig().dcoffset_db[i]);
        temp_string += temp_char;
    }
    sprintf (temp_char,"\nIntegration window width = %d\n", runmanager.GetAconfig().WindowWidth);
    temp_string += temp_char;
    for (int i = 0; i < runmanager.GetDconfig().NumChannels; i++) if (runmanager.GetDconfig().chtype_db[i] > 0)
    {
        sprintf(temp_char, "Channel %d trigger window = (%d, %d)\n", i, runmanager.GetAconfig().intsig_db[i], runmanager.GetAconfig().intbl_db[i] - runmanager.GetAconfig().WindowWidth);
        temp_string += temp_char;
    }
    sprintf (temp_char,"Charge Histos Range = (%d, %d)\n", runmanager.GetAconfig().rmin, runmanager.GetAconfig().rmax);
    temp_string += temp_char;
    sprintf (temp_char,"Vpp = %d\n", runmanager.GetDconfig().Vpp);
    temp_string += temp_char;
    sprintf (temp_char,"CreateFit = %d\n", runmanager.GetAconfig().CreateFit);
    temp_string += temp_char;
    sprintf (temp_char,"Read Temperatures = %d\n", runmanager.GetRconfig().ReadTemp);
    temp_string += temp_char;
    str = temp_string.c_str();
    // str = "Hello";
    serv->SetItemField("/Parameters", "value", str);
}
