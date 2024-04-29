#include <stdexcept>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/cereal.hpp>
#include <fstream>

#include "VoxelStream/VoxelDatabase.h"
#include "InternalDefinitions.h"
#include "Serialization/VoxelDatabaseData.h"

#include "Chunk.h"
#include "FullyLoadedChunk.h"
#include "ChunkMemoryManager.h"
#include "VoxelMemoryPaletteManager.h"

VS::VoxelDatabase::VoxelDatabase(VS::DatabaseOptions opt)
	: dbOpt(opt)
{
	chunkMemory = new ChunkMemoryManager(opt);
	voxelPalette = new VoxelMemoryPaletteManager(opt);

}

VS::VoxelDatabase::VoxelDatabase(const char* fileLoaction)
{
	VoxelDatabaseData _databaseData;

	{
		std::ifstream fileStream(fileLoaction, std::ios::binary);
		cereal::PortableBinaryInputArchive _outputArchive(fileStream);
		_outputArchive(_databaseData);

	}

	dbOpt = _databaseData.databaseOptions;
	voxelPalette = _databaseData.paletteData;
	chunkMemory = new ChunkMemoryManager(dbOpt, _databaseData.optimizedChunks);

	delete[] _databaseData.optimizedChunks;
}

VS::VoxelDatabase::~VoxelDatabase()
{
	delete chunkMemory;
	delete voxelPalette;

}

void VS::VoxelDatabase::SetVoxel(unsigned int x, unsigned int y, unsigned int z, VoxelData voxelData)
{
	// Check for bounds
#ifdef DEBUG
	if (x < 0 || x >= dbOpt.chunkSizeX * 32 ||
		y < 0 || y >= dbOpt.chunkSizeY * 32 ||
		z < 0 || z >= dbOpt.chunkSizeZ * 32)
		throw std::invalid_argument("Voxel position out of bounds");
#endif

	//Gets the id of the voxel type
	VOXEL_TYPE _voxel = voxelPalette->getVoxelID(voxelData);

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

	// Calculate a number to increase priority, if the chunk is near the 0 it is not
	// necessary a big amount but if it is in the last pos it needs a lot
	int _chunkAbsolutePosition = _chunk.priorityPosition + (_chunk.saveState == ChunkSaveState::FullyLoadedChunk) ? dbOpt.fullyLoadedChunkBufferSize : 0;
	int incrementPriority = (_chunkAbsolutePosition * dbOpt.modifiedVoxelPriorityValue / (dbOpt.chunkSizeX * dbOpt.chunkSizeY * dbOpt.chunkSizeZ)) + 1;

	for (int x = 0; x < incrementPriority; x++)
		chunkMemory->incrementPriority(_chunk);

	if (_chunk.saveState == ChunkSaveState::FullyLoadedChunk)
		_objectWasFullyLoaded++;

	_chunk.modifyVoxel(VOXEL_ID(relativeX, relativeY, relativeZ), _voxel);

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

	// Calculate a number to increase priority, if the chunk is near the 0 it is not
	// necessary a big amount but if it is in the last pos it needs a lot
	int _chunkAbsolutePosition = _chunk.priorityPosition + (_chunk.saveState == ChunkSaveState::FullyLoadedChunk) ? dbOpt.fullyLoadedChunkBufferSize : 0;
	int incrementPriority = (_chunkAbsolutePosition * dbOpt.queryedVoxelPriorityValue / (dbOpt.chunkSizeX * dbOpt.chunkSizeY * dbOpt.chunkSizeZ)) + 1;

	for (int x = 0; x < incrementPriority; x++)
		chunkMemory->incrementPriority(_chunk);

	if (_chunk.saveState == ChunkSaveState::FullyLoadedChunk)
		_objectWasFullyLoaded++;

	VOXEL_TYPE voxelID = _chunk.getVoxel(VOXEL_ID(relativeX, relativeY, relativeZ));


	return voxelPalette->getVoxelData(voxelID);
}

void VS::VoxelDatabase::saveData(const char* fileLocation)
{
	const int chunkCount = dbOpt.chunkSizeX * dbOpt.chunkSizeY * dbOpt.chunkSizeZ;

	// Constructing
	VoxelDatabaseData _data{
		.databaseOptions = dbOpt,
		.optimizedChunks = nullptr,
		.paletteData = voxelPalette
	};

	_data.optimizedChunks = new OptimizedChunk[chunkCount]();

	for (int chunkID = 0; chunkID < chunkCount; chunkID++) {
		Chunk& _workingChunk = chunkMemory->getChunk(chunkID);
		switch (_workingChunk.saveState)
		{
		case ChunkSaveState::FullyLoadedChunk:
			_data.optimizedChunks[chunkID] = OptimizedChunk(*_workingChunk.fullyLoadedChunk);
			break;

		case ChunkSaveState::OptimizedChunk:
			_data.optimizedChunks[chunkID] = OptimizedChunk(*_workingChunk.optimizedChunk);
			break;
		}
	}
	
	{
		std::ofstream file(fileLocation, std::ios::binary);
		cereal::PortableBinaryOutputArchive _archive(file);

		_archive(_data);
	}

	delete[] _data.optimizedChunks;
}

int VS::VoxelDatabase::getTransformations()
{
	int tempValue = chunkMemory->transformations;
	chunkMemory->transformations = 0;
	return tempValue;
}

int VS::VoxelDatabase::objectWasFullyLoaded()
{
	int tempValue = _objectWasFullyLoaded;
	_objectWasFullyLoaded = 0;
	return tempValue;
}
