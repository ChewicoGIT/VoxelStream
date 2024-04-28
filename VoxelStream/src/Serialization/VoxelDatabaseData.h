#pragma once
#include "VoxelStream/DataStructures.h"
#include "OptimizedChunk.h"
#include "ObjectsSerialization.h"
#include "VoxelMemoryPaletteManager.h"
#include "ChunkMemoryManager.h"

namespace VS {

	struct VoxelDatabaseData {
		DatabaseOptions databaseOptions;
		OptimizedChunk* optimizedChunks;
		VoxelMemoryPaletteManager* paletteData;

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(databaseOptions);
		
			const int chunkCount = databaseOptions.chunkSizeX *
				databaseOptions.chunkSizeY * databaseOptions.chunkSizeZ;
			
			for (int x = 0; x < chunkCount; x++)
				archive(optimizedChunks[x]);
			
			archive(*paletteData);
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(databaseOptions);

			const int chunkCount = databaseOptions.chunkSizeX *
				databaseOptions.chunkSizeY * databaseOptions.chunkSizeZ;
			
			optimizedChunks = new OptimizedChunk[chunkCount];
			for (int x = 0; x < chunkCount; x++)
				archive(optimizedChunks[x]);
			
			paletteData = new VoxelMemoryPaletteManager(databaseOptions);
			archive(*paletteData);
		}


	};



}