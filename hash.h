#ifndef HASH_H_EXISTS
#define HASH_H_EXISTS
#include <vector>
#include <string>

class hash{
	private:
		std::vector<std::string>* table; //pointer to array representing hash table

	public:
		hash();
		~hash();
		std::vector<std::string>* getTable();
		void setTable(std::vector<std::string>* table);
		int hashFunction(std::string word);
		void insert(std::string word);
		void buildTable(std::string dict[]);
		void printList();
};

#endif
