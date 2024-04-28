
# VoxelStream

This is a C++ library to provide a memory-efficient way to store voxels in a fixed-size world. The main objective of the library is to reduce the memory size in RAM. In the example benchmark, memory usage reduced from 8GB to 300MB, but how?

## Features

- Memory-efficient chunk store
- Custom voxel store (Only when compiled)
- Up to 2^16 possible voxel combinations
- Very simple API without external dependencies

## How to Use?
### Include:
Import Voxel Stream. There are only 2 header files.
```cpp
#include "VoxelStream/DataStructures.h"
#include "VoxelStream/VoxelDatabase.h"
```

### Database Options:
First, you need to create Database Options.

```cpp
const VS::DatabaseOptions _dbOptions = {
	.chunkSizeX = 64,
	.chunkSizeY = 16,
	.chunkSizeZ = 64,
	.maxBlockID = 200,
	.fullyLoadedChunkBufferSize = 64 * 64 + 10,
	.modifiedVoxelPriorityValue = 34,
	.queryedVoxelPriorityValue = 1
};
```
#### Parameters:
-   To define the size, you have to specify it in chunks. Each chunk is 32 voxels in each direction. So, if you put 64 chunks, this means the world will be 64 chunks * 32 voxels in the direction you decided.
-   Max block ID is the max block ID you will have. If you have 300 types of blocks and your highest ID is 345, you have to put 346. It's always 1 + max block ID.
-   Fully loaded chunk buffer size is a special part of the library. For better understanding, read the 'How Does It Work' part. Basically, this is the amount of chunks you think will be constantly edited in the world. The more you put, the more memory it will consume. The optimal value would be your ChunkSizeX * ChunkSizeZ + some amount or even less.
-   Modified Voxel Priority value is how an edit of a voxel increments the priority in a chunk. The optimal value is the chunk size / 2.
-   Queryed Voxel Priority value is how an obtained voxel increments the priority in a chunk. The optimal value is low because obtaining a value does not affect CPU usage in an optimal chunk.

To create the voxel database, just make the object with the value.
 
###  Voxel Database:
To create the voxel database just make the object with the value
```cpp
VS::VoxelDatabase myVoxelDatabase(_dbOptions);
```
Edit or get a voxel
```cpp
void funct(){
    // Edit Voxel
    VS::VoxelData myVoxel {
    	.blockID = _blockID,
    	.blockState = _blockState
    };

    myVoxelDatabase.SetVoxel(_posX, _posY, _posZ, myVoxel);

    // Get Voxel
    myVoxel = vd.GetVoxel(randomX, randomY, randomZ);
    
}
```
## Build
This project uses Premake, it is very easy to use just look up the documentation 

> https://premake.github.io/docs/Building-Premake

If you build for Visual Studio 20 just run the batch script in **Scrips/build.bat**
This will create a project solution tu just build the project

## How does it work?

It has 2 memory optimitzation, chunk storing and voxel palette

### Chunk storing


VoxelStream uses two methods for storing chunks (32 x 32 x 32). One is a fully loaded chunk, which means it is an array of fixed size of 32 x 32 x 32. This method is very optimal for modifying and obtaining values, but it is very low efficient in memory, so we want to minimize it.

The other method is a dynamic array which stores Nodes that are made of a voxel ID and a repetition value. This means that instead of repeating voxels, it can join them in a single group and use less memory, but this means that obtaining and reading memory will be slower.

How to decide which one to use for every voxel? This will be decided dynamically depending on the usage. Internally, there is a big list of priority or chunk usage. Every time a block is updated or obtained, it will increase in the priority group, and if it is used frequently, it will become a fully loaded chunk. However, if it is not used, it will eventually be replaced by another that is used more, and all of it is dynamic.

 

### Voxel Palette

A voxel contains a 2-byte ID that references a palette that has the info of the chunk, and it can be custom. This means that instead of saving the information on the voxel array and incrementing the save size, we can only store the ID with 2 bytes and use less memory.

# Benchmark


This library is made for real applications in games which use voxels in interactive worlds. Normally, there are always chunks that are used a lot and others like inside terrain that are not even referenced until something happens. But if we make a simple benchmark where we have a world of 64 chunks (2048 voxels) x 64 chunks x 16 chunks(512 voxels) and we put a fully loaded chunk buffer of 64 x 64 + 10 and we edit 1,000,000 voxels in any random place, we get:

808 milliseconds and 489 transformations (optimized chunks to fully loaded chunk swaps), and it does arrive at 300MB of memory. If we took this benchmark without these optimizations, we would get a memory usage of 32 voxels * 32 voxels * 32 voxels * 64 chunks * 64 chunks * 16 chunks * 4 bytes per voxel (voxelID, voxel state ...) = 8GB of memory. So it is an improvement of 27:1.

In the case there is no transformation, 1,000,000 voxels will give: 300 ms.

