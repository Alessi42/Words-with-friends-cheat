#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <chrono>
#include <fstream>
#include <iostream>
#include <locale>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;
using ns = chrono::nanoseconds;
using get_time = chrono::steady_clock;

/// Function prototype for the reading the file function
void readFile(string, set<string> &, ifstream &); 
/// Function modifies freq to be a character frequency of that string
void characterFrequency(string text, map<char, uint8_t> &freq);
/// Bool can the word be made with the hand
inline bool validWord(string word, char i, map<char, uint8_t> hand, uint8_t blanks);
/// Bool sorting which string has higher wordscore
inline bool wordScore(string i, string j); 

map<char, int> letterValues = { { 'A',1 },{ 'B',4 },{ 'C',4 },{ 'D',2 },{ 'E',1 },{ 'F' , 4 },{ 'G' , 3 },{ 'H' , 3 },{ 'I' , 1 },{ 'J' , 10 },{ 'K' , 5 },{ 'L' , 2 },{ 'M' , 4 },{ 'N' , 2 },{ 'O' , 1 },{ 'P' , 4 },{ 'Q' , 10 },{ 'R' , 1 },{ 'S', 1 },{ 'T' , 1 },{ 'U', 2 },{ 'V',5 },{ 'W',4 },{ 'X',8 },{ 'Y',3 },{ 'Z',10 } };
ifstream inFile;
// Input file name
string filename = "words.txt";
locale loc;
string quit; 
// Set of all english words in words.txt
set<string> wordList; 
vector<string>::iterator word; 
// All words that are matched
set<string> words; 
// Map amount of times each character appears in hand
map<char, uint8_t> freq; 
// Number of '?' wildcards
uint8_t blanks; 
// String of characters already on the board you're branching off of
string branch; 
// String of characters you have in your hand
string hand;

int main()
{
	auto time = get_time::now();
	// Read the contents of filename into the wordlist set
	readFile(filename, wordList, inFile); 
	cout << "Any key to continue `q` to quit:" << endl;
	// Has the user pressed q to quit
	while (getline(cin, quit) && quit != "q")
	{
		cout << "Enter the tiles already on the board: ";
		getline(cin, branch);
		cout << "Enter the tiles that are in your hand: ";
		getline(cin, hand);
		// Convert the input to uppercase
		for (std::string::size_type i = 0; i<branch.length(); ++i)
			branch[i] = toupper(branch[i], loc);
		for (std::string::size_type i = 0; i<hand.length(); ++i)
		    hand[i] = toupper(hand[i], loc); 
		auto start = get_time::now();
		// Map the character occurences in the hand and set blanks equal to the number or wildcards in the hand 
		characterFrequency(hand, freq); 
		blanks = freq['?']; 

		for (char const& i : branch)
		{
			// Adds branch character to the frequency map
			if (freq.find(i) == freq.end())
				freq[i] = 1; 
			else
				++freq[i];
			// Loop over each word in the wordlist
			for (string const& word : wordList) { 
				// Is the branch character in the word?
				if (word.find(i) != std::string::npos) 
				{
					// Is the word valid? If so we add it to the words set
					if (validWord(word, i, freq, blanks)) 
					{
						words.insert(word);
					}
				}
			}
			// Remove the occurence of the branch character in the frequency map
			freq[i]--;
		}

		auto end = get_time::now();
		auto diff = end - start;
		// Copys the set into a vector so it can be sorted and then sorts it based on the wordScore function
		vector<string> list(words.begin(), words.end()); 
		sort(list.begin(), list.end(), wordScore); 

		// Print the 50 highest scoreing words in the vector
		for (std::vector<int>::size_type i = max(0,int(list.size()-50)); i != list.size(); i++)
		{
			cout << list[i] << endl; 
		}

		cout << "Elapsed time is :  " << chrono::duration_cast<ns>(diff).count()/1000000 << " ms " << endl;
		cout << "Any key to continue `q` to quit:" << endl;

		words.clear();
		list.clear();
	}
}

void readFile(string strFile, set<string> &vecNames, ifstream &iFile)
{
	string word; 
	// Opens file
	iFile.open(strFile.c_str());

	// While the file is copying into words
	while (iFile >> word) 
	{
		// Pushes the word onto the back of the of the words vector
		vecNames.insert(word); 
	}
	// Close the input file
	iFile.close();
}

void characterFrequency(string text, map<char, uint8_t> &freq)
{
	// Clear the frequency for a new hand
	freq.clear(); 
	for (char&c : text) 
	{
		// For each character in the string if it is not already in the map add it an initalise it to 1 otherwise increment it's value by 1
		if (freq.find(c) == freq.end())
			freq[c] = 1;
		else
			++freq[c];
	}
}

inline bool validWord(string word, char i, map<char, uint8_t> hand, uint8_t blanks)
{
	// Keep a count of the amount of times a character is not in word
	uint8_t failed = 0; 
	for (char&c : word)
	{
		// For each character in word if the amount of times it appears in the hand is less than the number of times it appears in the word add a failure
		if (hand[c] < count(word.begin(), word.end(), c))
		{
			++failed;
			if (failed > blanks) {
				return false;
			}

		}
	}
	return true;
}

inline bool wordScore(string i, string j) 
{
	// Sum value score from letterValue map of each character for each string
	int scorei = 0, scorej = 0;
	for (char&c : i)
	{
		int tmp = letterValues[c];
		scorei += tmp;
	}
	
	for (char&c : j)
	{
		int tmp = letterValues[c];
		scorej += tmp;
	}
	return (scorei < scorej);
}
