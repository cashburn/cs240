
//
// Implementation of a HashTable that stores void *
//
#include "HashTableVoid.h"

// Obtain the hash code of a key
int HashTableVoid::hash(const char * key) {
    int sum = 0;
    int count = 0;
    while (*key) {
        sum += *key;
        sum += count;
        key++;
        count++;
    }
    return sum % TableSize;
}

// Constructor for hash table. Initializes hash table
HashTableVoid::HashTableVoid() {
    _buckets = (HashTableVoidEntry **) malloc(TableSize * sizeof(HashTableVoidEntry *));
    for (int i = 0; i < TableSize; i++) {
        _buckets[i] = NULL;
    }       
}

// Add a record to the hash table. Returns true if key already exists.
// Substitute content if key already exists.
bool HashTableVoid::insertItem( const char * key, void * data) {
    int hash = HashTableVoid::hash(strdup(key));
    HashTableVoidEntry * e = _buckets[hash];
    while (e != NULL) {
        if (strcmp(key, e->_key) == 0) {
            e->_data = data;
            return true;
        }
        e = e->_next;
    }
    e = new HashTableVoidEntry;
    e->_data = data;
    e->_key = strdup(key);
    e->_next = _buckets[hash];
    _buckets[hash] = e;
    return false;
}

// Find a key in the dictionary and place in "data" the corresponding record
// Returns false if key is does not exist
bool HashTableVoid::find( const char * key, void ** data) {
    int hash = HashTableVoid::hash(strdup(key));
    HashTableVoidEntry * e = _buckets[hash];
    while (e != NULL) {
        if (strcmp(key, e->_key) == 0) {
            *data = e->_data;
            return true;
        }
        e = e->_next;
    }
    return false;
}

// Removes an element in the hash table. Return false if key does not exist.
bool HashTableVoid::removeElement(const char * key) {
    int hash = HashTableVoid::hash(strdup(key));
    HashTableVoidEntry * e = _buckets[hash];
    HashTableVoidEntry * prev = NULL;
    while (e != NULL) {
        if (strcmp(key, e->_key) == 0) {
            if (prev == NULL)
                _buckets[hash] = e->_next;
            else
                prev->_next = e->_next;
            delete e;
            return true;
        }
        prev = e;
        e = e->_next;
    }
    return false;
}

// Creates an iterator object for this hash table
HashTableVoidIterator::HashTableVoidIterator(HashTableVoid * hashTable) {
    _hashTable = hashTable;
    _currentBucket = 0;
    _currentEntry = hashTable->_buckets[_currentBucket];
}

// Returns true if there is a next element. Stores data value in data.
bool HashTableVoidIterator::next(const char * & key, void * & data) {
    //_currentBucket++;
    //_currentEntry = _hashTable._buckets[_currentBucket];
    bool next = false;
    while (1) {
        while ((_currentEntry == NULL) && (_currentBucket < _hashTable->TableSize)) {
            _currentBucket++;
            _currentEntry = _hashTable->_buckets[_currentBucket];
            next = true;
        }
        if (_currentBucket == _hashTable->TableSize)
            return false;
        if (next) {
            _currentEntry->_data = data;
            return true;
        }
        if (_currentEntry->_next != NULL) {
            _currentEntry = _currentEntry->_next;
            _currentEntry->_data = data;
            return true;
        }
        _currentBucket++;
    }
    return false;
}

