#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "heap.h"

Heap* HeapInit(int cap, int itemSize, bool (*Less)(void* a, void* b), void (*ToString)(int index, void* a)) {
        Heap* h = (Heap*)malloc(sizeof(Heap));
        CHECK_OOM(h);
        h->len = 0;
        h->cap = (cap <= 0) ? 1 : cap;
        h->itemSize = itemSize;
        h->Less = Less;
        h->ToString = ToString;
        int ms = h->itemSize * h->cap;
        h->array = malloc(ms);
        CHECK_OOM(h->array);
        memset(h->array, 0, ms);
        return h;
}

void HeapFree(Heap** h) {
        free((*h)->array);
        free(*h);
        *h = NULL;
}

void HeapDisplay(Heap* h) {
        if (!h) {
                printf("heap: %p | size: %d | capacity: %d | length: %d\n", NULL, 0, 0, 0);
                return;      
        }
        printf("heap: %p | size: %d | capacity: %d | length: %d\n", h, h->itemSize, h->cap, h->len);
        for (int i = 0; i < h->len; i++) {
                h->ToString(i, h->array+i*h->itemSize);
        }
}

void HeapRealloc(Heap* h, int newCap) {
        h->cap = newCap;
        h->array = realloc(h->array, h->itemSize*h->cap);
        CHECK_OOM(h->array);
}

void HeapAdd(Heap* h, void* item) {
        if (h->len >= h->cap) {
                HeapRealloc(h, h->cap*2);
        }
        void* base = h->array;
        int sz = h->itemSize;
        int i = h->len++;
        int p = (i-1)/2;
        memcpy(base+i*sz, item, sz);
        while (i > 0 && h->Less(base+i*sz, base+p*sz)) {
                void* t = malloc(sz);
                CHECK_OOM(t);
                memcpy(t, base+i*sz, sz);
                memcpy(base+i*sz, base+p*sz, sz);
                memcpy(base+p*sz, t, sz);
                free(t);
                i = p;
                p = (i-1)/2;
        }
}

void* HeapRemove(Heap* h) {
        if (h->len == 0) {
                errno = ERANGE;
                return 0;
        }
        void* base = h->array;
        int sz = h->itemSize;
        void* out = malloc(sz);
        CHECK_OOM(out);
        memcpy(out, base, sz);
        memcpy(base, base+(--h->len)*sz, sz);
        int i = 0;
        while (true) {
                int l = i*2+1;
                int r = i*2+2;
                int m = i;
                if (l < h->len && h->Less(base+l*sz, base+m*sz)) {
                        m = l;
                }
                if (r < h->len && h->Less(base+r*sz, base+m*sz)) {
                        m = r;
                }
                if (i == m) {
                        break;
                }
                void* t = malloc(sz);
                CHECK_OOM(t);
                memcpy(t, base+m*sz, sz);
                memcpy(base+m*sz, base+i*sz, sz);
                memcpy(base+i*sz, t, sz);
                free(t);
                i = m;
        }
        if (h->len < h->cap/3) {
                HeapRealloc(h, h->cap/2);
        }
        return out;
}
