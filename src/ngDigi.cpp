#include "stdafx.h"
#include "CAENDigitizerEmu.h"
#include "Digitizer.h"
#include "RunManager.h"
#include "Server.h"
#include "Config.h"

bool f = 1;
void myHandler(int dummy);

RunManager runmanager;

int main(int argc, char *argv[])
{
    Digitizer *digitizer = new Digitizer();
    Server *server = new Server();
    Config *config = new Config();
    
    signal(SIGINT, myHandler);

    runmanager.ReadAllConfigsFromFile(argv[1]);
    digitizer->Program(runmanager.GetDconfig())

    runmanager.Configure(server, digitizer);
    runmanager.Run();


    delete digitizer;
    delete server;
    delete config;

    return 0;

}

void myHandler(int dummy)
{
    std::cout << "stopping..." << std::endl;
    sleep(1);
    std::cout << "exiting..." << std::endl;
    runmanager.SetStatus(0);
}