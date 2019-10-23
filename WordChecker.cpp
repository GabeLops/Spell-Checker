// WordChecker.cpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"

#include <algorithm>

WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}

bool WordChecker::wordExists(const std::string& word) const
{
    return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    std::vector<std::string> suggestions;

    findSuggestionsTechnique1(suggestions, word);
    findSuggestionsTechnique2(suggestions, word);
    findSuggestionsTechnique3(suggestions, word);
    findSuggestionsTechnique4(suggestions, word);
    findSuggestionsTechnique5(suggestions, word);

    return suggestions;
}

void WordChecker::findSuggestionsTechnique1(std::vector<std::string> &suggestions, std::string word) const
{
	for(int i = 0; i < word.size()-1; i++) 
	{
		// swap characters
		std::swap(word[i], word[i+1]);

		// if word exists add it to suggestions
		if(wordExists(word))
		{
			suggestions.push_back(word);
		}

		//swap characters back
		std::swap(word[i], word[i+1]);
	}
}

void WordChecker::findSuggestionsTechnique2(std::vector<std::string> &suggestions, std::string word) const
{
	for(int i = 0; i < word.size()+1; i++) 
	{
		for(char c = 'A'; c <= 'Z'; c++)
		{
			// insert char
			word.insert(i, 1, c);

			// if word exists add it to suggestions
			if(wordExists(word))
			{
				suggestions.push_back(word);
			}

			//delete inserted char
			word.erase(i, 1);
		}
	}	
}

void WordChecker::findSuggestionsTechnique3(std::vector<std::string> &suggestions, std::string word) const
{
	for(int i = 0; i < word.size(); i++) 
	{
		// first save char
		char c = word[i];

		// delete char
		word.erase(i, 1);

		// if word exists add it to suggestions
		if(wordExists(word))
		{
			suggestions.push_back(word);
		}

		//insert char back
		word.insert(i, 1, c);
	}	
}

void WordChecker::findSuggestionsTechnique4(std::vector<std::string> &suggestions, std::string word) const
{
	for(int i = 0; i < word.size(); i++) 
	{
		// first saved the char
		char saved = word[i];
		word.erase(i, 1);

		for(char c = 'A'; c <= 'Z'; c++)
		{
			// insert char
			word.insert(i, 1, c);

			// if word exists add it to suggestions
			if(wordExists(word))
			{
				suggestions.push_back(word);
			}

			//delete inserted char
			word.erase(i, 1);
		}

		// insert char back
		word.insert(i, 1, saved);
	}	
}

void WordChecker::findSuggestionsTechnique5(std::vector<std::string> &suggestions, std::string word) const
{
	for(int i = 1; i < word.size(); i++)
	{
		std::string left = word.substr(0, i);
		std::string right = word.substr(i, word.size() - i);

		// if words exists add it to suggestions
		if(wordExists(left) && wordExists(right))
		{
			suggestions.push_back(left + " " + right);
		}		
	}
}