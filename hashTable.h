#ifndef HASHTABLE_INCLUDED
#define HASHTABLE_INCLUDED

#include <string>
#include <list>
#include <vector>

//all functions in hashTable are case insensitive
class hashTable {
private:
	int MAX_BUCKETS;	
	std::vector<std::list<std::string>> buckets;
	unsigned int hash(std::string toHash);
	void convertToCap(std::string& s);

public:
	hashTable();
	bool hasKey(std::string query);
	void insert(std::string toInsert);
};
#endif