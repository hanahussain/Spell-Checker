#include "hash.h"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>

hash::hash(){
	this->table = new std::vector<std::string>[20];
}

hash::~hash(){
}

std::vector<std::string>* hash::getTable(){
	return table;
}

void hash::setTable(std::vector<std::string>* table){
	this->table = table;
} //end setTable

//create hashFunction to map words to their indices in hash table
int hash::hashFunction(std::string word){
	int total = 0;
	for(int i = 0; i < word.size(); i++){
		total += int(word[i]); //add ascii value of current letter to total
	} //end for
	int weight = int(word[0]);
	int size = word.size();
	int weight2 = int(word[1]);
	//hash is based on sum of letter's ASCII values times the ASCII value of the first and second letters
	//then divided by size by a constant
	//this way, we are considering the ASCII values, the first two letters, and the size of the word
	return (total*weight*weight2+total)/(size*25000*4); 
} //end hashFunction

void hash::insert(std::string word){
	int index = hashFunction(word);
	table[index].push_back(word);
	//std::cout << table[index][1] << std::endl;
} //end insert

void hash::buildTable(std::string dict[]){
	for(int i = 0; i < 200; i++){
		this->insert(dict[i]);
	} //end for
} //end buildTable

//print list for testing
void hash::printList(){ 
	std::cout << std::to_string(table->size()) << std::endl;
	for(int j = 0; j < 16; j++){
		if(!table[j].empty()){
			for(int i = 0; i < table[j].size(); i++){
				std::cout << std::to_string(j) << "," << std::to_string(i) << ": " << table[j][i] << " -> ";
			} //end inner for
		} //end if
		std::cout << "\n";
	} //end outer for
} //end printList	
