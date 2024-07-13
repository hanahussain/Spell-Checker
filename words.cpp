#include <iostream>
#include <string>
#include <fstream>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include "words.h"
#include "TimeInterval.h"
#include <cstring>
#include <vector>
#include "hash.h"

words::words(){
	this->parseDictionary();
	sort(dict, dict+200);
	this->fixSort();
	words::typedWord = this->userString();
	h.buildTable(dict); //initializing hash table using dict input	
}

words::~words(){
} //end destructor


//convert dictionary input file into array of words
void words::parseDictionary(){
	std::ifstream inFile;
	std::string line;
	int i = 0;
	inFile.open("Dictionary.txt"); //use input stream to connect to input file

	while(getline(inFile, line)){ //as long as there's another line inn the input file
		dict[i] = line; //set the current array element to the current line in input file
		i++;
	} //end while
	inFile.close(); //close connection to file
} //end parseDictionary


//fixes the sort to account for the fact that it doesn't ignore case
void words::fixSort(){
	for(int i =2; i <= 8; i++){
		dict[i-2] = dict[i]; //America & I were at the front, shift everything 2 backward
	} //end for
	dict[7] = "America";
	for(int i = 9; i <= 69; i++){
		dict[i-1] = dict[i]; //shift only 1 backward because America has been placed
	} //end for
	dict[69] = "I";
} //end fixSort

//accounts for possible weird user casing
std::string words::toLowerCase(){
	//std::string newString = typedWord;
	for(int i = 0; i < typedWord.size(); i++){
		typedWord[i] = (char) tolower(typedWord[i]);
	} //end for
	return typedWord;
} //end toLowerCase

std::string words::userString(){
	bool validInput = false;
	std::string userString;
	while(!validInput){
		std::cout << "Type a word: ";
		std::cin >> userString;
		if(userString.find(" ") != std::string::npos){
			std::cout << "Please enter a single word (no spaces)" << std::endl;
		} else {
			validInput = true; //exit loop
		} //end if else
	} //end while
	typedWord = userString;
	userString = this->toLowerCase();
	return userString;
} //end userString

//determine if the word the user typed is in dictionary
bool words::inDict(){
	int index = h.hashFunction(typedWord);
	std::vector<std::string>* table = h.getTable();
	for(int i = 0; i < table[index].size(); i++){
		if(table[index][i] == typedWord){
			std::cout << "True" << std::endl;
			return true;
		} //end if
	} //end for
	return false;
} //end inDict

void words::setTypedWord(std::string typedGuess){
	typedWord = typedGuess;
} //end setTypedWord

//give either similar words to the user if they typed a word in the dictionary, or give a suggested word if they mis-typed
void words::giveSuggestions(){
	TimeInterval t;
	t.start(); //start the stopwatch
	bool inDictionary = this->inDict(); //determine if the word they typed is in the dictionary
	std::vector<std::string> similar = this->reduceSimilar(); //determine the list of similar words to the word they typed in
	if(inDictionary){
		for(int i = 0; i < similar.size(); i++){
			if(similar[i].substr(0,2) == typedWord.substr(0,2)){ //if the first two letters in the similar list's current word are the same as the first two letters in the word that was typed in
				if(!boost::iequals(similar[i], typedWord)){ //as long as they're not the samw word
					std::cout << similar[i] << std::endl; //print out the similar word
				} //end inner if
			} //end if
		} //end for
		t.stop(); //stop the timer
		std::cout << std::to_string(t.GetInterval()) << " micro-sec" << std::endl; //print out the time it took in microseconds	
	} else {
		t.stop(); //stop the timer
		std::string guess = this->determineWord(); //determineWord returns the word the algorithm guessed to be the most similar to the word that was typed in
		bool correct = this->guessCorrect(guess);
		if(correct){
			this->setTypedWord(guess);
			this->giveSuggestions(); //give similar words with the correctly spelled word
		} else { //if the user said "N" to the suggested word
			std::cout << "False" << std::endl; 
		} //end if else
	} //end if else
} //end giveSuggestions

//make a vector of words with a similar hash value as the word typed in
std::vector<std::string> words::makeSimilar(){
	int index = h.hashFunction(typedWord); //get the hash value of the word that was typed in
	std::vector<std::string> similar;
	std::vector<std::string>* table = h.getTable(); //get the hash table
	int start = index;
	if(index >= 2){ //deal with possible out of bounds errors
		start-=2;
	} //end if
	int end = index;
	if(index < 12){
		end += 2;
	} //end if
	for(int i = start; i <= end; i++){ //check the hash table slots 2 before and 2 after the hash value of the typed in word, just in case
		if(!table[i].empty()){ //as long as we're not looking at an empty slot
			for(int j = 0; j < table[i].size(); j++){ 
				similar.push_back(table[i][j]); //add the current hash element to the list of similar words
			} //end for
		} //end if
	} //end for
	std::sort(similar.begin(), similar.end()); //sort this similar list alphabetically
	return similar;
} //end makeSimilar

