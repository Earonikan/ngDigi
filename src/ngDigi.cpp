#include "stdafx.h"
#include "CAENDigitizerEmu.h"
#include "Digitizer.h"
#include "RunManager.h"
#include "Server.h"
#include "utils.h"

void myHandler(int dummy);

RunManager runmanager;

int main(int argc, char *argv[])
{
    Digitizer *digitizer = new Digitizer();
    Server *server = new Server();
    
    signal(SIGINT, myHandler);

    runmanager.ReadAllConfigsFromFile(argv[1]);
    // std::cout << runmanager.GetDconfig().test_db[0] << std::endl;
    // runmanager.ReadAllConfigsFromFile("dconfnew.cfg");
    runmanager.Configure(server, digitizer);
    runmanager.Run();


    delete digitizer;
    delete server;

    return 0;

}

void myHandler(int dummy)
{
    // std::cout << "stopping..." << std::endl;
    sleep(1);
    std::cout << "exiting..." << std::endl;
    runmanager.SetStatus(0);
}