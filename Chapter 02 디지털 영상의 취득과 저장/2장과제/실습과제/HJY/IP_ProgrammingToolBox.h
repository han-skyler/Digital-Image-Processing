#pragma once
#include "IP_IO.h"

class CIP_ProgrammingToolBox
{
public:
    CIP_IO io;   // CIP_IO 클래스 객체 포함

public:
    CIP_ProgrammingToolBox(void);
    ~CIP_ProgrammingToolBox(void);
};
