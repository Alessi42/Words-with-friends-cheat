#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdint>
#include <fstream>
#include <string>
#include <locale>         // std::locale, std::toupper
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <chrono>

using namespace std;
using ns = chrono::nanoseconds;
using get_time = chrono::steady_clock;

void readFile(string, set<string> &, ifstream &); //Function prototype for the reading the file function
void characterFrequency(string text, map<char, uint8_t> &freq); //Function modifies freq to be a character frequency of that string
inline bool validWord(string word, char i, map<char, uint8_t> hand, uint8_t blanks); //Bool can the word be made with the hand
inline bool wordScore(string i, string j); //Bool sorting which string has higher wordscore

map<char, int> letterValues = { { 'A',1 },{ 'B',4 },{ 'C',4 },{ 'D',2 },{ 'E',1 },{ 'F' , 4 },{ 'G' , 3 },{ 'H' , 3 },{ 'I' , 1 },{ 'J' , 10 },{ 'K' , 5 },{ 'L' , 2 },{ 'M' , 4 },{ 'N' , 2 },{ 'O' , 1 },{ 'P' , 4 },{ 'Q' , 10 },{ 'R' , 1 },{ 'S', 1 },{ 'T' , 1 },{ 'U', 2 },{ 'V',5 },{ 'W',4 },{ 'X',8 },{ 'Y',3 },{ 'Z',10 } };
ifstream inFile; //Input file
string filename = "words.txt"; //Input file name
locale loc; // locale for converting to uppercase
string quit; 
set<string> wordList; // set of all english words in words.txt
vector<string>::iterator word;  // declare an iterator to a vector of strings
set<string> words; // all words that are matched
map<char, uint8_t> freq; // amount of times each character appears in hand
uint8_t blanks; // number of '?' wildcards
string branch; // the string of characters already on the board you're branching off of
string hand; // the string of characters you have in your hand

int main()
{
	auto time = get_time::now();
	
	readFile(filename, wordList, inFile); // reads the contents of filename into the wordlist set
	cout << "Any key to continue `q` to quit:" << endl;
	while (getline(cin, quit) && quit != "q") // Loops while the user doesn't want to quit
	{
		cout << "Enter the tiles already on the board: ";
		getline(cin, branch);
		cout << "Enter the tiles that are in your hand: ";
		getline(cin, hand);
		for (std::string::size_type i = 0; i<branch.length(); ++i)
			branch[i] = toupper(branch[i], loc);
		for (std::string::size_type i = 0; i<hand.length(); ++i)
		    hand[i] = toupper(hand[i], loc); // converts the inputed strings to uppercase to match the wordlist
		auto start = get_time::now();
		characterFrequency(hand, freq); // gets a map of the amount of times each character occurs in a given hand, {{'h',1},{'a',2}}
		blanks = freq['?']; // set blanks equal to the number or wildcards in the hand 

		for (char const& i : branch) // loop over each character a word can branch off of
		{
			if (freq.find(i) == freq.end())
				freq[i] = 1; // adds the character to the frequency map if it is not in there already
			else
				++freq[i]; // otherwise increments the count by 1

			for (string const& word : wordList) { // loop over each word in the wordlist 
				if (word.find(i) != std::string::npos) // terminate early if the branch character is not in the word
				{
					if (validWord(word, i, freq, blanks)) // if the word can be made with the hand add it to the list of found words
					{
						words.insert(word);
					}
				}
			}
			freq[i]--; // removes the count of the branch character 
		}

		auto end = get_time::now();
		auto diff = end - start;

		vector<string> list(words.begin(), words.end()); // copy the set to the variable list
		sort(list.begin(), list.end(), wordScore); // sort the list by the custom sort wordscore that sums the value of each character

		for (std::vector<int>::size_type i = max(0,int(list.size()-50)); i != list.size(); i++) // loop over last 50 elements in the list
		{
			cout << list[i] << endl; 
		}

		cout << "Elapsed time is :  " << chrono::duration_cast<ns>(diff).count()/1000000 << " ms " << endl;
		cout << "Any key to continue `q` to quit:" << endl;

		words.clear();
		list.clear();
	}
}

void readFile(string strFile, set<string> &vecNames, ifstream &iFile) //Read the file into the vector function definition
{
	string word; 

	iFile.open(strFile.c_str()); //Opens file

	while (iFile >> word) //While the file is copying into the first and last names 
	{
		vecNames.insert(word); //Push the names onto the back of the of the vector
	}

	iFile.close(); //Close the input file
}

void characterFrequency(string text, map<char, uint8_t> &freq)
{
	freq.clear(); // clears the old frequency character map 
	for (char&c : text) // loop for each char in string
	{
		if (freq.find(c) == freq.end()) // if the character is not already in the map add it an initalise it to 1
			freq[c] = 1;
		else
			++freq[c]; // if already in map increment its value by 1
	}
}

inline bool validWord(string word, char i, map<char, uint8_t> hand, uint8_t blanks)
{

	uint8_t failed = 0; // keeps a count of the amount of times a character is not in word used for blanks
	for (char&c : word)
	{
		if (hand[c] < count(word.begin(), word.end(), c)) // if character not in hand
		{
			++failed;
			if (failed > blanks) {
				return false;
			}

		}
	}
	return true; // if no errors after looping over all characters return true yay
}

inline bool wordScore(string i, string j) 
{
	int scorei = 0, scorej = 0;
	for (char&c : i)
	{
		int tmp = letterValues[c];
		scorei += tmp;
	}
	// sum value score from letterValue map of each character
	for (char&c : j)
	{
		int tmp = letterValues[c];
		scorej += tmp;
	}
	return (scorei < scorej);
}
