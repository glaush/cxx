#include <boost/scoped_ptr.hpp>
#include <iostream>

class foo_class
{
public:

    foo_class() = default;
    foo_class(const char* str)
    {
       arr = str;
    }

    ~foo_class() = default;

private:
    const char* arr;
};

bool some_func1(foo_class& pointer)
{
    throw std::bad_exception();

    return false;
}

bool some_func2(foo_class* reference)
{
    return false;
}


bool foo3()
{
    const boost::scoped_ptr<foo_class> p(new foo_class("Some data"));

    const bool something_else_happend = some_func1(*p);

    if (something_else_happend)
    {
        return false;
    }
    
    some_func2(p.get());

    return true;
}

int main()
{
    foo3();

    return 0;
}