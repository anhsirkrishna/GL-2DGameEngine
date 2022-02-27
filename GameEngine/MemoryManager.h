/******************************************************************************/
/*!
/*File   MemoryManager.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   02/19/2022
/*  Memory Manager header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#pragma once
#include <stdlib.h>

class MemoryManager {
private:
	size_t memory_used_bytes;
public:
	/*Function to get the mememory used by the engine in Bytes
	* Returns: unsigned int
	*/
	size_t GetMemoryUsedBytes();
	/*Function to get the mememory used by the engine in Kilo Bytes
	* Returns: unsigned int
	*/
	size_t GetMemoryUsedKBytes();
	/*Function to get the mememory used by the engine in Mega Bytes
	* Returns: unsigned int
	*/
	size_t GetMemoryUsedMBytes();

	/*Function to add to the memory used
	* Returns: void
	*/
	void AddMemoryUsage(size_t size);

	/*Function to remove memory used
	*/
	void RemoveMemoryUsage(size_t size);
};

extern MemoryManager g_memory_manager;
