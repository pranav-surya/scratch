#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "table.h"

Table* TableInit(int cap, int (*GenHash)(TableKey* tk), void (*KeyToString)(TableKey* tk), void (*ValToString)(TableVal* tv), bool (*IsEqual)(TableKey* t1, TableKey* t2)) {
  Table* t = (Table*)malloc(sizeof(Table));
  CHECK_OOM(t);

  t->len = 0;
  t->cap = (cap <= 0) ? 1 : cap;
  t->GenHash = GenHash;
  t->IsEqual = IsEqual;
  t->KeyToString = KeyToString;
  t->ValToString = ValToString;

  int ms = sizeof(TableEntry)*t->cap;
  t->entries = (TableEntry*)malloc(ms);
  CHECK_OOM(t->entries);
  memset(t->entries, 0, ms);

  return t;
}

void FreeTableEntries(TableEntry** entries, int cap) {
  for (int i = 0; i < cap; i++) {
    TableKey* k = (*entries)[i].key;
    TableVal* v = (*entries)[i].val;
    if (k) {
      free(k->ptr);
      k->ptr = NULL;
      free(k);
    }
    if (v) {
      free(v->ptr);
      v->ptr = NULL;
      free(v);
    }
  }
  free(*entries);
  *entries = NULL;
}

void TableFree(Table** t) {
  FreeTableEntries(&(*t)->entries, (*t)->cap);
  free(*t);
  *t = NULL;
}

void TableRealloc(Table* t) {
  int newCap = t->cap*2;
  int ms = newCap*sizeof(TableEntry);
  TableEntry* newEntries = (TableEntry*)malloc(ms);
  CHECK_OOM(newEntries);
  memset(newEntries, 0, ms);
  t->len = 0;
  for (int i = 0; i < t->cap; i++) {
    TableEntry* e = &t->entries[i];
    if (e->key && e->val) {
      TableEntry* ne = FindEntry(newEntries, newCap, e->key, t->GenHash, t->IsEqual);
      ne->key = e->key;
      ne->val = e->val;
      e->key = NULL;
      e->val = NULL;
      t->len++;
    }
  }
  FreeTableEntries(&(t->entries), t->cap);
  t->entries = newEntries;
  t->cap = newCap;
}

void TableSet(Table* t, TableKey* tk, TableVal* tv) {
  if (t->len+1 > t->cap*0.75) {
    TableRealloc(t);
  }
  TableEntry* e = FindEntry(t->entries, t->cap, tk, t->GenHash, t->IsEqual);
  if (e->key) {
    if (e->val) {
      free(e->key->ptr);
      free(e->key);
      free(e->val->ptr);
      free(e->val);
    } else {
      free(e->key->ptr);
      free(e->key);
    }
  } else {
    t->len++;
  }
  e->key = tk;
  e->val = tv;
}

TableVal* TableGet(Table* t, TableKey* tk) {
  if (!t->len) return NULL;
  TableEntry* e = FindEntry(t->entries, t->cap, tk, t->GenHash, t->IsEqual);
  return (e->key) ? e->val : NULL;
}

void TableRemove(Table* t, TableKey* tk) {
  if (!t->len) return;
  TableEntry* e = FindEntry(t->entries, t->cap, tk, t->GenHash, t->IsEqual);
  if (!e->key) return;
  free(e->val->ptr);
  free(e->val);
  e->val = NULL;
}

TableEntry* FindEntry(TableEntry* entries, int cap, TableKey* tk, int (*gh)(TableKey* tk), bool (*ieq)(TableKey* t1, TableKey* t2)) {
  int h = gh(tk);
  int i = h%cap;
  TableEntry* tombstone = NULL;
  int start = i;
  while (true) {
    TableEntry* e = &entries[i];
    if (e->key) {
      if (ieq(e->key, tk)) {
        return e;
      } else if (!e->val) {
        tombstone = e;
      }
    } else {
      return (tombstone == NULL) ? e : tombstone;
    }
    i = (i+1)%cap;
    if (i == start) {
      printf("Could not find empty bucket!\n");
      exit(1);
    }
  }
}

void TableDisplay(Table* t) {
  if (!t) {
    printf("table: %p | length: %d | capacity: %d\n", NULL, 0, 0);
    return;
  }
  printf("table: %p | length: %d | capacity: %d\n", t, t->len, t->cap);
  for (int i = 0; i < t->cap; i++) {
    printf("[%d]\n", i);
    if (!t->entries[i].key) {
      continue;
    }
    if (t->entries[i].key && !t->entries[i].val) {
      printf("[tombstone]\n");
    }
    t->KeyToString(t->entries[i].key);
    t->ValToString(t->entries[i].val);
  }
}
