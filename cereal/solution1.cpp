#include <cassert>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <vector>

#include <cereal/archives/json.hpp>

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("usage: %s TEST\n", argv[0]);
        return 1;
    }

    std::ifstream ifs(argv[1], std::ios::binary);
    std::istreambuf_iterator<char> begin{ifs}, end;
    std::string s(begin, end);

    std::stringstream ss;

    {
        cereal::JSONOutputArchive oa(ss);
        oa(s);
    }
    

    return 0;
}

