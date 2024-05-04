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

VS::VoxelDatabase::VoxelDatabase(VS::DatabaseOptions opt)
	: dbOpt(opt)
{
	chunkMemory = new ChunkMemoryManager(opt);

}

VS::VoxelDatabase::VoxelDatabase(const char* fileLoaction)
{
	VoxelDatabaseData _data;

	{
		std::ifstream fileStream(fileLoaction, std::ios::binary);
		cereal::PortableBinaryInputArchive _outputArchive(fileStream);
		_outputArchive(_data);

	}

	dbOpt = _data.databaseOptions;
	chunkMemory = new ChunkMemoryManager(dbOpt, _data.optimizedChunks, _data.chunkPriorityOrder);

	delete[] _data.optimizedChunks;
	delete[] _data.chunkPriorityOrder;
}

VS::VoxelDatabase::~VoxelDatabase()
{
	delete chunkMemory;

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

	_chunk.setVoxel(VOXEL_ID(relativeX, relativeY, relativeZ), voxelData);

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

	return _chunk.getVoxel(VOXEL_ID(relativeX, relativeY, relativeZ));
}

void VS::VoxelDatabase::saveData(const char* fileLocation)
{
	const int chunkCount = dbOpt.chunkSizeX * dbOpt.chunkSizeY * dbOpt.chunkSizeZ;

	// Constructing
	VoxelDatabaseData _data{
		.databaseOptions = dbOpt,
		.optimizedChunks = nullptr,
		.chunkPriorityOrder = nullptr
	};

	int fullyLoadedCount = dbOpt.fullyLoadedChunkBufferSize;

	_data.optimizedChunks = new OptimizedChunk[chunkCount]();
	_data.chunkPriorityOrder = new unsigned short[chunkCount]();

	for (int chunkID = 0; chunkID < chunkCount; chunkID++) {
		Chunk& _workingChunk = chunkMemory->getChunk(chunkID);
		switch (_workingChunk.saveState)
		{
		case ChunkSaveState::FullyLoadedChunk:
			_data.optimizedChunks[chunkID] = OptimizedChunk(*_workingChunk.fullyLoadedChunk);
			_data.chunkPriorityOrder[_workingChunk.priorityPosition] = chunkID;
			break;

		case ChunkSaveState::OptimizedChunk:
			_data.optimizedChunks[chunkID] = OptimizedChunk(*_workingChunk.optimizedChunk);
			_data.chunkPriorityOrder[_workingChunk.priorityPosition + fullyLoadedCount] = chunkID;
			break;
		}
	}
	
	{
		std::ofstream file(fileLocation, std::ios::binary);
		cereal::PortableBinaryOutputArchive _archive(file);

		_archive(_data);
	}

	delete[] _data.optimizedChunks;
	delete[] _data.chunkPriorityOrder;
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
