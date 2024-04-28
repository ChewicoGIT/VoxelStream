#pragma once
#include "VoxelStream/DataStructures.h"
#include "OptimizedChunk.h"
#include "VoxelMemoryPaletteManager.h"
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
	void serialize(Archive& archive, VoxelData& _voxelData) {
		archive(_voxelData.blockID);
		archive(_voxelData.blockState);
	
	}
	
	template<class Archive>
	void save(Archive& archive, VoxelMemoryPaletteManager const& _palette) {
	
		archive(_palette.paletteData);
	
		for (int x = 0; x < _palette.dbOpt.maxBlockID; x++)
			archive(_palette.dataPalettePointer[x]);
	
	}
	
	template<class Archive>
	void load(Archive& archive, VoxelMemoryPaletteManager& _palette) {
	
		archive(_palette.paletteData);
	
		_palette.dataPalettePointer = new std::vector<VOXEL_TYPE>[_palette.dbOpt.maxBlockID];
		for (int x = 0; x < _palette.dbOpt.maxBlockID; x++)
			archive(_palette.dataPalettePointer[x]);
	
	}

	template<class Archive>
	void save(Archive& archive, DatabaseOptions const& _databaseOptions) {

		archive(_databaseOptions.chunkSizeX);
		archive(_databaseOptions.chunkSizeY);
		archive(_databaseOptions.chunkSizeZ);
		archive(_databaseOptions.fullyLoadedChunkBufferSize);
		archive(_databaseOptions.maxBlockID);
		archive(_databaseOptions.modifiedVoxelPriorityValue);
		archive(_databaseOptions.queryedVoxelPriorityValue);

	}
	template<class Archive>
	void load(Archive& archive, DatabaseOptions& _databaseOptions) {
	
		archive(_databaseOptions.chunkSizeX);
		archive(_databaseOptions.chunkSizeY);
		archive(_databaseOptions.chunkSizeZ);
		archive(_databaseOptions.fullyLoadedChunkBufferSize);
		archive(_databaseOptions.maxBlockID);
		archive(_databaseOptions.modifiedVoxelPriorityValue);
		archive(_databaseOptions.queryedVoxelPriorityValue);
	
	}

}
