#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#pragma once

namespace Core
{

#ifdef DEBUG
#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "Assertion failed at %s:%d\n", __FILE__, __LINE__); \
            printf("Press enter to continue program"); \
            std::cin.get(); \
        } \
    } while (0)
#else
#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "Assertion failed at %s:%d\n", __FILE__, __LINE__); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)
#endif

class Logger
{
public:
    enum class LogLevels
    {
        LOG_ERROR,
        LOG_WARNING,
        LOG_INFO,
        LOG_DEBUG
    };

    static void Log(const std::string& message, LogLevels level = LogLevels::LOG_INFO, const char* file = __FILE__, int line = __LINE__) {
        std::string levelStr;
        switch (level) {
        case LogLevels::LOG_ERROR:
            levelStr = "ERROR";
            break;
        case LogLevels::LOG_WARNING:
            levelStr = "WARNING";
            break;
        case LogLevels::LOG_INFO:
            levelStr = "INFO";
            break;
        case LogLevels::LOG_DEBUG:
            levelStr = "DEBUG";
            break;
        default: break;
        }

        std::time_t t = std::time(nullptr);
        std::cout << "[" << levelStr << "] " << std::asctime(std::localtime(&t)) << "[" << file << ":" << line << "] " << message << std::endl;
    }

    static void LogError(const std::string& message, const char* file = __FILE__, int line = __LINE__) {
        Log(message, LogLevels::LOG_ERROR, file, line);
    }

    static void LogWarning(const std::string& message, const char* file = __FILE__, int line = __LINE__) {
        Log(message, LogLevels::LOG_WARNING, file, line);
    }

    static void LogInfo(const std::string& message, const char* file = __FILE__, int line = __LINE__) {
        Log(message, LogLevels::LOG_INFO, file, line);
    }

    static void LogDebug(const std::string& message, const char* file = __FILE__, int line = __LINE__) {
        Log(message, LogLevels::LOG_DEBUG, file, line);
    }
};

}