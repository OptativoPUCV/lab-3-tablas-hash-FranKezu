#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair(char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash(char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    if ((float) map->size / map->capacity >= 0.7) enlarge(map);

    long pos = hash(key, map->capacity);

    do{
        if(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL){
            Pair *elem = createPair(key, value);
            map->buckets[pos] = elem;
            map->current = pos;
            map->size++;
            return;
        }
        pos = (pos + 1) % map->capacity;
    }while(1);
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)

    Pair **viejo = map->buckets;
    map->capacity *= 2;
    map->buckets = (Pair **) calloc(map->capacity, sizeof(Pair *));
    map->size = 0;
    map->current = -1;

    for(long i = 0; i < (map->capacity/2); i++)
        if(viejo[i] != NULL && viejo[i]->key != NULL) insertMap(map, viejo[i]->key, viejo[i]->value);
    free(viejo);    
}


HashMap * createMap(long capacity) {
    HashMap *map = (HashMap*) malloc(sizeof(HashMap));
    map->buckets = (Pair **) calloc(capacity, sizeof(Pair));

    map->current = -1;
    map->capacity = capacity;
    map->size = 0;

    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    long pos = hash(key, map->capacity);
    long original = pos;

    do{
        if(map->buckets[pos] == NULL) return;

        if(is_equal(key, map->buckets[pos]->key)){
            free(map->buckets[pos]->key);
            map->buckets[pos]->key = NULL;
            map->size--;
            map->current = pos;
            return;
        }
        pos = (pos + 1) % map->capacity;
    }while(original != pos);
}

Pair * searchMap(HashMap * map,  char * key) {   
    long pos = hash(key, map->capacity);
    long original = pos;

    do{
        if(map->buckets[pos] == NULL) return NULL;

        if(is_equal(key, map->buckets[pos]->key)){
            map->current = pos;
            return map->buckets[pos];
        }
        pos = (pos + 1) % map->capacity;
    }while(original != pos);

    return NULL;
}

Pair * firstMap(HashMap * map) {
    if (map == NULL || map->buckets == NULL) return NULL;

    for(long i = 0 ; i < map->capacity ; i++){
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    if (map == NULL || map->buckets == NULL) return NULL;

    for(long i = map->current + 1 ; i < map->capacity ; i++){
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}
