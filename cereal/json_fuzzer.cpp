#include <cassert>
#include <cstdint>

#include <cereal/archives/json.hpp>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    // TODO: Serialize to JSON
    // TODO: De-serialize from JSON
    // TODO: Check for equivalency, fail otherwise.
    return 0;
}

