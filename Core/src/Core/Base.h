#pragma once

#include <memory>

namespace Core
{

    #ifdef _WIN32
    #define ASSERT(x) \
        do { \
            if(!(x)) {\
                std::cerr << "Assertion failed\n";\
                std::cerr << " in file " << __FILE__ << " at line " << __LINE__ << std::endl;\
                __debugbreak();\
            } \
        } while(false)

    #else
    #define ASSERT(x) \
        do { \
            if(!(x)) {\
                std::cerr << "Assertion failed\n";\
                std::cerr << " in file " << __FILE__ << " at line " << __LINE__ << std::endl;\
                std::abort();\
            } \
        } while(false)
    #endif

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
	    return std::make_shared<T>(std::forward<Args>(args)...);
    }

}

#include "Utils.h"
#include "UUID.h"