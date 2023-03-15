#include "utils.h"

long GetCurrentTime()
{
    struct timeval t1;
    gettimeofday(&t1, NULL);
    return (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
}

void myHandler(int dummy)
{
    std::cout << "exiting..." << std::endl;
    sleep(1);
    GlobalWrapper<RunManager>::GetInstance().Getter()->SetStatus(0);
}

void ChangeParameterOne(int arg1, const char *ch)
{
    GlobalWrapper<RunManager>::GetInstance().Getter()->SetParameter(arg1, ch);
}

void ChangeParameterTwo(int arg1, int arg2, const char *ch)
{
    GlobalWrapper<RunManager>::GetInstance().Getter()->SetVecParameter(arg1, arg2, ch);
}

void ChangeParameterChar(const char *arg1, const char *ch)
{
    GlobalWrapper<RunManager>::GetInstance().Getter()->SetCharParameters(arg1, ch);
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