#include "stdafx.h"
#include "RunManager.h"
#include "GlobalWrapper.h"


int main(int argc, char *argv[])
{
    // Digitizer *digitizer = new Digitizer();
    // Server *server = new Server();
    // RunManager runmanager(argv[1]);
    RunManager runmanager("dconfnew.cfg");

    GlobalWrapper<RunManager>::GetInstance().Setter(&runmanager);
    
    signal(SIGINT, myHandler);

    runmanager.Configure();
    runmanager.Run();

    // runmanager.ReadAllConfigsFromFile("dconfnew.cfg");

    return 0;

}