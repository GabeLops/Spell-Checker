// WordChecker.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// The WordChecker class can check the spelling of single words and generate
// suggestions for words that have been misspelled.
//
// You can add anything you'd like to this class, but you will not be able
// to modify the declarations of any of its member functions, since the
// provided code calls into this class and expects it to look as originally
// given.
//
// You are permitted to use the C++ Standard Library in this class.

#ifndef WORDCHECKER_HPP
#define WORDCHECKER_HPP

#include <string>
#include <vector>
#include "Set.hpp"



class WordChecker
{
public:
    // The constructor requires a Set of words to be passed into it.  The
    // WordChecker will store a reference to a const Set, which it will use
    // whenever it needs to look up a word.
    WordChecker(const Set<std::string>& words);


    // wordExists() returns true if the given word is spelled correctly,
    // false otherwise.
    bool wordExists(const std::string& word) const;


    // findSuggestions() returns a vector containing suggested alternative
    // spellings for the given word, using the five algorithms described in
    // the project write-up.
    std::vector<std::string> findSuggestions(const std::string& word) const;


private:
    const Set<std::string>& words;

    // find suggestion by swapping each adjacent pair of characters in the word.
    // add suggestions into vector passed in as parameter
    void findSuggestionsTechnique1(std::vector<std::string> &suggestions, std::string word) const;

    // find suggestion by following technique:
    // In between each adjacent pair of characters in the word (also before the first character and after the last character), each letter from 'A' through 'Z' is inserted.
    // add suggestions into vector passed in as parameter
    void findSuggestionsTechnique2(std::vector<std::string> &suggestions, std::string word) const;

    // find suggestion by following technique:
    // Deleting each character from the word.
    // add suggestions into vector passed in as parameter
    void findSuggestionsTechnique3(std::vector<std::string> &suggestions, std::string word) const;

    // find suggestion by following technique:
    // Replacing each character in the word with each letter from 'A' through 'Z'.
    // add suggestions into vector passed in as parameter
    void findSuggestionsTechnique4(std::vector<std::string> &suggestions, std::string word) const;

    // find suggestion by following technique:
    // Splitting the word into a pair of words by adding a space in between each adjacent pair of characters in the word. 
    // It should be noted that this will only generate a suggestion if both words in the pair are found in the word set
    // add suggestions into vector passed in as parameter
    void findSuggestionsTechnique5(std::vector<std::string> &suggestions, std::string word) const;
};



#endif // WORDCHECKER_HPP

