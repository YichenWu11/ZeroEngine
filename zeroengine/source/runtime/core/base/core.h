#pragma once

#include <memory>

#ifdef NDEBUG
#define ZE_ASSERT(statement)
#else
#define ZE_ASSERT(statement) assert(statement)
#endif

#define BIT(x) (1 << x)

#define ZE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

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
} // namespace Zero