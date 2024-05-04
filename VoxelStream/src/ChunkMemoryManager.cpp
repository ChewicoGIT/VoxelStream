#include "ChunkMemoryManager.h"
#include "Chunk.h"
#include "FullyLoadedChunk.h"
#include "OptimizedChunk.h"
#include "InternalDefinitions.h"
#include <iostream>

VS::ChunkMemoryManager::ChunkMemoryManager(DatabaseOptions _dbOpt)
	:dbOpt(_dbOpt)
{

	unsigned int _chunkQuantity = dbOpt.chunkSizeX * dbOpt.chunkSizeY * dbOpt.chunkSizeZ;
	unsigned int _fullyLoadedChunkSize = dbOpt.fullyLoadedChunkBufferSize;
	// We need to be able to store all the chunks for the worst case
	unsigned int _optimizedChunksPrioritySize = _chunkQuantity - dbOpt.fullyLoadedChunkBufferSize;

	// Chunk array of all chunks
	chunks = new Chunk[_chunkQuantity];
	// Priority array of fully loaded
	fullyLoadedChunksPriority = new FullyLoadedChunkLink[_fullyLoadedChunkSize];
	// Priority array of optimized
	optimizedChunksPriority = new Chunk * [_optimizedChunksPrioritySize];

	// Heavy array of raw chunks data
	fullyLoadedChunks = new FullyLoadedChunk[_fullyLoadedChunkSize];

	// The first ones will be fully loaded
	for (unsigned int x = 0; x < _fullyLoadedChunkSize; x++) {
		FullyLoadedChunkLink& _chunkLink = fullyLoadedChunksPriority[x];
		_chunkLink._chunkData = &fullyLoadedChunks[x];

		// This is to obtain the position in the array of the chunk link
		ARRAY_POINTER _chunkLinkPosition = x;
		chunks[x].initFullyLoadedChunk(_chunkLinkPosition, _chunkLink._chunkData);
		Chunk* _chunk = &chunks[x];

		_chunkLink._chunk = _chunk;

	}

	// The other ones will be optimized
	for (int x = 0; x < _optimizedChunksPrioritySize; x++) {
		optimizedChunksPriority[x] = &chunks[x + _fullyLoadedChunkSize];
		optimizedChunksPriority[x]->initOptimizedChunk(x);

	}


}

VS::ChunkMemoryManager::ChunkMemoryManager(DatabaseOptions _dbOpt, OptimizedChunk* _optimizedChunk, unsigned short* priorityOrder)
	: dbOpt(_dbOpt)
{

	unsigned int _chunkQuantity = dbOpt.chunkSizeX * dbOpt.chunkSizeY * dbOpt.chunkSizeZ;
	unsigned int _fullyLoadedChunkSize = dbOpt.fullyLoadedChunkBufferSize;
	// We need to be able to store all the chunks for the worst case
	unsigned int _optimizedChunksPrioritySize = _chunkQuantity - dbOpt.fullyLoadedChunkBufferSize;

	// Chunk array of all chunks
	chunks = new Chunk[_chunkQuantity];
	// Priority array of fully loaded
	fullyLoadedChunksPriority = new FullyLoadedChunkLink[_fullyLoadedChunkSize];
	// Priority array of optimized
	optimizedChunksPriority = new Chunk * [_optimizedChunksPrioritySize];

	// Heavy array of raw chunks data
	fullyLoadedChunks = new FullyLoadedChunk[_fullyLoadedChunkSize];

	// The first ones will be fully loaded
	for (unsigned int x = 0; x < _fullyLoadedChunkSize; x++) {
		FullyLoadedChunkLink& _chunkLink = fullyLoadedChunksPriority[x];
		_chunkLink._chunkData = &fullyLoadedChunks[priorityOrder[x]];

		// This is to obtain the position in the array of the chunk link
		ARRAY_POINTER _chunkLinkPosition = x;
		chunks[x].initFullyLoadedChunk(x, _chunkLink._chunkData, &_optimizedChunk[x]);
		Chunk* _chunk = &chunks[x];

		_chunkLink._chunk = _chunk;

	}

	// The other ones will be optimized
	for (int x = 0; x < _optimizedChunksPrioritySize; x++) {
		chunks[priorityOrder[x] + _fullyLoadedChunkSize].initOptimizedChunk(x, &_optimizedChunk[x + _fullyLoadedChunkSize]);
		optimizedChunksPriority[x] = &chunks[x + _fullyLoadedChunkSize];

	}


}

