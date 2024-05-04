#include "Common.h"

void saveExample() {

	vd = new VS::VoxelDatabase(_dbOpt);

	time_t randomSeed = time(NULL);

	srand(5);
	int lastX = 0;
	auto start = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < MAX_ITIRENATIONS; x++) {

		int randomX = x / 2024;
		int randomY = rand() % (16 * 1 * 32);
		int randomZ = rand() % (64 * 1 * 32);

		unsigned short randomVoxel = rand() % 200;
		unsigned short randomStat = rand() % 200;

		vd->SetVoxel(randomX, randomY, randomZ, VS::VoxelData{
			.blockID = randomVoxel,
			.lightInfo = randomStat
			});

		//if (lastX / 16 != randomX / 16)
		//{
		//	lastX = randomX;
		//	std::cout << lastX << " - t : " << vd->getTransformations() << " : fl : " << ((float)vd->objectWasFullyLoaded() / (float)(2048 * 16) * 100) << "\n ";
		//}


	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Finished writing \n";
	std::cout << "Transformations: " << vd->getTransformations() << "\n";
	std::cout << "Time taken: " << duration.count() << " milliseconds" << "\n";
	std::cout << "\n";

	getchar();

	start = std::chrono::high_resolution_clock::now();
	end = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Finished saving \n";
	std::cout << "Time taken: " << duration.count() << " milliseconds" << "\n";
	std::cout << "\n";

	vd->saveData("voxels.bin");
	std::cout << "Saved in voxels.bin" << "\n";
	//delete vd;

}


#include "Common.h"

void loadExample() {


	std::cout << " Loading... \n";
	auto _start = std::chrono::high_resolution_clock::now();
	//vd = new VS::VoxelDatabase("voxels.bin");

	auto _end = std::chrono::high_resolution_clock::now();
	auto _duration = std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start);
	std::cout << "Loaded finished \n";
	std::cout << "Time taken: " << _duration.count() << " milliseconds" << std::endl;

	srand(5);

	int collisionOrError = 0;
	_start = std::chrono::high_resolution_clock::now();
	int lastX = 0;
	for (int x = 0; x < MAX_ITIRENATIONS; x++) {

		int randomX = x / 2024;
		int randomY = rand() % (16 * 1 * 32);
		int randomZ = rand() % (64 * 1 * 32);

		unsigned short randomVoxel = rand() % 200;
		unsigned short randomStat = rand() % 200;

		VS::VoxelData voxel = vd->GetVoxel(randomX, randomY, randomZ);

		if (voxel.blockID != randomVoxel || voxel.lightInfo != randomStat) {
			collisionOrError++;
		}
	}

	_end = std::chrono::high_resolution_clock::now();
	_duration = std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start);
	std::cout << "Finished reading \n";
	std::cout << "Time taken: " << _duration.count() << " milliseconds" << std::endl;

	std::cout << "Collision errors : " << collisionOrError;
	delete vd;
	getchar();

}