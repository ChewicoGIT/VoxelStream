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
	for (int x = 0; x < _fullyLoadedChunkSize; x++) {
		FullyLoadedChunkLink& _chunkLink = fullyLoadedChunksPriority[x];
		_chunkLink._chunkData = &fullyLoadedChunks[x];

		// This is to obtain the position in the array of the chunk link
		unsigned short _chunkLinkPosition = x;
		chunks[x].useFullyLoadedChunk(_chunkLink._chunkData, _chunkLinkPosition);
		Chunk* _chunk = &chunks[x];

		_chunkLink._chunk = _chunk;

	}

	// The other ones will be optimized
	for (int x = 0; x < _optimizedChunksPrioritySize; x++) {
		optimizedChunksPriority[x] = &chunks[x + _fullyLoadedChunkSize];
		optimizedChunksPriority[x]->useOptimizedChunk(x);

	}


}

VS::ChunkMemoryManager::~ChunkMemoryManager()
{
	delete[] chunks;
	delete[] fullyLoadedChunksPriority;
	delete[] optimizedChunksPriority;
	delete[] fullyLoadedChunks;
}

VS::Chunk& VS::ChunkMemoryManager::getChunk(unsigned int id)
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

		// This wont happen because when initializing all positions are occupied
		// // We want to go to the furthest position without making a gap
		// while (_frontChunkLink._chunk == nullptr && nextPosition != 0) {
		//	  nextPosition--;
		//	  _frontChunkLink = fullyLoadedChunksPriority[nextPosition];
		// }

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

void VS::ChunkMemoryManager::debug()
{
	
	std::cout << "Fully uploaded chunks : " << dbOpt.fullyLoadedChunkBufferSize << "\n";
	for (int x = 0; x < dbOpt.fullyLoadedChunkBufferSize; x++) {

		std::cout << x + 1 << " - \t\t";

		if (fullyLoadedChunksPriority[x]._chunk == nullptr) {
			std::cout << "empty\n";
			continue;
		}

		unsigned int chunkID = fullyLoadedChunksPriority[x]._chunk - &chunks[0];

		unsigned int chunkID_Y = chunkID / (dbOpt.chunkSizeX * dbOpt.chunkSizeZ);
		unsigned int chunkID_Z = (chunkID - chunkID_Y * dbOpt.chunkSizeX * dbOpt.chunkSizeZ) / dbOpt.chunkSizeX;
		unsigned int chunkID_X = chunkID - chunkID_Y * dbOpt.chunkSizeX * dbOpt.chunkSizeZ - chunkID_Z * dbOpt.chunkSizeX;

		std::cout << "pos x: " << chunkID_X << " y: " << chunkID_Y << " z: " << chunkID_Z << "\n";

	}

}

