#pragma once
#include "DataStructures.h"
#include "Definitions.h"

namespace VS {

	class VoxelDatabase
	{
	public:
		VoxelDatabase(DatabaseOptions opt);
		~VoxelDatabase();

	private:
		DatabaseOptions dbOpt;
		ChunkMemoryManager* chunkMemory;

	};

}

