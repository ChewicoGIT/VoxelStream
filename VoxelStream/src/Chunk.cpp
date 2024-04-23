#include "Chunk.h"
#include "FullyLoadedChunk.h"
#include "OptimizedChunk.h"

VOXEL_TYPE VS::Chunk::getVoxel(unsigned short voxelID)
{

	switch (saveState)
	{
		// If it is a fully loaded chunk is easy to retrive to
	case VS::ChunkSaveState::FullyLoadedChunk:
		return fullyLoadedChunk->voxelData[voxelID];

	case VS::ChunkSaveState::CompressedChunk:
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

	case VS::ChunkSaveState::CompressedChunk:
		optimizedChunk->setVoxel(voxelID, _voxel);
		break;

	}
}

VS::Chunk::Chunk()
{
	saveState = ChunkSaveState::CompressedChunk;
	optimizedChunk = new OptimizedChunk();

}

VS::Chunk::~Chunk()
{
	delete optimizedChunk;
}

