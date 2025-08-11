#pragma once

#include "Utils.h"

class CLog
{
public:
    CLog(const std::string& name);
    ~CLog();
    void Log(const std::string& str);

private:
    std::string _name;

};

extern "C"
{
    void* CreateCLog_Log(MonoString* name);
    void CallCLog_Log(CLog* obj, MonoString* monoStr);
    void DestoryCLog_Log(void* ptr);
}

void RegisterInternalCalls(const char* nspace_name, const char* class_name);