#include <cassert>
#include <cstdint>

#include <cereal/archives/json.hpp>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    std::string s(data, data + size);

    std::stringstream ss;

    std::string s2;

    // Note: we have to wrap writing to the output archive because the data
    // isn't flushed to stringstream until it goes out of scope.
    {
        cereal::JSONOutputArchive oa(ss);
        oa(s);
    }

    cereal::JSONInputArchive ia(ss);
    ia(s2);

    assert(s == s2);

    return 0;
}

