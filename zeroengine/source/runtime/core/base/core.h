#pragma once

#include <memory>

#include <CDX12/Common/Timer.h>

#include "runtime/core/base/hash.h"

#ifdef NDEBUG
#define ASSERT(statement, msg)
#else
#define ASSERT(statement, msg) \
    if (!(statement)) { \
        LOG_ERROR("{0}", msg); \
        __debugbreak(); \
    }
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
#define ZE_XSTR(s) ZE_STR(s)
#define ZE_STR(s) #s

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace Zero {
    template <typename T>
    using Scope = std::unique_ptr<T>;
    template <typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using Ref = std::shared_ptr<T>;
    template <typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    using Chen::CDX12::Timer;
} // namespace Zero