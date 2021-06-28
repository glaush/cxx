#include <any>
#include <coroutine>
#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <variant>

void shout(std::string_view msg) { std::cout << msg << '\n'; }

struct Open
{
};
struct Close
{
};
struct Knock
{
};
struct Lock
{
};
struct Unlock
{
};

enum class State
{
    Closed,
    Open,
    Locked
};

namespace switch_impl
{
struct Door
{
    State state = State::Closed;

    template <typename E>
    void onEvent(E)
    {
        switch (state)
        {
            case State::Closed:
                if constexpr (std::is_same_v<E, Knock>)
                {
                    shout("Come in, it's open!");  // нет перехода
                }
                else if constexpr (std::is_same_v<E, Open>)
                {
                    state = State::Open;  // переход в состояние Open
                }
                break;

            case State::Open:
                if constexpr (std::is_same_v<E, Close>)
                    state = State::Closed;  // переход в состояние Closed
        }
    }
};

struct Door2
{
    State state          = State::Closed;
    std::string response = "Come in, it's open!";

    template <typename E>
    void onEvent(E)
    {
        switch (state)
        {
            case State::Closed:
                if constexpr (std::is_same_v<E, Knock>)
                {
                    shout(response);
                }
                else if constexpr (std::is_same_v<E, Open>)
                {
                    state = State::Open;
                }
                break;
            case State::Open:
                if constexpr (std::is_same_v<E, Close>) state = State::Closed;
        }
    }
};

struct LockableDoor
{
    State state = State::Closed;

    template <typename E>
    void onEvent(E)
    {
        switch (state)
        {
            case State::Closed:
                if constexpr (std::is_same_v<E, Knock>)
                {
                    shout("Come in, it's open!");  // нет перехода
                }
                else if constexpr (std::is_same_v<E, Open>)
                {
                    state = State::Open;  // переход в состояние Open
                }
                else if constexpr (std::is_same_v<E, Lock>)
                {
                    state = State::Locked;  // переход в состояние Locked
                }
                break;
            case State::Open:
                if constexpr (std::is_same_v<E, Close>)
                    state = State::Closed;  // переход в состояние Closed
                break;
            case State::Locked:
                if constexpr (std::is_same_v<E, Unlock>)
                    state = State::Closed;  // переход в состояние Closed
        }
    }
};
}  // namespace switch_impl

namespace templates
{
template <typename... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

struct lambda1
{
    inline /*constexpr */ void operator()(int i) const { std::cout << "int: " << i << '\n'; }
};

struct lambda2
{
    inline /*constexpr */ void operator()(std::string s) const
    {
        std::cout << "string: " << s << '\n';
    }
};

template <>
struct overloaded<lambda1, lambda2> : lambda1, lambda2
{
    using lambda1::operator();
    using lambda2::operator();
};

void overloadedTest()
{
    {
        auto func = overloaded{[](int i) { std::cout << "int: " << i << '\n'; },
                               [](std::string s) { std::cout << "string: " << s << '\n'; }};
    }
    {
        overloaded<lambda1, lambda2> func = overloaded{lambda1{}, lambda2{}};
        func(42);
        func("Hello, C++ Siberia 2021!");
    }
}

template <typename S, typename... Handlers>
struct StateMachine;

template <typename S>
struct TransitionTo
{
    using TargetState = S;
};

template <typename... States, typename... Handlers>
struct StateMachine<std::tuple<States...>, Handlers...> : Handlers...
{
    using Handlers::operator()...;

    template <typename... H>
    StateMachine(H&&... h) : Handlers(h)...
    {
    }

    template <typename E>
    void onEvent(E&& e)
    {
        std::visit(
            [this, &e](auto statePtr) {
                if constexpr (std::is_invocable_v<StateMachine, decltype(*statePtr), E&&>)
                {
                    using ResultType = std::invoke_result_t<StateMachine, decltype(*statePtr), E&&>;
                    if constexpr (std::is_same_v<ResultType, void>)
                    {
                        (*this)(*statePtr, std::forward<E>(e));
                        std::cout << "(no transition)\n";
                    }
                    else
                    {
                        auto transitionTo = (*this)(*statePtr, std::forward<E>(e));
                        currentState      = &std::get<typename ResultType::TargetState>(states);
                        std::cout << "(transitioned to " << currentState.index() << ")\n";
                    }
                }
                else
                {
                    std::cout << "(no rules invoked)\n";
                }
            },
            currentState);
    }

