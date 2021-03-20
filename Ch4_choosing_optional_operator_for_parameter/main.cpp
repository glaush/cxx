#include <iostream>
#include <boost/type_traits/conditional.hpp>

#include <boost/type_traits/has_plus.hpp>
#include <boost/type_traits/has_plus_assign.hpp>
#include <boost/type_traits/has_post_increment.hpp>
#include <boost/type_traits/has_pre_increment.hpp>


namespace detail
{
    struct pre_inc_functor
    {
        template< class T>
        void operator()(T& value) const
        {
            ++value;
        }
    };

    struct post_inc_functor
    {
        template< class T>
        void operator()(T& value) const
        {
            value++;
        }
    };

    struct pluss_assignable_functor
    {
        template< class T>
        void operator()(T& value) const
        {
            value += T(1);
        }
    };

    struct plus_functor
    {
        template< class T>
        void operator()(T& value) const
        {
            value = value + T(1);
        }
    };
}

template<class T>
void inc(T& value)
{
    typedef detail::plus_functor step_0_t;

    typedef typename boost::conditional
        <boost::has_plus_assign<T>::value, 
        detail::pluss_assignable_functor, 
        step_0_t
        >::type step_1_t;

    typedef typename boost::conditional
        <boost::has_post_increment<T>::value,
        detail::post_inc_functor,
        step_1_t
        >::type step_2_t;

    typedef typename boost::conditional
        <boost::has_pre_increment<T>::value,
        detail::pre_inc_functor,
        step_2_t
        >::type step_3_t;

    step_3_t()(value);

    std::cout << value << std::endl;
}

int main()
{
    int k = 3;

    inc(k);

    return 0;
}