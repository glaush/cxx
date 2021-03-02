#include <iostream>
#include <boost/shared_ptr.hpp>
#include <cassert>
#include <boost/make_shared.hpp>

struct base
{
    virtual void some_methods() = 0;
    virtual ~base() {}
};

struct derived : public base
{
    int id;
    derived(int id = 0) {}

    void some_methods() {}
    virtual void derived_method() const;
    
    ~derived() {}
};

void derived::derived_method() const
{
    std::cout << "I'm derived" << std::endl;
}

void im_accepting_derived(boost::shared_ptr<const derived> p)
{
    assert(p);
}

boost::shared_ptr<const base> construct_derived()
{
    return boost::make_shared<derived>();
}

void trying_hard_to_pass_derived()
{
    boost::shared_ptr<const base> d = construct_derived();
}

void trying_hard_to_pass_derived2()
{
    boost::shared_ptr<const derived> d = boost::dynamic_pointer_cast<const derived>(
        construct_derived()
        );
    
    if (!d)
    {
        throw std::runtime_error("Failed to dynamic cast");
    }
    
    d->derived_method();
    im_accepting_derived(d);
}

int main()
{
    trying_hard_to_pass_derived2();
    return 0;
}
