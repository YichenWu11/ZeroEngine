#pragma once

#include <memory>

#ifdef NDEBUG
#define ZE_ASSERT(statement)
#else
#define ZE_ASSERT(statement) assert(statement)
#endif

#define BIT(x) (1 << x)

#define ZE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
#define ZERO_XSTR(s) ZERO_STR(s)
#define ZERO_STR(s) #s

template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace Zero {
    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T>
    using Ref = std::shared_ptr<T>;
} // namespace Zero