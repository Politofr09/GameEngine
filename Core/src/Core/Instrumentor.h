#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Core
{

    struct ProfileResult
    {
        std::string Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {
    private:
        InstrumentationSession* m_CurrentSession;
        std::ofstream m_OutputStream;
        int m_ProfileCount;
    public:
        Instrumentor()
            : m_CurrentSession(nullptr), m_ProfileCount(0)
        {
        }

        void BeginSession(const std::string& name, const std::string& filepath = "results.json");

        void EndSession();

        void WriteProfile(const ProfileResult& result);

        void WriteHeader();

        void WriteFooter();

        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }
    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name);

        ~InstrumentationTimer();

        void Stop();
    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
    };


#define CORE_PROFILING 1

#ifdef CORE_PROFILING

#include "Instrumentor.h"

#define CORE_PROFILE_BEGIN_SESSION(name, filepath) ::Core::Instrumentor::Get().BeginSession(name, filepath)
#define CORE_PROFILE_END_SESSION() ::Core::Instrumentor::Get().EndSession()
#define CORE_PROFILE_SCOPE_LINE2(name, line) ::Core::InstrumentationTimer timer##line(__func__)
#define CORE_PROFILE_SCOPE_LINE(name, line) CORE_PROFILE_SCOPE_LINE2(name, line)
#define CORE_PROFILE_SCOPE(name) CORE_PROFILE_SCOPE_LINE(name, __LINE__)
#define CORE_PROFILE_FUNCTION() CORE_PROFILE_SCOPE(__FUNC__)

#else
#define CORE_PROFILE_BEGIN_SESSION(name, filepath)
#define CORE_PROFILE_END_SESSION()
#define CORE_PROFILE_SCOPE(name)
#define CORE_PROFILE_FUNCTION()
#endif

}