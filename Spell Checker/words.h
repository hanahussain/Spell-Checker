#include <string>
#include "hash.h"

#ifndef WORDS_H_EXISTS
#define WORDS_H_EXISTS

class words {
	private:
		std::string dict[200];
		std::string typedWord;
		hash h;	
	public:
		words();
		~words();
		void setTypedWord(std::string typedGuess); //for when user says yes to the "did you mean" suggestion
		void parseDictionary(); //converts input file to array
		void fixSort();
		std::string userString();
		std::string toLowerCase();
		bool inDict();
		void giveSuggestions();
		std::vector<std::string> makeSimilar();
		std::vector<std::string> reduceSimilar();
		std::string determineWord();
		int letterComparison(std::string curr);
		int weightedComparison(std::string curr);
		bool guessCorrect(std::string guess);
};

#endif