//reduces the size of the similar words by eliminating what it definitely can't be
std::vector<std::string> words::reduceSimilar(){
	std::vector<std::string> similar = this->makeSimilar();
	std::vector<std::string> newSimilar;
	for(int i = 0; i < similar.size(); i++){
		if(typedWord[0] == similar[i][0]){ //only add the words that have the same first letter (since we're assuming the first letter is correct)
			newSimilar.push_back(similar[i]);
		} //end if
	} //end for
	return newSimilar;
} //end reduceSimilar

//determines the word that is closest to what user typed in
std::string words::determineWord(){
	std::vector<std::string> similar = this->reduceSimilar();
	std::vector<int> penalties; //each element in penalties corresponds to the penalty score of each element in the similar list
	for(int i = 0; i < similar.size(); i++){
		if(typedWord.size() == similar[i].size()){ //do a simpler comparison if they're the same length
			penalties.push_back(this->letterComparison(similar[i])); //add the penalty found to penalties list
		} else { //do a weighted comparison if they have different lengths
			penalties.push_back(this->weightedComparison(similar[i]));
		} //end if else
	} //end for
	int smallestPenalty = 0;
	for(int j = 1; j < penalties.size(); j++){
		if(penalties[j] < penalties[smallestPenalty]){
			smallestPenalty = j; //determine the element in the penalties list that is the smallest
		} //end if
	} //end for
	return similar[smallestPenalty]; //return the corresponding element in the similar list
} //end determineWord

//compares typed word to a word in the hash table that is similar to it, then creates a penalty depending on how far off we are
int words::letterComparison(std::string curr){
	int penalty = 0;
	for(int i = 1; i < typedWord.size(); i++){ //don't test for first letter since that's already the same
		if(typedWord[i] == curr[i]){
			penalty -= 5;
		} else {
			penalty += 2;
		} //end if else
	} //end for
	return penalty;
} //end letterComparison

//for comparing words of different lengths, do a weighted comparison
int words::weightedComparison(std::string curr){
	int typedSize = typedWord.size();
	int currSize = curr.size();
	int lower = typedSize;
	std::string lowerString = typedWord;
	int higher = currSize;
	std::string higherString = curr; //determine lower&higher for looping&comparing
	int penalty = 0;
	if(currSize < typedSize){
		lower = currSize;
		lowerString = curr;
		higher = typedSize;
		higherString = typedWord;
	} //end if
	for(int i = 1; i < lower; i++){
		if(typedWord[i] == curr[i]){
			penalty -= 5; //reduce the penalty by a lot if the same letter is in the same place
		} else {
			if(i != lower-1){ //ensuring we don't go out of bounds
				if(higherString[i+1] == lowerString[i]){ //this takes into account letter offsetting that occurs from forgetting/adding a letter
					penalty -= 3; //reduce the penalty by a decent amount if any letter is offset by one position
					i++; //we've already tested the next i, so move on
				} else {
					penalty += 2; //increase penalty
				} //end inner if else
			} else {
				penalty +=2;
			} //end inner if else
		} //end outermost if else
	} //end for
	int difference = higher-lower;
	for(int k = difference; k < lower; k++){ //account for the fact that we only looped through the shorter word
		for(int m = 0; m < difference; m++){ //loop through the last part of the current word in the hash
			if(typedWord[m] == lowerString[k]){ 
				penalty -= 2;
			} //end if 
		}//end for
	} //end for
	return penalty;
} //end weightedComparison

//ask the user if they meant to type in a word in dictionary, return corresponding bool values
bool words::guessCorrect(std::string guess){
	bool correct = false;
	bool validInput = false;
	std::string input;
	while(!validInput){
		std::cout << "Do you mean " << guess << "? (Y/N): ";
		std::cin >> input;
		if((char) tolower(input[0]) == 'y'){ //account for if they said "yes" or "Y"
			correct = true;
			validInput = true;
		} else if ((char) tolower(input[0]) == 'n'){
			validInput = true;
		} else {
			std::cout << "Invalid input. Enter Y if my guess is correct, and N if it's incorrect." << std::endl;
		} //end if else
	} //end while
	return correct;
} //end guessCorrect
