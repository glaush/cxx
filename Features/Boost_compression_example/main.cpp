#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    // Compress
    std::stringstream sender;
    boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
    out.push(boost::iostreams::zlib_compressor());
    out.push(sender);

    //274 symbols
    //sender << "Hello Worldewf       wef w;ociwcpwjg'perkmlksjfmpow;eljmp]ow;lgkpro;glkpr;lgkprglkrepgokrgrog3pgp3ogkpogpo2rgpopo3r3rgpkorg'pokrgpok3rgpok34g'pok3gpok3rgpok3g34g34gpok3rgpok3gpok34g3g34gkpo34gpok3g3rgrgpok3rgpo3rgpok3gpok3rgpgoknscm32872wef*&(*(&*^&%^$%#####$CFhekafhjbcbc,m";
    
    //85 symbols
    sender << "ohfhwef7832493fu9384yg02984vh^%$##%^&*()(*&^@#$%&TYUHbuygryfueifefewfwfwefwefefewewef";

    std::cout << strlen(sender.str().c_str()) << " - size of uncompressed data.\n";

    std::stringstream compressed;
    boost::iostreams::copy(out, compressed);

    std::cout << strlen(compressed.str().c_str()) << " - size of compressed data.\n";

    // Decompress
    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    in.push(boost::iostreams::zlib_decompressor());
    in.push(compressed);
    std::istream is(&in);
    std::size_t const buf_size = 256;
    char buf[buf_size]         = {'\0'};
#if 0
    is.getline(buf, buf_size); // works fine
#else
    is.read(buf, buf_size);
    std::size_t read_size = is.gcount();
    std::cout << "read_size:" << read_size << std::endl;
#endif
    std::cout << "rdstate:" << is.rdstate() << std::endl;
    std::cout << buf << std::endl;
}