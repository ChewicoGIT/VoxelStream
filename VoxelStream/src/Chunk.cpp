#include "Chunk.h"
#include "FullyLoadedChunk.h"
#include "OptimizedChunk.h"
#include <iostream>

vs::VoxelData vs::Chunk::getVoxel(unsigned short voxelID)
{

	switch (saveState)
	{
		// If it is a fully loaded chunk is easy to retrive to
	case vs::ChunkSaveState::FullyLoadedChunk:
		return fullyLoadedChunk->voxelData[voxelID];

	case vs::ChunkSaveState::OptimizedChunk:
		return optimizedChunk->getVoxel(voxelID);

	}

	return vs::VoxelData();
}

void vs::Chunk::setVoxel(unsigned short voxelID, vs::VoxelData _voxel)
{
	switch (saveState)
	{
		// If it is a fully loaded chunk is easy to obtain
	case vs::ChunkSaveState::FullyLoadedChunk:
		fullyLoadedChunk->voxelData[voxelID] = _voxel;
		break;

	case vs::ChunkSaveState::OptimizedChunk:
		optimizedChunk->setVoxel(voxelID, _voxel);
		break;

	}
}

void vs::Chunk::initOptimizedChunk(ARRAY_POINTER _priorityPosition, OptimizedChunk* _optimizedChunk)
{
	optimizedChunk = new OptimizedChunk(*_optimizedChunk);
	priorityPosition = _priorityPosition;
	saveState = ChunkSaveState::OptimizedChunk;
}

void vs::Chunk::initFullyLoadedChunk(ARRAY_POINTER _priorityPosition, FullyLoadedChunk* _fullyLoadedChunk)
{
	fullyLoadedChunk = _fullyLoadedChunk;

	priorityPosition = _priorityPosition;
	saveState = ChunkSaveState::FullyLoadedChunk;
	
}

void vs::Chunk::initFullyLoadedChunk(ARRAY_POINTER _priorityPosition, FullyLoadedChunk* _fullyLoadedChunk, OptimizedChunk* _optimizedChunk)
{
	saveState = ChunkSaveState::FullyLoadedChunk;
	fullyLoadedChunk = _fullyLoadedChunk;
	priorityPosition = _priorityPosition;

	_fullyLoadedChunk->load(_optimizedChunk);

}

void vs::Chunk::initOptimizedChunk(ARRAY_POINTER _priorityPosition)
{
	priorityPosition = _priorityPosition;
	optimizedChunk = new OptimizedChunk();
	saveState = ChunkSaveState::OptimizedChunk;
}

void vs::Chunk::convertToOptimizedChunk(ARRAY_POINTER _priorityPosition)
{
	priorityPosition = _priorityPosition;
	optimizedChunk = new OptimizedChunk(*fullyLoadedChunk);
	saveState = ChunkSaveState::OptimizedChunk;

}

void vs::Chunk::convertToFullyLoadedChunk(ARRAY_POINTER _priorityPosition, FullyLoadedChunk* data)
{
	fullyLoadedChunk = data;
	priorityPosition = _priorityPosition;
	saveState = ChunkSaveState::FullyLoadedChunk;

	data->load(optimizedChunk);
	delete optimizedChunk;

}

vs::Chunk::~Chunk()
{
	if (saveState == ChunkSaveState::OptimizedChunk)
		delete optimizedChunk;
}

