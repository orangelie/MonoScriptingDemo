#include "CLog.h"

CLog::CLog(const std::string& name)
{
	_name = name;
	std::cout << "[C++] Created: " << _name << std::endl;
}

CLog::~CLog()
{
	std::cout << "[C++] Destroyed: " << _name << std::endl;
}

void CLog::Log(const std::string& str)
{
	std::cout << "[C++] " << str << " from " << _name << std::endl;
}

void* CreateCLog_Log(MonoString* name)
{
	char* nativeName = mono_string_to_utf8(name);
	CLog* instance = new CLog(nativeName);
	mono_free(nativeName);

	return instance; // IntPtr로 반환
}

void CallCLog_Log(CLog* obj, MonoString* monoStr)
{
	if (!obj)
		return;

	char* utf8Str = mono_string_to_utf8(monoStr); // C# string → UTF-8
	std::string cppStr(utf8Str);
	mono_free(utf8Str); // 할당 해제 해야함 무적권 ㅇㅅㅇ;;

	obj->Log(cppStr);
}

void DestoryCLog_Log(void* ptr)
{
	if (ptr)
	{
		delete static_cast<CLog*>(ptr);
	}
}

void RegisterInternalCalls(const char* nspace_name, const char* class_name)
{
	std::string base = std::string(nspace_name) + "." + class_name + "::";

	std::string s0 = base + "Create";
	mono_add_internal_call(s0.c_str(), (const void*)CreateCLog_Log);

	std::string s1 = base + "NLog";
	mono_add_internal_call(s1.c_str(), (const void*)CallCLog_Log);

	std::string s2 = base + "Destroy";
	mono_add_internal_call(s2.c_str(), (const void*)DestoryCLog_Log);
}
