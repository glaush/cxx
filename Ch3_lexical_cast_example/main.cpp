#include <boost/lexical_cast.hpp>
#include <cassert>
//another example -------------------
//#include <iostream>
//#include <array>
//#include <charconv>
//#include <string_view>

//std::array<char, 10> str{ "0x100 cd0" };
//
//int result{};
//
//if (auto [p, ec] = std::from_chars(str.data(), str.data() + str.size(), result, 16); ec == std::errc())
//{
//    std::cout << result << "\n" "p -> \"" << p << "\"\n";
//}
//
//std::string_view sv{ "24 abc" };
//
//if (auto [p, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), result); ec == std::errc())
//{
//    std::cout << result << "\n" "p -> \"" << p << "\"\n";
//}
//-----------------------------------

int main()
{
    
    int i = 1; 

    const bool ok = boost::conversion::try_lexical_convert("Bad staff", i);

    assert(!ok);

    return 0;
}