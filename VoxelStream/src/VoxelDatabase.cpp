#include <stdexcept>
#include "VoxelStream/VoxelDatabase.h"
#include "InternalDefinitions.h"
#include "Chunk.h"
#include "FullyLoadedChunk.h"
#include "ChunkMemoryManager.h"
#include "VoxelMemoryPaletteManager.h"

VS::VoxelDatabase::VoxelDatabase(VS::DatabaseOptions opt)
{
	dbOpt = opt;
	chunkMemory = new ChunkMemoryManager(opt);
	voxelPalette = new VoxelMemoryPaletteManager(opt);

}

VS::VoxelDatabase::~VoxelDatabase()
{
	delete chunkMemory;
	delete voxelPalette;

}

void VS::VoxelDatabase::AddVoxel(unsigned int x, unsigned int y, unsigned int z, VoxelData voxelData)
{
	// Check for bounds
#ifdef DEBUG
	if (x < 0 || x >= dbOpt.chunkSizeX * 32 ||
		y < 0 || y >= dbOpt.chunkSizeY * 32 ||
		z < 0 || z >= dbOpt.chunkSizeZ * 32)
		throw std::invalid_argument("Voxel position out of bounds");
#endif

	//Gets the id of the voxel type
	VOXEL_TYPE voxelID = voxelPalette->getVoxelID(voxelData);

	//This is the most efficient way of dividing by 32 (0b_0010_0000)
	unsigned int chunkID_X = x >> 5;
	unsigned int chunkID_Y = y >> 5;
	unsigned int chunkID_Z = z >> 5;

	unsigned int chunkID = chunkID_X + chunkID_Z * dbOpt.chunkSizeX + chunkID_Y * dbOpt.chunkSizeX * dbOpt.chunkSizeZ;

	// 31 is the same as 0b_0001_1111 that give us the modulo of 32
	unsigned int relativeX = x & 31;
	unsigned int relativeY = y & 31;
	unsigned int relativeZ = z & 31;

	Chunk& _chunk = chunkMemory->getChunk(chunkID);

}

VS::VoxelData VS::VoxelDatabase::GetVoxel(unsigned int x, unsigned int y, unsigned int z)
{
	// Check for bounds
#ifdef DEBUG
	if (x < 0 || x >= dbOpt.chunkSizeX * 32 ||
		y < 0 || y >= dbOpt.chunkSizeY * 32 ||
		z < 0 || z >= dbOpt.chunkSizeZ * 32)
		throw std::invalid_argument("Voxel position out of bounds");
#endif

	//This is a more efficient way of dividing by 32 (0b_0010_0000)
	unsigned int chunkID_X = x >> 5;
	unsigned int chunkID_Y = y >> 5;
	unsigned int chunkID_Z = z >> 5;

	unsigned int chunkID = chunkID_X + chunkID_Z * dbOpt.chunkSizeX + chunkID_Y * dbOpt.chunkSizeX * dbOpt.chunkSizeZ;

	// 31 is the same as 0b_0001_1111 that give us the modulo of 32
	unsigned int relativeX = x & 31;
	unsigned int relativeY = y & 31;
	unsigned int relativeZ = z & 31;

	Chunk& _chunk = chunkMemory->getChunk(chunkID);
	VOXEL_TYPE voxelID = _chunk.getVoxel(VOXEL_ID(relativeX, relativeY, relativeZ));

	return voxelPalette->getVoxelData(voxelID);
}
