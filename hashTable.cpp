#include<string>
#include "hashTable.h"


hashTable::hashTable() : MAX_BUCKETS(79999) { //prime
	// Start off the buckets with MAX_BUCKETS
	buckets.resize(MAX_BUCKETS);
}

bool hashTable::hasKey(std::string query) {
	//all words stored in hashTable are in caps
	convertToCap(query);
	// Store a pointer to the bucket's list
	std::list<std::string>* lPtr = &buckets[hash(query)];
	// Start an iterator at it's beginning
	std::list<std::string>::iterator it = lPtr->begin();
	// Search linearly for the query item
	while (it != lPtr->end()) {
		if (*it == query) {
			break;
		}
		it++;
	}
	// Return whether the iterator found a
	// match or not
	return it != lPtr->end();
}

void hashTable::insert(std::string toInsert) {
	if (hasKey(toInsert)) {
		return;
	}
	//all words stored in hashTable are in caps
	convertToCap(toInsert);
	// [!] Push toInsert to the back of the list that the
	// hash function gives you from input toInsert
	buckets[hash(toInsert)].push_back(toInsert);
}


unsigned int hashTable::hash(std::string toHash)
{
	unsigned int result = 0;
	for (int i = 0; i < toHash.length(); i++) {
		result = result * 101 + toHash[i];
	}
	return result % MAX_BUCKETS;
}

void hashTable::convertToCap(std::string& s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] <= 'z' && s[i] >= 'a')
			s[i] = s[i] - 'a' + 'A';
	}
}