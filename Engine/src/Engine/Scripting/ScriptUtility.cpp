/************************************************************************************//*!
\file           ScriptUtility.cpp
\project        <PROJECT_NAME>
\author         Solomon Tan Teng Shue, t.tengshuesolomon, 620010020
\par            email: t.tengshuesolomon\@digipen.edu
\date           August 3, 2021
\brief          Defines any helper functions declared in ScriptUtility.h to make
                using the mono library easier

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/

#include "pch.h"
#include "ScriptUtility.h"
#include "Engine/Core/Log.h"

namespace engine
{
    namespace ScriptUtility
    {
        ScriptSystemInfo g_SystemInfo;

        void ScriptSystemInfo::Initialize(const char* dllPath, std::unordered_map<std::string, ScriptSystem::RegisteredComponent> const& refMap)
        {
            // load scripting dll
            if (mono_domain_get() == nullptr)
            {
                MonoDomain* rootDomain = mono_jit_init("root");
                mono_thread_set_main(mono_thread_current());
            }
            domain = mono_domain_create_appdomain((char*)"scripts", NULL);
            if (!mono_domain_set(domain, false))
            {
                LOG_ENGINE_ERROR("Script Loading Error: failed to set scripting domain");
                return;
            }

            std::ifstream ifsDll(dllPath, std::ifstream::binary);
            if (!ifsDll)
            {
                LOG_ENGINE_ERROR("Script Loading Error: failed to open dll file");
                return;
            }
            ifsDll.seekg(0, ifsDll.end);
            size_t dllLength = ifsDll.tellg();
            ifsDll.seekg(0, ifsDll.beg);
            std::vector<char> dllData(dllLength);
            ifsDll.read(&dllData[0], dllLength);
            ifsDll.close();

            MonoImageOpenStatus status;
            MonoImage* image = mono_image_open_from_data_full(&dllData[0], dllData.size(), true, &status, false);
            if (image == nullptr || status != MONO_IMAGE_OK)
            {
                LOG_ENGINE_ERROR("Script Loading Error: failed to set load image from memory");
                Reset();
                return;
            }
            MonoAssembly* assembly = mono_assembly_load_from_full(image, dllPath, &status, false);
            mono_image_close(image);
            if (assembly == nullptr || status != MONO_IMAGE_OK)
            {
                LOG_ENGINE_ERROR("Script Loading Error: failed to set load assembly from image");
                Reset();
                return;
            }

            //MonoAssembly* assembly = mono_domain_assembly_open(domain, dllPath);
            //if (assembly == nullptr)
            //{
            //    LOG_ENGINE_ERROR("Script Loading Error: failed to open assembly using " + std::string(dllPath));
            //    domain = nullptr;
            //    return;
            //}

            mono_set_dirs("../Engine/vendor/mono/lib/mono/4.5", NULL);
            scripting = mono_assembly_get_image(assembly);

            // get all script and component class info
            MonoClass* baseScriptClass = GetBaseScriptMonoClass();
            const MonoTableInfo* tableInfo = mono_image_get_table_info(scripting, MONO_TABLE_TYPEDEF);
            unsigned int tableRows = mono_table_info_get_rows(tableInfo);
            for (unsigned int i = 1; i < tableRows; ++i)
            {
                uint32_t cols[MONO_TYPEDEF_SIZE];
                mono_metadata_decode_row(tableInfo, i, cols, MONO_TYPEDEF_SIZE);
                const char* name = mono_metadata_string_heap(scripting, cols[MONO_TYPEDEF_NAME]);
                const char* name_space = mono_metadata_string_heap(scripting, cols[MONO_TYPEDEF_NAMESPACE]);
                MonoClass* _class = mono_class_from_name(scripting, name_space, name);

                if (_class != baseScriptClass && CheckBaseClass(_class, baseScriptClass))
                {
                    classInfoList.push_back(ScriptClassInfo(name_space, name));
                    continue;
                }

                std::string fullName = std::string(name_space) + "." + std::string(name);
                auto& search = refMap.find(fullName);
                if (search == refMap.end())
                    continue;
                MonoType* type = mono_class_get_type(_class);
                int id = componentMap.size();
                componentMap.insert({ type, search->second });
            }
        }

        void ScriptSystemInfo::Reset()
        {
            // unload current app domain
            MonoDomain* oldDomain = mono_domain_get();
            if (oldDomain != nullptr && oldDomain != mono_get_root_domain())
            {
                if (!mono_domain_set(mono_get_root_domain(), false))
                {
                    LOG_ENGINE_ERROR("Script Compiling Error: failed to set root domain");
                }
                mono_domain_unload(oldDomain);
                // Trigger C# garbage collection, not necessary but good point to clean up stuff
                mono_gc_collect(mono_gc_max_generation());
            }
            domain = nullptr;
            scripting = nullptr;
            classInfoList.clear();
            componentMap.clear();
        }

        /*-----------------------------------------------------------------------------*/
        /* Helper Functions                                                            */
        /*-----------------------------------------------------------------------------*/
        
        MonoClass* GetMonoClass(const char* name_space, const char* name)
        {
            return mono_class_from_name(g_SystemInfo.scripting, name_space, name);
        }

        MonoClass* GetMonoClass(ScriptClassInfo const& classInfo)
        {
            return mono_class_from_name(g_SystemInfo.scripting, classInfo.name_space.c_str(), classInfo.name.c_str());
        }

        MonoClass* GetGameObjectMonoClass()
        {
            return mono_class_from_name(g_SystemInfo.scripting, "Ouroboros", "GameObject");
        }

        MonoClass* GetBaseComponentMonoClass()
        {
            return mono_class_from_name(g_SystemInfo.scripting, "Ouroboros", "Component");
        }

        MonoClass* GetBaseScriptMonoClass()
        {
            return mono_class_from_name(g_SystemInfo.scripting, "Ouroboros", "MonoBehaviour");
        }

        MonoClass* GetTransformMonoClass()
        {
            return mono_class_from_name(g_SystemInfo.scripting, "Ouroboros", "Transform");
        }

        MonoObject* MonoObjectNew(MonoClass* klass)
        {
            return mono_object_new(g_SystemInfo.domain, klass);
        }

        MonoString* MonoStringNew(const char* text)
        {
            return mono_string_new(g_SystemInfo.domain, text);
        }

        ScriptSystem::RegisteredComponent const& GetRegisteredComponent(MonoType* type)
        {
            auto& search = g_SystemInfo.componentMap.find(type);
            ENGINE_ASSERT(search != g_SystemInfo.componentMap.end());
            return search->second;
        }

        bool IsClassFieldPublic(MonoClassField* field)
        {
            MonoClass* testClass = mono_get_object_class();
            MonoMethod* testMethod = mono_class_get_method_from_name(testClass, "GetType", 0);
            return mono_method_can_access_field(testMethod, field);
        }

        bool CheckBaseClass(MonoClass* klass, MonoClass* desiredBase)
        {
            while (klass != nullptr)
            {
                if (klass == desiredBase)
                    return true;
                klass = mono_class_get_parent(klass);
            }
            return false;
        }

        bool IsMonoTypeGenericList(MonoType* type)
        {
            MonoObject* typeObj = (MonoObject*)mono_type_get_object(g_SystemInfo.domain, type);
            MonoClass* typeClass = mono_object_get_class(typeObj);

            MonoProperty* genericProperty = mono_class_get_property_from_name(typeClass, "IsGenericType");
            bool isGenericType = *(bool*)mono_property_get_value(genericProperty, typeObj, NULL, NULL);
            if (!isGenericType)
                return false;

            MonoClass* listClass = mono_class_from_name(mono_get_corlib(), "System.Collections.Generic", "List`1");
            MonoMethod* defMethod = mono_class_get_method_from_name(typeClass, "GetGenericTypeDefinition", 0);
            MonoReflectionType* result = (MonoReflectionType*)mono_runtime_invoke(defMethod, typeObj, NULL, NULL);
            MonoClass* resultClass = mono_type_get_class(mono_reflection_type_get_type(result));
            return listClass == resultClass;
        }

        MonoMethod* FindFunction(MonoObject* obj, const char* functionName, int paramCount)
        {
            MonoClass* _class = mono_object_get_class(obj);
            MonoMethod* _method = mono_class_get_method_from_name(_class, functionName, paramCount);
            MonoClass* _pClass = mono_class_get_parent(_class);

            while (_method == nullptr && _pClass != nullptr && _class != _pClass)
            {
                _class = _pClass;
                _pClass = mono_class_get_parent(_class);
                _method = mono_class_get_method_from_name(_class, functionName, paramCount);
            }
            return _method;
        }

        std::vector<MonoClass*> const GetMonoTypeGenericClasses(MonoType* type)
        {
            std::vector<MonoClass*> resultList;
            MonoObject* typeObj = (MonoObject*)mono_type_get_object(g_SystemInfo.domain, type);
            MonoClass* typeClass = mono_object_get_class(typeObj);
            MonoMethod* getMethod = mono_class_get_method_from_name(typeClass, "GetGenericArguments", 0);
            MonoArray* resultArray = (MonoArray*)mono_runtime_invoke(getMethod, typeObj, NULL, NULL);
            unsigned int arrayLength = mono_array_length(resultArray);
            for (unsigned int i = 0; i < arrayLength; ++i)
            {
                MonoReflectionType* genericType = mono_array_get(resultArray, MonoReflectionType*, i);
                MonoClass* genericClass = mono_type_get_class(mono_reflection_type_get_type(genericType));
                resultList.push_back(genericClass);
            }
            return resultList;
        }
    }
}
