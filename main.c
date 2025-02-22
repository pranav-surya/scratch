#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "heap.h"
#include "table.h"

bool Less(void* a, void* b) {
	return strcmp(*(char**)a, *(char**)b) < 0;
}

void ToString(int i, void* a) {
	printf("[%05d] %p %p %s\n", i, a, *(char**)a, *(char**)a);
}

int HeapTest() {
	srand(time(NULL));
	Heap* h = HeapInit(5, sizeof(char*), Less, ToString);
	char *alphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
	for (int i = 0; i < 150; i++) {
		int sz = (rand()%20)+1;
		int ms = sizeof(char)*(sz+1);
		char* s = (char*)malloc(ms);
    CHECK_OOM(s);
		memset(s, 0, ms);
		for (int j = 0; j < sz; j++) {
			int p = rand()%36;
			s[j] = alphabet[p];
		}
		s[sz] = '\0';
		HeapAdd(h, &s);
	}
	HeapDisplay(h);
	for (;h->len;) {
		errno = 0;
		char** s = HeapRemove(h);
		if (errno) {
			printf("Tried to remove from empty heap!\n");
			exit(1);
		}
		free(*s);
		free(s);
	}
	HeapDisplay(h);
	HeapFree(&h);
	HeapDisplay(h);
	return 0;
}

int GenHash(TableKey* tk) {
	int* k = (int*)tk->ptr;
	int hash = 21613;
  int k1 = *k;
  while (k1) {
  	hash ^= k1%10;
  	hash *= 167;
  	k1 /= 10;
  }
  return (hash < 0) ? hash*-1 : hash;
}

void KeyToString(TableKey* tk) {
	if (tk == NULL || tk->ptr == NULL) {
		printf("key: %p\n", NULL);
	} else {
		printf("key: %d\n", *(int*)tk->ptr);
	}
}

void ValToString(TableVal* tv) {
	if (tv == NULL || tv->ptr == NULL) {
		printf("val: %p\n", NULL);
	} else {
		printf("val: %d\n", *(int*)tv->ptr);
	}
}

bool IsEqual(TableKey* t1, TableKey* t2) {
	return *(int*)(t1->ptr) == *(int*)(t2->ptr);
}

int TableTest() {
	srand(time(NULL));
	Table* t = TableInit(20, GenHash, KeyToString, ValToString, IsEqual);

	int *a, *b;
	TableKey* tk;
	TableVal* tv;

	for (int i = 0; i < 20; i++) {
		a = (int*)malloc(sizeof(int));
		CHECK_OOM(a);
		// *a = rand() % 1000;
		*a = i+1;

		tk = (TableKey*)malloc(sizeof(TableKey));
		CHECK_OOM(tk);
		tk->ptr = a;

		b = (int*)malloc(sizeof(int));
		CHECK_OOM(b);
		// *b = rand() % 1000;
		*b = (i+1)*10;

		tv = (TableVal*)malloc(sizeof(TableVal));
		CHECK_OOM(tv);
		tv->ptr = b;
		TableSet(t, tk, tv);
	}
	TableDisplay(t);

	for (int i = 0; i < 20; i++) {
		a = (int*)malloc(sizeof(int));
		CHECK_OOM(a);
		// *a = rand() % 1000;
		*a = i+1;

		tk = (TableKey*)malloc(sizeof(TableKey));
		CHECK_OOM(tk);
		tk->ptr = a;

		b = (int*)malloc(sizeof(int));
		CHECK_OOM(b);
		// *b = rand() % 1000;
		*b = (i+1)*100;

		tv = (TableVal*)malloc(sizeof(TableVal));
		CHECK_OOM(tv);
		tv->ptr = b;
		TableSet(t, tk, tv);
	}
	TableDisplay(t);

	for (int i = 0; i < 20; i++) {
		a = (int*)malloc(sizeof(int));
		CHECK_OOM(a);
		// *a = rand() % 1000;
		*a = i+1;

		tk = (TableKey*)malloc(sizeof(TableKey));
		CHECK_OOM(tk);
		tk->ptr = a;

		TableRemove(t, tk);
		free(tk->ptr);
		free(tk);
	}
	TableDisplay(t);

	for (int i = 0; i < 20; i++) {
		a = (int*)malloc(sizeof(int));
		CHECK_OOM(a);
		// *a = rand() % 1000;
		*a = i+1;

		tk = (TableKey*)malloc(sizeof(TableKey));
		CHECK_OOM(tk);
		tk->ptr = a;

		b = (int*)malloc(sizeof(int));
		CHECK_OOM(b);
		// *b = rand() % 1000;
		*b = (i+1)*100;

		tv = (TableVal*)malloc(sizeof(TableVal));
		CHECK_OOM(tv);
		tv->ptr = b;
		TableSet(t, tk, tv);
	}
	TableDisplay(t);

	for (int i = 0; i < 20; i++) {
		a = (int*)malloc(sizeof(int));
		CHECK_OOM(a);
		// *a = rand() % 1000;
		*a = i+1;

		tk = (TableKey*)malloc(sizeof(TableKey));
		CHECK_OOM(tk);
		tk->ptr = a;

		TableVal* tv = TableGet(t, tk);
		free(tk->ptr);
		free(tk);
	}
	TableDisplay(t);

	TableFree(&t);
	TableDisplay(t);
}

int main() {
	return 0;
}