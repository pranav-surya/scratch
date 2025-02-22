#ifndef __TABLE_H__
#define __TABLE_H__

#include <stdbool.h>

#include "common.h"

typedef struct {
  void* ptr;
} TableKey;

typedef struct {
  void* ptr;
} TableVal;

typedef struct {
  TableKey* key;
  TableVal* val;
} TableEntry;

typedef struct {
  int len;
  int cap;
  TableEntry* entries;
  int (*GenHash)(TableKey* tk);
  bool (*IsEqual)(TableKey* t1, TableKey* t2);
  void (*KeyToString)(TableKey* tk);
  void (*ValToString)(TableVal* tv);
} Table;

Table* TableInit(int cap, int (*GenHash)(TableKey* tk), void (*KeyToString)(TableKey* tk), void (*ValToString)(TableVal* tv), bool (*IsEqual)(TableKey* t1, TableKey* t2));
void TableFree(Table** t);
void TableSet(Table* t, TableKey* tk, TableVal* tv);
void TableRemove(Table* t, TableKey* tk);
TableVal* TableGet(Table* t, TableKey* tk);
void TableDisplay(Table* t);
TableEntry* FindEntry(TableEntry* entries, int cap, TableKey* tk, int (*gh)(TableKey* tk), bool (*ieq)(TableKey* t1, TableKey* t2));

#endif