    std::tuple<States...> states;
    std::variant<States*...> currentState = &std::get<0>(states);
};

template <typename S, typename... Handlers>
auto makeStateMachine(Handlers&&... h)
{
    return StateMachine<S, std::decay_t<Handlers>...>{std::forward<Handlers>(h)...};
}

struct StateOpen
{
};
struct StateClosed
{
    std::string response = "Come in, it's open!";
};
struct StateLocked
{
};

auto getDoor()
{
    auto sm = makeStateMachine<std::tuple<StateClosed, StateOpen>>(
        [](StateClosed& s, Open event) -> TransitionTo<StateOpen> { return {}; },
        [](StateClosed& s, Knock event) /*нет перехода*/ { shout("Come in, it's open!"); },
        [](StateOpen& s, Close event) -> TransitionTo<StateClosed> { return {}; });
    return sm;
}

auto getDoor2()
{
    auto sm = makeStateMachine<std::tuple<StateClosed, StateOpen>>(
        [](StateClosed& s, Open event) -> TransitionTo<StateOpen> { return {}; },
        [](StateClosed& s, Knock event) /*нет перехода*/ { shout(s.response); },
        [](StateOpen& s, Close event) -> TransitionTo<StateClosed> { return {}; });
    return sm;
}

auto getDoor3()
{
    auto myResponse = "Nobody's home. Come later.";
    auto sm         = makeStateMachine<std::tuple<StateClosed, StateOpen>>(
        [](StateClosed& s, Open event) -> TransitionTo<StateOpen> { return {}; },
        [myResponse](StateClosed& s, Knock event) /*нет перехода*/ { shout(myResponse); },
        [](StateOpen& s, Close event) -> TransitionTo<StateClosed> { return {}; });
    return sm;
}

auto getLockableDoor()
{
    auto sm = makeStateMachine<std::tuple<StateClosed, StateOpen, StateLocked>>(
        [](StateClosed& s, Open event) -> TransitionTo<StateOpen> { return {}; },
        [](StateClosed& s, Knock event) /*нет перехода*/ { shout("Come in, it's open!"); },
        [](StateOpen& s, Close event) -> TransitionTo<StateClosed> { return {}; },
        [](StateClosed& s, Lock event) -> TransitionTo<StateLocked> { return {}; },
        [](StateLocked& s, Unlock event) -> TransitionTo<StateClosed> { return {}; });
    return sm;
}
}  // namespace templates

namespace coroutines
{
template <typename... Events>
struct Event
{
};

struct StateMachine
{
    struct promise_type
    {
        using CoroHandle = std::coroutine_handle<promise_type>;
        StateMachine get_return_object() noexcept { return {CoroHandle::from_promise(*this)}; }
        auto initial_suspend() const noexcept { return std::suspend_never{}; }
        auto final_suspend() const noexcept { return std::suspend_always{}; }
        void return_void() noexcept {}
        void unhandled_exception() noexcept {}

        template <typename... E>
        auto await_transform(Event<E...>) noexcept
        {
            isWantedEvent = [](const std::type_info& type) -> bool {
                return ((type == typeid(E)) || ...);
            };

            struct Awaitable
            {
                bool await_ready() const noexcept { return false; }
                void await_suspend(CoroHandle) noexcept {}
                auto await_resume() const
                {
                    std::variant<E...> event;
                    (void)((currentEvent->type() == typeid(E)
                                ? (event = std::move(*std::any_cast<E>(currentEvent)), true)
                                : false) ||
                           ...);
                    return event;
                }
                const std::any* currentEvent;
            };

            return Awaitable{&currentEvent};
        }

        std::any currentEvent;
        bool (*isWantedEvent)(const std::type_info&) = nullptr;
    };

    template <typename E>
    void onEvent(E&& e)
    {
        auto& promise = coro.promise();
        if (promise.isWantedEvent(typeid(E)))
        {
            promise.currentEvent = std::forward<E>(e);
            coro();
        }
    }

