#pragma once
#include "VoxelStream/DataStructures.h"
#include "VoxelStream/Definitions.h"

namespace VS {

	class ChunkMemoryManager
	{
	public:
		ChunkMemoryManager(DatabaseOptions _dbOpt);
		~ChunkMemoryManager();
	private:
		DatabaseOptions dbOpt;

		/// This is a list that stores the chunks(the general info about the chunk)
		Chunk* chunks;

		/// This stores the chunk priority of fully loaded chunks,
		/// the first one, are those chunks that are being used the 
		/// most and the last ones are those who are not being used
		FullyLoadedChunkLink* fullyLoadedChunksPriority;

		/// This stores the chunk priority of optimized chunks,
		/// the first one, are those chunks that are being used the 
		/// most and the last ones are those who are not being used
		Chunk** optimizedChunksPriority;

		/// Here is a list of the raw chunk buffer, this are
		/// the chunks that are used the most so for this the 
		/// chunk is fully loaded
		FullyLoadedChunk* fullyLoadedChunks;

		/// This is a count to keep track of all the empty fully
		/// loaded chunks 
		int emptyFullyLoadedChunks;

		void incrementPriority(Chunk& _chunk);

	};

}
