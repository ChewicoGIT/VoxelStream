#include "VoxelMemoryPaletteManager.h"

VS::VoxelMemoryPaletteManager::VoxelMemoryPaletteManager(DatabaseOptions _dbOpt)
{
	dbOpt = _dbOpt;
	dataPalettePointer = new std::vector<VOXEL_TYPE>();

	getVoxelID(VoxelData() = {
			.blockID = 0,
			.blockState = 0
	});

}

VOXEL_TYPE VS::VoxelMemoryPaletteManager::getVoxelID(VoxelData voxel)
{
	for (const VOXEL_TYPE& _paletteID : dataPalettePointer[voxel.blockID]) {
		if (paletteData[_paletteID].voxelData == voxel)
			return _paletteID;
	}

	VOXEL_TYPE _voxelPosition = paletteData.size();
	paletteData.push_back(VoxelPaletteData() = {
		.voxelData = voxel
	});

	dataPalettePointer[voxel.blockID].push_back(_voxelPosition);
	return _voxelPosition;
}

VS::VoxelData& VS::VoxelMemoryPaletteManager::getVoxelData(VOXEL_TYPE voxelID)
{
	return paletteData[voxelID].voxelData;
}
