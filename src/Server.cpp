#include "Server.h"

extern RunManager runmanager;

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
    serv->RegisterCommand("/commands/FitAfterRun", "ChangeParameterOne(%arg1%,\"fit_after\");", "rootsys/icons/right_arrow_cursor.png");
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

void ChangeParameterOne(int arg1, const char *ch)
{
    if (strcmp("num_evs", ch) == 0) runmanager.GetRconfig().Nevents = arg1; //std::cout << runmanager.GetRconfig().Nevents << std::endl;
    if (strcmp("num_samps", ch) == 0) runmanager.GetDconfig().Samples = arg1;
    if (strcmp("posttrigger", ch) == 0) runmanager.GetDconfig().PostTrigger = arg1;
    if (strcmp("charge_width", ch) == 0) runmanager.GetAconfig().WindowWidth = arg1;
    if (strcmp("vpp", ch) == 0) runmanager.GetDconfig().Vpp = arg1;
    if (strcmp("run_time", ch) == 0) runmanager.GetRconfig().Actime = arg1;
    if (strcmp("read_temps", ch) == 0) runmanager.GetRconfig().ReadTemp = arg1;
    if (strcmp("fit_after", ch) == 0) runmanager.GetRconfig().ReadTemp = arg1;
}

void ChangeParameterTwo(int arg1, int arg2, const char *ch)
{
    if (strcmp("thrs", ch) == 0) runmanager.GetDconfig().thresh_db[arg1] = arg2;
    if (strcmp("dcoff", ch) == 0) runmanager.GetDconfig().dcoffset_db[arg1] = arg2;
    if (strcmp("change_window", ch) == 0)
    {
        runmanager.GetAconfig().rmin = arg1;
        runmanager.GetAconfig().rmax = arg2;
    }
    if (strcmp("change_start", ch) == 0)
    {
        runmanager.GetAconfig().intsig_db[arg1] = arg2;
        runmanager.GetAconfig().intbl_db[arg1] = runmanager.GetAconfig().intsig_db[arg1] - runmanager.GetAconfig().WindowWidth;
    }
}

Server::~Server()
{
    delete serv;
}

void cmdStart()
{
    runmanager.GetRunParameters().run_status = 2;
    runmanager.GetRunParameters().NeedToProgram = 1;

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
    runmanager.GetRunParameters().run_status = 2;
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
    runmanager.GetRunParameters().run_status = 1;
    runmanager.GetRunParameters().NeedToProgram = 0;

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

    if (runmanager.GetRunParameters().run_status == 0)
    {
        serv->SetItemField("/Status", "value", "Stop");
        serv->SetItemField("/Status", "_status", "0"); 
    }
    if (runmanager.GetRunParameters().run_status == 2)
    {
        sprintf(temp_char,"Running... \n%d / %d\n", runmanager.GetRunParameters().nevent, runmanager.GetRconfig().Nevents);
        serv->SetItemField("/Status", "value", temp_char);
        serv->SetItemField("/Status", "_status", "1");
    }
    if (runmanager.GetRunParameters().run_status == 1)
    {
        sprintf(temp_char,"Idling... \n%d / %d\n", runmanager.GetRunParameters().nevent, runmanager.GetRconfig().Nevents);
        serv->SetItemField("/Status", "value", temp_char);
        serv->SetItemField("/Status", "_status", "1");
    }
    
    serv->SetItemField("/Fits", "value", "Undefined");

    sprintf(temp_char, "Run will stop after %d events or %d seconds", runmanager.GetRconfig().Nevents, runmanager.GetRconfig().Actime);
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
