#pragma once
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <chrono>

#include "VoxelStream/DataStructures.h"
#include "VoxelStream/VoxelDatabase.h"

#define MAX_ITIRENATIONS 4000000


extern const VS::DatabaseOptions _dbOpt;

extern VS::VoxelDatabase* vd;

void randomAccesBenchMark();
void weightedBenchmark();
void saveExample();
void loadExample();
