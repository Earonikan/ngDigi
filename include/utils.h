#pragma once

#include "stdafx.h"
#include "RunManager.h"
#include "GlobalWrapper.h"

long GetCurrentTime();
void myHandler(int dummy);

void cmdStart();
void cmdStop();
void cmdUpdate();
void ChangeParameterOne(int arg1, const char *ch);
void ChangeParameterTwo(int arg1, int arg2, const char *ch);
    