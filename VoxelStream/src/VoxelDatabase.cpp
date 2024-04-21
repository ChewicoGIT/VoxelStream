#include "VoxelStream/VoxelDatabase.h"
#include "InternalDefinitions.h"
#include "Chunk.h"
#include "FullyLoadedChunk.h"

VS::VoxelDatabase::VoxelDatabase(DatabaseOptions opt)
{
	dbOpt = opt;

	unsigned int _chunkQuantity = dbOpt.chunkSizeX * dbOpt.chunkSizeY * dbOpt.chunkSizeZ;
	unsigned int _fullyLoadedChunkSize = dbOpt.fullyLoadedChunkBufferSize;
	//We need to be able to store all the chunks for the worst case
	unsigned int _optimizedChunksPrioritySize = _chunkQuantity - dbOpt.fullyLoadedChunkBufferSize;

	//Chunk array of all chunks
	chunks = new Chunk[_chunkQuantity];
	//Priority array of fully loaded
	fullyLoadedChunksPriority = new FullyLoadedChunkLink [_fullyLoadedChunkSize];
	//Priority array of optimized
	optimizedChunksPriority = new Chunk*[_optimizedChunksPrioritySize];


	//Heavy array of raw chunks data
	fullyLoadedChunks = new FullyLoadedChunk[_fullyLoadedChunkSize];

}

void VS::VoxelDatabase::incrementPriority(Chunk& _chunk)
{
	switch (_chunk.saveState)
	{
	case ChunkSaveState::FullyLoadedChunk:
		// This means that the current position is the highest
		if (_chunk.priorityPosition == 0)
			return;

		ARRAY_POINTER _oldPriorityPosition = _chunk.priorityPosition;
		ARRAY_POINTER nextPosition = _chunk.priorityPosition - 1;

		FullyLoadedChunkLink _chunkLink = fullyLoadedChunksPriority[_oldPriorityPosition];
		FullyLoadedChunkLink _frontChunkLink = fullyLoadedChunksPriority[nextPosition];

		//We want to go to the furthest position without making a gap
		while (_frontChunkLink._chunk == nullptr && nextPosition != 0) {
			nextPosition--;
			_frontChunkLink = fullyLoadedChunksPriority[nextPosition];
		}

		// Finally if it exists we will swap the positions
		fullyLoadedChunksPriority[nextPosition] = _chunkLink;
		fullyLoadedChunksPriority[_oldPriorityPosition] = _frontChunkLink;

		// Update the state in the chunk class
		_chunkLink._chunk->priorityPosition = nextPosition;
		// Maybe the chunk is empty and we could reference an non existing position
		if(_frontChunkLink._chunk != nullptr)
			_frontChunkLink._chunk->priorityPosition = _oldPriorityPosition;

		break;

	case ChunkSaveState::CompressedChunk:

		// This means that the cunk has to swap with a fully loaded,
		// here is the dangerous part where we will exchange big chunks of data
		// so we want to minimize this call
		if (_chunk.priorityPosition == 0) {
			//To do..

		}

		//To do...

		return;
	}

}
