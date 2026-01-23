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

struct chunk
{
    std::array<char, 4> CkId;
    std::array<char, 4> CkSize;
    std::vector<chunk*> SubChunks;

    virtual std::size_t custom_size() const = 0;

    // MasterChunk.size_of_chunk_bytes() should report the CkSize of master chunk
    std::size_t size_of_chunk_bytes() const
    {
        std::size_t Total = 0;
        for( int i = 0; i < SubChunks.size(); i++ )
        {
            std::size_t SubChunkSize = SubChunks.at( i )->size_of_chunk_bytes();
            Total = Total + SubChunkSize + CkId.size() + CkSize.size();
        }

        return Total + custom_size();
    }

    template <typename ChunkT>
    void add_sub_chunk( const ChunkT& NewChunk )
    {
        SubChunks.push_back( new ChunkT(NewChunk) );
    }

    virtual ~chunk()
    {
        std::for_each( SubChunks.begin(), SubChunks.end(), []( chunk* Chunk ){ delete Chunk; } );
    }
};

struct master_chunk : public chunk
{
    optional<std::array<char, 4>> WaveId;

    virtual std::size_t custom_size() const
    {
        return WaveId ? ( *WaveId ).size() : 0;
    }
};

struct format_chunk : public chunk
{
    optional<std::array<char, 2>> Format;
    optional<std::array<char, 2>> Channels;
    optional<std::array<char, 4>> SamplesPerSec;
    optional<std::array<char, 4>> AvgBytesPerSec;
    optional<std::array<char, 2>> BlockAlign;
    optional<std::array<char, 2>> BitsPerSample;
    optional<std::array<char, 2>> CBSize;
    optional<std::array<char, 2>> ValidBitsPerSample;
    optional<std::array<char, 4>> ChannelMask;
    optional<std::array<char, 16>> SubFormat;

    virtual std::size_t custom_size() const
    {
        return      ( Format                  ? ( *Format ).size() : 0)
                +   ( Channels                ? ( *Channels ).size() : 0 )
                +   ( SamplesPerSec           ? ( *SamplesPerSec ).size() : 0 )
                +   ( AvgBytesPerSec          ? ( *AvgBytesPerSec ).size() : 0 )
                +   ( BlockAlign              ? ( *BlockAlign ).size() : 0 )
                +   ( BitsPerSample           ? ( *BitsPerSample ).size() : 0 )
                +   ( CBSize                  ? ( *CBSize ).size() : 0 )
                +   ( ValidBitsPerSample      ? ( *ValidBitsPerSample ).size() : 0 )
                +   ( ChannelMask             ? ( *ChannelMask ).size() : 0 )
                +   ( SubFormat               ? ( *SubFormat ).size() : 0 );
    }
};

struct data_chunk : public chunk
{
    std::vector<char> Samples;
    bool Padded;

    virtual std::size_t custom_size() const
    {
        return Samples.size() + ( Padded ? 1 : 0 );
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
