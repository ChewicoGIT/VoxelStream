#include "Chunk.h"
#include "FullyLoadedChunk.h"
#include "OptimizedChunk.h"
#include <iostream>

VOXEL_TYPE VS::Chunk::getVoxel(unsigned short voxelID)
{

	switch (saveState)
	{
		// If it is a fully loaded chunk is easy to retrive to
	case VS::ChunkSaveState::FullyLoadedChunk:
		return fullyLoadedChunk->voxelData[voxelID];

	case VS::ChunkSaveState::OptimizedChunk:
		return optimizedChunk->getVoxel(voxelID);

	}

	return 0;
}

void VS::Chunk::modifyVoxel(unsigned short voxelID, VOXEL_TYPE _voxel)
{
	switch (saveState)
	{
		// If it is a fully loaded chunk is easy to obtain
	case VS::ChunkSaveState::FullyLoadedChunk:
		fullyLoadedChunk->voxelData[voxelID] = _voxel;
		break;

	case VS::ChunkSaveState::OptimizedChunk:
		optimizedChunk->setVoxel(voxelID, _voxel);
		break;

	}
}

void VS::Chunk::initAsOptimizedChunk(BIG_ARRAY_POINTER _priorityPosition, OptimizedChunk* _optimizedChunk)
{
	optimizedChunk = new OptimizedChunk(*_optimizedChunk);
	priorityPosition = _priorityPosition;
	saveState = ChunkSaveState::OptimizedChunk;
}

void VS::Chunk::useFullyLoadedChunk(FullyLoadedChunk* _fullyLoadedChunk, BIG_ARRAY_POINTER _priorityPosition)
{
	fullyLoadedChunk = _fullyLoadedChunk;

	priorityPosition = _priorityPosition;
	saveState = ChunkSaveState::FullyLoadedChunk;
	
}

void VS::Chunk::useOptimizedChunk(BIG_ARRAY_POINTER _priorityPosition)
{
	priorityPosition = _priorityPosition;
	optimizedChunk = new OptimizedChunk();
	saveState = ChunkSaveState::OptimizedChunk;
}

void VS::Chunk::convertToOptimizedChunk(BIG_ARRAY_POINTER _priorityPosition)
{
	priorityPosition = _priorityPosition;
	optimizedChunk = new OptimizedChunk(*fullyLoadedChunk);
	saveState = ChunkSaveState::OptimizedChunk;

}

void VS::Chunk::convertToFullyLoadedChunk(BIG_ARRAY_POINTER _priorityPosition, FullyLoadedChunk* data)
{
	fullyLoadedChunk = data;
	priorityPosition = _priorityPosition;
	saveState = ChunkSaveState::FullyLoadedChunk;

	data->load(optimizedChunk);
	delete optimizedChunk;

}

VS::Chunk::Chunk(BIG_ARRAY_POINTER _priorityPosition, OptimizedChunk* _optimizedChunk)
{
	priorityPosition = 69;
	optimizedChunk = new OptimizedChunk(*_optimizedChunk);
	saveState = ChunkSaveState::OptimizedChunk;
}

VS::Chunk::~Chunk()
{
	if (saveState == ChunkSaveState::OptimizedChunk)
		delete optimizedChunk;
}