    StateMachine(StateMachine&&) = default;
    StateMachine& operator=(StateMachine&&) = default;
    ~StateMachine() { coro.destroy(); }

private:
    StateMachine(std::coroutine_handle<promise_type> coro) : coro{coro} {}
    std::coroutine_handle<promise_type> coro;
};

StateMachine getDoor()
{
    for (;;)
    {
        // closed
        auto e = co_await Event<Open, Knock>{};
        if (std::holds_alternative<Knock>(e))
        {
            shout("Come in, it's open!");
        }
        else if (std::holds_alternative<Open>(e))
        {
            // open
            co_await Event<Close>{};
        }
    }
}

StateMachine getDoor2()
{
closed:
    for (;;)
    {
        auto e = co_await Event<Open, Knock>{};
        if (std::holds_alternative<Knock>(e))
        {
            shout("Come in, it's open!");
        }
        else if (std::holds_alternative<Open>(e))
        {
            goto open;
        }
    }
open:
    co_await Event<Close>{};
    goto closed;
}

StateMachine getDoor(std::string response)
{
closed:
    for (;;)
    {
        auto e = co_await Event<Open, Knock>{};
        if (std::holds_alternative<Knock>(e))
        {
            shout(response);
        }
        else if (std::holds_alternative<Open>(e))
        {
            goto open;
        }
    }
open:
    co_await Event<Close>{};
    goto closed;
}

struct Connect
{
};
struct Request
{
    size_t id;
};
struct Disconnect
{
};

void handleRequest(const Request& r) { std::cout << "handling request " << r.id << '\n'; }

StateMachine getHandler()
{
    co_await Event<Connect>{};
    for (;;)
    {
        auto e = co_await Event<Disconnect, Request>{};
        if (std::holds_alternative<Request>(e))
        {
            handleRequest(std::get<Request>(e));
        }
        else if (std::holds_alternative<Disconnect>(e))
        {
            co_await Event<Connect>{};
        }
    }
}

StateMachine getLockableDoor()
{
closed:
    for (;;)
    {
        auto e = co_await Event<Open, Knock, Lock>{};
        if (std::holds_alternative<Knock>(e))
        {
            shout("Come in, it's open!");
        }
        else if (std::holds_alternative<Open>(e))
        {
            goto open;
        }
        else if (std::holds_alternative<Lock>(e))
        {
            goto locked;
        }
    }
open:
    co_await Event<Close>{};
    goto closed;
locked:
    co_await Event<Unlock>{};
    goto closed;
}

StateMachine getLockableDoor2()
{
    for (;;)
    {
        // closed
        auto e = co_await Event<Open, Knock, Lock>{};
        if (std::holds_alternative<Knock>(e))
        {
            shout("Come in, it's open!");
        }
        else if (std::holds_alternative<Open>(e))
        {
            // open
            co_await Event<Close>{};
        }
        else if (std::holds_alternative<Lock>(e))
        {
            // locked
            co_await Event<Unlock>{};
        }
    }
}
}  // namespace coroutines

int main()
{
    {
        using namespace switch_impl;

        std::cout << " switch\n\n";

        Door door;
        door.onEvent(Open{});   // Closed -> Open
        door.onEvent(Close{});  // Open -> Closed
        door.onEvent(Knock{});
        door.onEvent(Close{});  // Closed -> Closed
    }

    {
        using namespace templates;

        std::cout << "\n\n templates 1\n\n";
        {
            auto door = getDoor();
            door.onEvent(Open{});   // Closed -> Open
            door.onEvent(Close{});  // Open -> Closed
            door.onEvent(Knock{});
            door.onEvent(Close{});  // Closed -> Closed
        }

        std::cout << "\n\n templates 2\n\n";
        {
            auto door = getDoor2();
            door.onEvent(Open{});   // Closed -> Open
            door.onEvent(Close{});  // Open -> Closed
            door.onEvent(Knock{});
            door.onEvent(Close{});  // Closed -> Closed
        }

        std::cout << "\n\n templates 3\n\n";
        {
            auto door = getDoor3();
            door.onEvent(Open{});   // Closed -> Open
            door.onEvent(Close{});  // Open -> Closed
            door.onEvent(Knock{});
            door.onEvent(Close{});  // Closed -> Closed
        }
    }

    {
        using namespace coroutines;

        std::cout << "\n\n coroutines 1\n\n";
        {
            auto door = getDoor();
            door.onEvent(Open{});   // Closed -> Open
            door.onEvent(Close{});  // Open -> Closed
            door.onEvent(Knock{});
            door.onEvent(Close{});  // Closed -> Closed
        }

        std::cout << "\n\n coroutines 2\n\n";
        {
            auto door = getDoor2();
            door.onEvent(Open{});   // Closed -> Open
            door.onEvent(Close{});  // Open -> Closed
            door.onEvent(Knock{});
            door.onEvent(Close{});  // Closed -> Closed
        }

        std::cout << "\n\n coroutines customized\n\n";
        {
            auto door = getDoor("Occupied!");
            door.onEvent(Open{});   // Closed -> Open
            door.onEvent(Close{});  // Open -> Closed
            door.onEvent(Knock{});
            door.onEvent(Close{});  // Closed -> Closed
        }

        std::cout << "\n\n coroutines requests\n\n";
        {
            auto handler = getHandler();
            handler.onEvent(Connect{});  // Disconnected -> Connected
            handler.onEvent(Request{42});
            handler.onEvent(Disconnect{});  // Connected -> Disconnected
            handler.onEvent(Disconnect{});  // Connected -> Disconnected
        }
    }
}