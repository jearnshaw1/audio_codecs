// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#ifndef WAVE_COMPONENTS_CHUNK_HPP_INCLUDED
#define WAVE_COMPONENTS_CHUNK_HPP_INCLUDED
// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G

// I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I
#include "dr_calculator/optional.h"

#include <algorithm>
#include <array>
#include <vector>
// I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I

// N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N
namespace audio_codecs {
namespace wave {
namespace components {
// N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N

struct wave
{
    master_chunk MasterChunk;

    wave()
    {
        MasterChunk.WaveId = std::array<char, 4>{};
    }

    //The arrays for CkId and CkSize might not contain char at some
    // point so would be nice to report size as lowest common denominator - bytes in this case
    const std::size_t total_size() const
    {
        return      MasterChunk.size_of_chunk_bytes()
                +   MasterChunk.CkId.size()
                +   MasterChunk.CkSize.size();
    }

};

// N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N
} // namespace components
} // namespace wave
} // namespace audio_codecs
// N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N

// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#endif // WAVE_COMPONENTS_CHUNK_HPP_INCLUDED
// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
