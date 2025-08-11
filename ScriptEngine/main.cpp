#include "Utils.h"
#include "CLog.h"

struct ManagedBehaviour
{
    MonoObject* instance;
    MonoMethod* startMethod;
    MonoMethod* updateMethod;
};

std::vector<ManagedBehaviour> behaviours;

MonoDomain* domain;

MonoObject* CreateCSharpObject(const char* assemblyName, const char* namespaceName, const char* className)
{
    MonoAssembly* assembly = mono_domain_assembly_open(domain, assemblyName);
    if (!assembly)
        throw std::runtime_error("Assembly load failed");

    MonoImage* image = mono_assembly_get_image(assembly);
    MonoClass* classInst = mono_class_from_name(image, namespaceName, className);
    if (!classInst)
        throw std::runtime_error("Class not found");

    MonoObject* obj = mono_object_new(domain, classInst);
    mono_runtime_object_init(obj);
    return obj;
}

void AddBehaviour(MonoObject* obj)
{
    MonoClass* classInst = mono_object_get_class(obj);
    MonoMethod* start = mono_class_get_method_from_name(classInst, "Start", 0);
    MonoMethod* update = mono_class_get_method_from_name(classInst, "Update", 0);

    behaviours.push_back({ obj, start, update });
}

void GameRun()
{
    for (auto& b : behaviours)
    {
        mono_runtime_invoke(b.startMethod, b.instance, nullptr, nullptr);
    }

    const int fakeGameLoopCount = 3;
    for (int i = 0; i < fakeGameLoopCount; ++i)
    {
        for (auto& b : behaviours)
        {
            mono_runtime_invoke(b.updateMethod, b.instance, nullptr, nullptr);
        }
    }
}

int main()
{
    mono_set_dirs("../ThirdParty/lib", "../ThirdParty/lib");
    domain = mono_jit_init("MyDomain");
    RegisterInternalCalls("GameAssembly", "CLog");

    char tr_nspace[0x100] = {};
    char tr_class[0x100] = {};

    try
    {
        // C# Mono namespace&class 이름 추적
        {
            MonoAssembly* assembly = mono_domain_assembly_open(mono_domain_get(), "GameAssembly.dll");
            if (!assembly) {
                printf("Failed to load assembly!\n");
                return 0;
            }

            MonoImage* image = mono_assembly_get_image(assembly);
            const MonoTableInfo* table = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
            int rows = mono_table_info_get_rows(table);

            for (int i = 0; i < rows; i++)
            {
                uint32_t cols[MONO_TYPEDEF_SIZE];
                mono_metadata_decode_row(table, i, cols, MONO_TYPEDEF_SIZE);

                const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
                const char* nspace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);

                // 클래스 로드
                MonoClass* klass = mono_class_from_name(image, nspace, name);

                // MonoBehaviour 상속 여부 체크
                MonoClass* parent = mono_class_get_parent(klass);
                while (parent)
                {
                    const char* pname = mono_class_get_name(parent);
                    if (strcmp(pname, "MonoBehaviour") == 0)
                    {
                        printf("Found MonoBehaviour: %s::%s\n", nspace, name);
                        strcpy_s(tr_nspace, nspace);
                        strcpy_s(tr_class, name);
                        break;
                    }
                    parent = mono_class_get_parent(parent);
                }
            }
        }

        // C# 객체 생성
        // MonoObject* player = CreateCSharpObject(ASSEMBLY_DLL_FILENAME, "GameAssembly", "Player");
        MonoObject* player = CreateCSharpObject(ASSEMBLY_DLL_FILENAME, tr_nspace, tr_class);
        AddBehaviour(player);

        // 가짜 게임 루프
        GameRun();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }

    mono_jit_cleanup(domain);
    return 0;
}