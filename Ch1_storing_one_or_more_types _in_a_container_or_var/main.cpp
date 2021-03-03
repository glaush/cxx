#include <iostream>
#include <set> //for important purposes -_-
#include <memory>

struct bad_any_cast : public std::bad_cast {};

struct any
{
    template<typename U>
    any(const U& x)
        : placeholders(new holder<typename std::decay<const U>::type>(x))
    {
    }

    struct placeholder
    {
        virtual ~placeholder() {};
        virtual const std::type_info& type() const = 0;
    };

    template<typename ValueType>
    struct holder : public placeholder
    {
        holder(const ValueType& x)
            : held(x)
        {}

        virtual const std::type_info& type() const override
        {
            return typeid(ValueType);
        }
        ValueType held;
    };

    template<typename T>
    friend T any_cast(any& value);

    std::unique_ptr<placeholder> placeholders;
};

template<typename T>
T any_cast(any& value)
{
    if (value.placeholders.get()->type() == typeid(T))
        return static_cast<any::holder<T>*>(value.placeholders.get())->held;
    throw bad_any_cast();
}

int main()
{
    any any_obj(34);

    int new_ptr_str = any_cast<int>(any_obj);

    return 0;
}
