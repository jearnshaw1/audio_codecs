// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#ifndef WAVE_DECODE_HPP_INCLUDED
#define WAVE_DECODE_HPP_INCLUDED
// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G

// I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I
#include "wave/components/chunk.h"
#include "wave/components/wave.h"

#include <fstream>

#include <exception>
#include <iostream>
#include <cstddef>
#include <memory>
// I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I

// N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N
namespace audio_codecs {
namespace wave {
// N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N

	struct wave_load_err : public std::exception
	{};

	class wave_loader
	{
	public:

		wave load( const char* FileName )
		{
			wave Wave;
			int FilePos = 0;

			std::ifstream FileObject;

			FileObject.open( FileName, std::ifstream::binary | std::ifstream::ate );
			if ( FileObject.fail() || !FileObject.is_open() )
				throw wave_load_err();

			const int FileLength = FileObject.tellg();

			FileObject.seekg( FilePos );
			FileObject.read( Wave.MasterChunk.CkId.data(), Wave.MasterChunk.CkId.size() );

			FilePos += Wave.MasterChunk.CkId.size();

			std::cout << "Value of master chunk id = ";
			for( const auto& b : Wave.MasterChunk.CkId )
			{
				std::cout << b;
			}
			std::cout << "\n";

			FileObject.seekg( FilePos );
			FileObject.read( Wave.MasterChunk.CkSize.data(), Wave.MasterChunk.CkId.size() );

			FilePos += Wave.MasterChunk.CkId.size();

			std::cout << "Value of master chunk size = ";
			std::cout << *( reinterpret_cast<std::uint32_t*>( Wave.MasterChunk.CkSize.data() ) );
			std::cout << "\n";

			FileObject.seekg( FilePos );
			FileObject.read( Wave.MasterChunk.WaveId.get().data(), Wave.MasterChunk.WaveId.get().size() );

			FilePos += ( *Wave.MasterChunk.WaveId ).size();

			std::cout << "Value of WaveId = ";
			for( const auto& b : *Wave.MasterChunk.WaveId  )
			{
				std::cout << b;
			}
			std::cout << "\n";

			Wave.MasterChunk.add_sub_chunk( format_chunk{} );

			format_chunk& FormatChunk = *( static_cast<format_chunk*>( Wave.MasterChunk.SubChunks.at( 0 ) ) );

			FileObject.seekg( FilePos );
			FileObject.read( FormatChunk.CkId.data(), FormatChunk.CkId.size() );

			FilePos += FormatChunk.CkId.size();

			std::cout << "Value of format chunk id = ";
			for( const auto& b : FormatChunk.CkId  )
			{
				std::cout << b;
			}
			std::cout << "\n";

			FileObject.seekg( FilePos );
			FileObject.read( FormatChunk.CkSize.data(), FormatChunk.CkSize.size() );

			FilePos += FormatChunk.CkSize.size();

			std::cout << "Value of format chunk size = ";
			std::cout << *( reinterpret_cast<std::uint32_t*>( FormatChunk.CkSize.data() ) );
			std::cout << "\n";


			const std::size_t FormatDataStartPos = FilePos;
			const std::size_t FormatChunkSize = *( reinterpret_cast<std::uint32_t*>( FormatChunk.CkSize.data() ) );

			if(  FilePos  <= ( FormatDataStartPos + FormatChunkSize ) )
			{
				FormatChunk.Format = std::array<char, 2>{};
				FileObject.seekg( FilePos );
				FileObject.read( FormatChunk.Format.get().data(), FormatChunk.Format.get().size() );

				FilePos += ( *FormatChunk.Format ).size();

				std::cout << "Value of wav format = ";
				std::cout << *( reinterpret_cast<std::uint16_t*>( ( *FormatChunk.Format ).data() ) );
				std::cout << "\n";
			}
			if(  FilePos  <= ( FormatDataStartPos + FormatChunkSize ) )
			{
				FormatChunk.Channels = std::array<char, 2>{};
				FileObject.seekg( FilePos );
				FileObject.read( FormatChunk.Channels.get().data(), FormatChunk.Channels.get().size() );

				FilePos += ( *FormatChunk.Channels ).size();

				std::cout << "Value of channels = ";
				std::cout << *( reinterpret_cast<std::uint16_t*>( ( *FormatChunk.Channels ).data() ) );
				std::cout << "\n";
			}
			if(  FilePos  <= ( FormatDataStartPos + FormatChunkSize ) )
			{
				FormatChunk.SamplesPerSec = std::array<char, 4>{};
				FileObject.seekg( FilePos );
				FileObject.read( FormatChunk.SamplesPerSec.get().data(), FormatChunk.SamplesPerSec.get().size() );

				FilePos += ( *FormatChunk.SamplesPerSec ).size();

				std::cout << "Value of samplesPerSec = ";
				std::cout << *( reinterpret_cast<std::uint32_t*>( ( *FormatChunk.SamplesPerSec ).data() ) );
				std::cout << "\n";
			}
			if(  FilePos  <= ( FormatDataStartPos + FormatChunkSize ) )
			{
				FormatChunk.AvgBytesPerSec = std::array<char, 4>{};
				FileObject.seekg( FilePos );
				FileObject.read( FormatChunk.AvgBytesPerSec.get().data(), FormatChunk.AvgBytesPerSec.get().size() );

				FilePos += ( *FormatChunk.AvgBytesPerSec ).size();

				std::cout << "Value of bytes per sec = ";
				std::cout << *( reinterpret_cast<std::uint32_t*>( ( *FormatChunk.AvgBytesPerSec ).data() ) );
				std::cout << "\n";
			}
			if(  FilePos  <= ( FormatDataStartPos + FormatChunkSize ) )
			{
				FormatChunk.BlockAlign = std::array<char, 2>{};
				FileObject.seekg( FilePos );
				FileObject.read( FormatChunk.BlockAlign.get().data(), FormatChunk.BlockAlign.get().size() );

				FilePos += ( *FormatChunk.BlockAlign ).size();

				std::cout << "Value of blockalign = ";
				std::cout << *( reinterpret_cast<std::uint16_t*>( ( *FormatChunk.BlockAlign ).data() ) );
				std::cout << "\n";
			}
			if(  FilePos  <= ( FormatDataStartPos + FormatChunkSize ) )
			{
				FormatChunk.BitsPerSample = std::array<char, 2>{};
				FileObject.seekg( FilePos );
				FileObject.read( FormatChunk.BitsPerSample.get().data(), FormatChunk.BitsPerSample.get().size() );

				FilePos += ( *FormatChunk.BitsPerSample ).size();

				std::cout << "Value of BitsPerSample = ";
				std::cout << *( reinterpret_cast<std::uint16_t*>( ( *FormatChunk.BitsPerSample ).data() ) );
				std::cout << "\n";
			}

			// END OF 16 byte format chunk

			// FileObject.seekg( FilePos );
			// FileObject.read( FormatChunk->CBSize.data(), FormatChunk->SamplesPerSec.size() );

			// FilePos += FormatChunk->SamplesPerSec.size();

			// std::cout << "Value of samplesPerSec = ";
			// std::cout << *( reinterpret_cast<std::uint32_t*>( FormatChunk->SamplesPerSec.data() ) );
			// std::cout << "\n";

			Wave.MasterChunk.add_sub_chunk( data_chunk{} );

			data_chunk& DataChunk = *( static_cast<data_chunk*>( Wave.MasterChunk.SubChunks.at( 1 ) ) );

			FileObject.seekg( FilePos );
			FileObject.read( DataChunk.CkId.data(), DataChunk.CkId.size() );

			FilePos += DataChunk.CkId.size();

			std::cout << "Value of datachunk id = ";
			for( const auto& b : DataChunk.CkId )
			{
				std::cout << b;
			}
			std::cout << "\n";

			FileObject.seekg( FilePos );
			FileObject.read( DataChunk.CkSize.data(), DataChunk.CkSize.size() );

			FilePos += DataChunk.CkSize.size();

			std::cout << "Value of data chunk size = ";
			std::cout << *( reinterpret_cast<std::uint32_t*>( DataChunk.CkSize.data() ) );
			std::cout << "\n";

			std::size_t DataChunkSize = *( reinterpret_cast<std::uint32_t*>( DataChunk.CkSize.data() ) );


			FileObject.seekg( FilePos );
			load_samples( FileObject, DataChunkSize, Wave );

			std::cout << "Size of sample data bytes = ";
			std::cout << DataChunk.Samples.size();
			std::cout << "\n";

			std::cout << "Total size of wave = ";
			std::cout << Wave.total_size();
			std::cout << "\n";

			std::cout << "Total size from filestream = ";

			FileObject.close();
			FileObject.open( FileName, std::ifstream::binary );

			if ( FileObject.fail() || !FileObject.is_open() )
				throw wave_load_err();

			std::cout << FileLength;
			std::cout << "\n";


			std::cout << "Size of tail = " << (FileLength - (FilePos + DataChunkSize ) ) << "\n";
			// char Tail[ FileLength - ( FilePos + DataChunkSize ) ];
			// FileObject.seekg( FilePos + DataChunkSize );
			// FileObject.read( Tail, FileLength - ( FilePos + DataChunkSize ) );

			// std::cout << "Tail bytes = ";
			// for( const auto& b : Tail )
			// {
			// 	std::cout << b;
			// }

			FileObject.close();

			return Wave;

		// data = new char[36];
		// // Big endian is fine , little end. not interpreted correctly
		// //ASSERT(sizeof(char) == 1);
		// fileObject.read(data, 36);

		// // bits per sample segment is little end;
		// if (data[34] == 0x10)
		// {
		// 	bitsPerSample = 16;
		// }
		// else if (data[34] == 0x18)
		// {
		// 	bitsPerSample = 24;
		// }

		// else if (data[34] == 0x20)
		// {
		// 	bitsPerSample = 32;
		// }
		// else
		// {
		// 	std::cout << "The bit depth is " << data[34] << "and the file cannot be read\n";


		// }
		// dataOffsetBytes = 44;
		// int numberSampleBytes{ data[40] };
		// samples = new char[numberSampleBytes];

		// fileObject.seekg(44, std::ios_base::beg);
		// fileObject.read(samples, numberSampleBytes);

		// numSamples = numberSampleBytes / (2 * bitsPerSample);
		};

	private:
		void load_samples( std::ifstream& FileObject, std::size_t SampleBytes, wave& Wave )
		{
			data_chunk& DataChunk = *( static_cast<data_chunk*>( Wave.MasterChunk.SubChunks.at( 1 ) ) );

			if( SampleBytes % 2 )
			{
				DataChunk.Padded = true;
			}

			while( SampleBytes-- )
			{
				DataChunk.Samples.push_back( FileObject.get() );
			}
		}
	};

// N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N
} // namespace wave
} // namespace audio_codecs
// N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N N

// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#endif // WAVE_DECODE_HPP_INCLUDED
// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
