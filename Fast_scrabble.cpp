// ConsoleApplication4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <cstdio>
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
void characterFrequency(string text, map<char, unsigned __int8> &freq); //Function modifies freq to be a character frequency of that string
inline bool validWord(string word, char i, map<char, unsigned __int8> hand, unsigned __int8 blanks); //Bool can the word be made with the hand

struct myclass {
    map<char, int> letterValues = { {'A',1 },{'B',4},{'C',4},{'D',2},{'E',1},{'F' , 4}, {'G' , 3}, {'H' , 3}, {'I' , 1}, {'J' , 10}, {'K' , 5}, {'L' , 2 }, { 'M' , 4 }, { 'N' , 2 }, { 'O' , 1 }, { 'P' , 4 }, { 'Q' , 10 }, { 'R' , 1 }, { 'S', 1 }, { 'T' , 1 }, { 'U', 2 }, { 'V',5 }, { 'W',4 }, { 'X',8 }, { 'Y',3 }, { 'Z',10 } };
	bool operator() (string i, string j) { 
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
		return (scorei<scorej);
	}
} wordScore;

int main()
{
	ifstream inFile; //Input file
	string filename = "words.txt";
	locale loc;
	string quit;
	set<string> wordList;
	vector<string>::iterator word;  // declare an iterator to a vector of strings
	set<string> words;
	map<char, unsigned __int8> freq;
	unsigned __int8 blanks;
	string branch;
	string hand;
	auto time = get_time::now();
	
	readFile(filename, wordList, inFile);
	cout << "Any key to continue `q` to quit:" << endl;
	while (getline(cin, quit) && quit != "q")
	{
		cout << "Enter the tiles already on the board: ";
		getline(cin, branch);
		cout << "Enter the tiles that are in your hand: ";
		getline(cin, hand);
		for (std::string::size_type i = 0; i<branch.length(); ++i)
			branch[i] = toupper(branch[i], loc);
		for (std::string::size_type i = 0; i<hand.length(); ++i)
		    hand[i] = toupper(hand[i], loc);
		auto start = get_time::now();
		characterFrequency(hand, freq);
		blanks = freq['?'];

		for (char const& i : branch)
		{
			if (freq.find(i) == freq.end())
				freq[i] = 1;
			else
				++freq[i];

			for (string const& word : wordList) {
				if (word.find(i) != std::string::npos)
				{
					if (validWord(word, i, freq, blanks))
					{
						words.insert(word);
					}
				}
			}
			freq[i]--;
		}

		auto end = get_time::now();
		auto diff = end - start;

		vector<string> list(words.begin(), words.end());
		sort(list.begin(), list.end(), wordScore);

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

void readFile(string strFile, set<string> &vecNames, ifstream &iFile) //Read the file into the vector function definition
		 {
	string word; 

	iFile.open(strFile.c_str()); //Opens file

	while (iFile >> word) //While the file is copying into the first and last names 
	{
		//cout << word;
		//sort(word.begin(), word.end());
		vecNames.insert(word); //Push the names onto the back of the of the vector
	}

	iFile.close(); //Close the input file
}

void characterFrequency(string text, map<char, unsigned __int8> &freq)
{
	freq.clear();
	for (char&c : text)
	{
		if (freq.find(c) == freq.end())
			freq[c] = 1;
		else
			++freq[c];
	}
}

inline bool validWord(string word, char i, map<char, unsigned __int8> hand, unsigned __int8 blanks)
{
	/*
	if (word.find(i) == std::string::npos)
	{
		return false;
	}
	*/
	unsigned __int8 failed = 0;
	//sort(word.begin(), word.end());
	for (char&c : word)
	{
		if (hand[c]<count(word.begin(),word.end(), c))
		{
			++failed;
			if (failed > blanks) {
				return false;
			}
			
		}
	}
	return true;
}
