#include "stdafx.h"
#include "CAENDigitizerEmu.h"
#include "Digitizer.h"
#include "RunManager.h"
#include "Server.h"
#include "utils.h"
#include "GlobalWrapper.h"


int main(int argc, char *argv[])
{
    // Digitizer *digitizer = new Digitizer();
    // Server *server = new Server();
    RunManager runmanager(argv[1]);

    GlobalWrapper<RunManager>::GetInstance().Setter(&runmanager);
    
    signal(SIGINT, myHandler);

    runmanager.Configure();
    runmanager.Run();

    // runmanager.ReadAllConfigsFromFile("dconfnew.cfg");

    return 0;

}