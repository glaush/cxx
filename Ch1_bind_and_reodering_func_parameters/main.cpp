#include <functional>

template<typename Func, typename... BinderArgs>
struct binder
{
    binder(Func const& func, BinderArgs &&... binderArgs)
        : m_func{ func }
        , m_args{ std::forward<BinderArgs>(binderArgs)... }
    {}

    template<typename... Args>
    void operator()(Args &&... args) const
    {
        // ...
    }
    // ...
private:
    invoker_t m_invoker;
    Func m_func;
    args_list<BinderArgs...> m_args;
};

template<typename Func, typename... BinderArgs>
binder<Func, BinderArgs...> bind(Func const& func, BinderArgs &&... args)
{
	return binder<Func, BinderArgs...>(func, std::forward<BinderArgs>(args)...);
}


int main() 
{

    return 0;
}