VS::ChunkMemoryManager::~ChunkMemoryManager()
{
	delete[] chunks;
	delete[] fullyLoadedChunksPriority;
	delete[] optimizedChunksPriority;
	delete[] fullyLoadedChunks;
}

VS::Chunk& VS::ChunkMemoryManager::getChunk(ARRAY_POINTER id)
{
	return chunks[id];
}

void VS::ChunkMemoryManager::incrementPriority(Chunk& _chunk)
{
	ARRAY_POINTER _oldPosition;
	ARRAY_POINTER _nextPosition;

	FullyLoadedChunkLink _chunkLink;
	FullyLoadedChunkLink _frontChunkLink;
	switch (_chunk.saveState)
	{
	case ChunkSaveState::FullyLoadedChunk:
		// This means that the current position is the highest
		if (_chunk.priorityPosition == 0)
			return;

		_oldPosition = _chunk.priorityPosition;
		_nextPosition = _chunk.priorityPosition - 1;

		_chunkLink = fullyLoadedChunksPriority[_oldPosition];
		_frontChunkLink = fullyLoadedChunksPriority[_nextPosition];

		// Finally if it exists we will swap the positions
		fullyLoadedChunksPriority[_nextPosition] = _chunkLink;
		fullyLoadedChunksPriority[_oldPosition] = _frontChunkLink;

		// Update the state in the chunk class
		_chunkLink._chunk->priorityPosition = _nextPosition;
		// Maybe the chunk is empty and we could reference an non existing position
		if (_frontChunkLink._chunk != nullptr)
			_frontChunkLink._chunk->priorityPosition = _oldPosition;

		break;
	case ChunkSaveState::OptimizedChunk:
		// We want to minimize this because here is where the heavy load occurs
		if (_chunk.priorityPosition == 0) {
			// Convert the fully loaded chunk to optimized chunk
			FullyLoadedChunkLink _oldFullyLoadedLink = fullyLoadedChunksPriority[dbOpt.fullyLoadedChunkBufferSize - 1];
			Chunk* _oldOptimizedChunk = optimizedChunksPriority[0];

			_oldFullyLoadedLink._chunk->convertToOptimizedChunk(0);
			optimizedChunksPriority[0] = _oldFullyLoadedLink._chunk;

			// Convert the optimized chunk to fully loaded chunk
			fullyLoadedChunksPriority[dbOpt.fullyLoadedChunkBufferSize - 1]._chunk = _oldOptimizedChunk;
			_oldOptimizedChunk->convertToFullyLoadedChunk(dbOpt.fullyLoadedChunkBufferSize - 1,
			_oldFullyLoadedLink._chunkData);

			transformations++;
			return;
		}

		_oldPosition = _chunk.priorityPosition;
		_nextPosition = _chunk.priorityPosition - 1;

		Chunk* _chunkNextPosition = optimizedChunksPriority[_nextPosition];
		Chunk* _chunkOldPosition = optimizedChunksPriority[_oldPosition];

		optimizedChunksPriority[_oldPosition] = _chunkNextPosition;
		optimizedChunksPriority[_nextPosition] = _chunkOldPosition;

		_chunkNextPosition->priorityPosition = _oldPosition;
		_chunkOldPosition->priorityPosition = _nextPosition;

		break;
	}

}


