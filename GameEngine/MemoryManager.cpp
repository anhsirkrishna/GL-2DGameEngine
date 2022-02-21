/******************************************************************************/
/*!
/*File   MemoryManager.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   02/19/2022
/*  Memory Manager header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "MemoryManager.h"

/*Function to get the mememory used by the engine in Bytes
* Returns: unsigned int
*/
size_t MemoryManager::GetMemoryUsedBytes() {
	return memory_used_bytes;
}

/*Function to get the mememory used by the engine in Kilo Bytes
* Returns: unsigned int
*/
size_t MemoryManager::GetMemoryUsedKBytes() {
	return memory_used_bytes / 1000;
}

/*Function to get the mememory used by the engine in Mega Bytes
* Returns: unsigned int
*/
size_t MemoryManager::GetMemoryUsedMBytes() {
	return memory_used_bytes / 1000 / 1000;
}

/*Function to add to the memory used
* Returns: void
*/
void MemoryManager::AddMemoryUsage(size_t size) {
	memory_used_bytes += size;
}

/*Function to remove memory used
*/
void MemoryManager::RemoveMemoryUsage(size_t size) {
	memory_used_bytes -= size;
}

void* operator new(size_t size) {
	g_memory_manager.AddMemoryUsage(size);
	return malloc(size * sizeof(char));
}

void operator delete(void* mem, size_t size) {
	g_memory_manager.RemoveMemoryUsage(size);
	free(mem);
}

void* operator new[](size_t size) {
	g_memory_manager.AddMemoryUsage(size);
	return malloc(size * sizeof(char));
}

void operator delete[](void* mem, size_t size) {
	g_memory_manager.RemoveMemoryUsage(size);
	free(mem);
}