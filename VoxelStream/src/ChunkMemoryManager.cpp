#include <iostream>
#include "ChunkMemoryManager.h"
#include "Chunk.h"
#include "FullyLoadedChunk.h"
#include "InternalDefinitions.h"

VS::ChunkMemoryManager::ChunkMemoryManager(DatabaseOptions _dbOpt)
{
	dbOpt = _dbOpt;

	unsigned int _chunkQuantity = dbOpt.chunkSizeX * dbOpt.chunkSizeY * dbOpt.chunkSizeZ;
	unsigned int _fullyLoadedChunkSize = dbOpt.fullyLoadedChunkBufferSize;
	//We need to be able to store all the chunks for the worst case
	unsigned int _optimizedChunksPrioritySize = _chunkQuantity - dbOpt.fullyLoadedChunkBufferSize;

	//Chunk array of all chunks
	chunks = new Chunk[_chunkQuantity];
	//Priority array of fully loaded
	fullyLoadedChunksPriority = new FullyLoadedChunkLink[_fullyLoadedChunkSize];
	//Priority array of optimized
	optimizedChunksPriority = new Chunk * [_optimizedChunksPrioritySize];

	//Heavy array of raw chunks data
	fullyLoadedChunks = new FullyLoadedChunk[_fullyLoadedChunkSize];
}

VS::ChunkMemoryManager::~ChunkMemoryManager()
{
	delete fullyLoadedChunksPriority;
	delete optimizedChunksPriority;
	delete fullyLoadedChunks;
	delete chunks;
}

VS::Chunk& VS::ChunkMemoryManager::getChunk(unsigned int id)
{
	return chunks[id];
}

void VS::ChunkMemoryManager::incrementPriority(Chunk& _chunk)
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
		if (_frontChunkLink._chunk != nullptr)
			_frontChunkLink._chunk->priorityPosition = _oldPriorityPosition;

		break;

	}

}

void VS::ChunkMemoryManager::debug()
{
	
	std::cout << "Fully uploaded chunks : " << dbOpt.fullyLoadedChunkBufferSize << "\n";
	for (int x = 0; x < dbOpt.fullyLoadedChunkBufferSize; x++) {

		std::cout << x + 1 << " - ";

		if (fullyLoadedChunksPriority[x]._chunk == nullptr) {
			std::cout << "empty\n";
			continue;
		}

		unsigned int chunkID = fullyLoadedChunksPriority[x]._chunk - chunks;
		chunkID /= sizeof(Chunk*);

		unsigned int chunkID_Y = chunkID / (dbOpt.chunkSizeX * dbOpt.chunkSizeZ);
		unsigned int chunkID_Z = (chunkID - chunkID_Y * dbOpt.chunkSizeX * dbOpt.chunkSizeZ) / dbOpt.chunkSizeX;
		unsigned int chunkID_X = chunkID - chunkID_Y * dbOpt.chunkSizeX * dbOpt.chunkSizeZ - chunkID_Z * dbOpt.chunkSizeX;

		std::cout << "pos x: " << chunkID_X << " y: " << chunkID_Y << " z: " << chunkID_Z << "\n";

	}

}
