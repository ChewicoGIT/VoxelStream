#pragma once
#include "VoxelStream/DataStructures.h"
#include "OptimizedChunk.h"
#include <cereal/types/vector.hpp>

namespace VS {
	
	template<class Archive>
	void serialize(Archive& archive,
		VS::Node & _node)
	{
		archive(_node.voxel, _node.repetition);
	}
	
	template<class Archive>
	void serialize(Archive& archive,
		VS::OptimizedChunk& _optimizedChunk)
	{
		archive(_optimizedChunk.nodes);
	}
	
	template<class Archive>
	void serialize(Archive& archive, DatabaseOptions& _databaseOptions) {

		archive(_databaseOptions.chunkSizeX, 
			_databaseOptions.chunkSizeY,
			_databaseOptions.chunkSizeZ,
			_databaseOptions.fullyLoadedChunkBufferSize,
			_databaseOptions.modifiedVoxelPriorityValue,
			_databaseOptions.queryedVoxelPriorityValue);

	}

}
