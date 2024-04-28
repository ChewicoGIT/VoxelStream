#include "VoxelMemoryPaletteManager.h"
#include "VoxelStream/DataStructures.h"

VS::VoxelMemoryPaletteManager::VoxelMemoryPaletteManager(DatabaseOptions _dbOpt)
{
	dbOpt = _dbOpt;
	dataPalettePointer = new std::vector<VOXEL_TYPE>[dbOpt.maxBlockID]();

	getVoxelID(VoxelData() = {
			.blockID = 0,
			.blockState = 0
	});

}

VOXEL_TYPE VS::VoxelMemoryPaletteManager::getVoxelID(VoxelData voxel)
{
	for (int i = 0; i < dataPalettePointer[voxel.blockID].size(); i++) {

		VOXEL_TYPE _currentPaletteID = dataPalettePointer[voxel.blockID][i];

		if (paletteData[_currentPaletteID] == voxel)
			return _currentPaletteID;
	}

	VOXEL_TYPE _voxelPosition = paletteData.size();
	paletteData.push_back(voxel);
	
	dataPalettePointer[voxel.blockID].push_back(_voxelPosition);

	return _voxelPosition;
}

VS::VoxelData& VS::VoxelMemoryPaletteManager::getVoxelData(VOXEL_TYPE voxelID)
{
	return paletteData[voxelID];
}
