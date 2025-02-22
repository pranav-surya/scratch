#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdbool.h>

#include "common.h"

typedef struct {
	void* array;
	int len;
	int cap;
	bool (*Less)(void* a, void* b);
	void (*ToString)(int index, void* a);
	int itemSize;
} Heap;

Heap* HeapInit(int cap, int itemSize, bool (*Less)(void* a, void* b), void (*ToString)(int index, void* a));
void HeapRealloc(Heap* h, int newCap);
void HeapFree(Heap** h);
void HeapDisplay(Heap* h);
void HeapAdd(Heap* h, void* item);
void* HeapRemove(Heap* h);

#endif

