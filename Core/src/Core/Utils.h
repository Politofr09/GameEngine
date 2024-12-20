#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <ctime>

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

namespace Core
{

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

#define LOG_ERROR(x) Core::Logger::LogError(x, __FILE__, __LINE__)
#define LOG_WARNING(x) Core::Logger::LogWarning(x, __FILE__, __LINE__)
#define LOG_INFO(x) Core::Logger::LogInfo(x, __FILE__, __LINE__)
#define LOG_DEBUG(x) Core::Logger::LogDebug(x, __FILE__, __LINE__)

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v);

glm::vec3 YAMLToVec3(const YAML::Node& node);

}
