#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "hashTable.h"

using namespace std;

void spellCheck(istream& inf, istream& wordlistfile, ostream& outf);

//read the wordlist into hashtable
void storeWordList(istream& wordlistfile, hashTable& wordTable);

//check if the word is legal
bool checkOneWord(string word, hashTable& wordTable);

void findSuggestions(string word, hashTable& wordTable, vector<string>& sgts);

void print(vector<string>& sgts, string line, string word, ostream& outf);

void convertToCap(std::string& s);

//check duplication before add to suggestion vector
void addSuggestion(string& s, vector<string>& sgts);

int main()
{
	cout << "Enter text file name: ";
	string filename;
	getline(cin, filename);
	ifstream infile(filename.c_str());
	if (!infile)
	{
		cout << "Cannot open " << filename << "!" << endl;
		return 1;
	}

	ifstream wlist("wordlist.txt");
	if (!wlist)
	{
		cout << "Cannot open wordlist!" << endl;
		return 1;
	}
	spellCheck(infile, wlist, cout);
}

void spellCheck(istream& inf, istream& wordlistfile, ostream& outf)
{
	hashTable wTable;
	storeWordList(wordlistfile, wTable);

	

	while (!inf.eof())
	{
		string line;
		getline(inf, line);

		stringstream ss;
		ss << line;

		while (!ss.eof())
		{
			string word;
			ss >> word;
			convertToCap(word);

			if (!checkOneWord(word, wTable))
			{
				vector<string> suggestions;
				findSuggestions(word, wTable, suggestions);
				print(suggestions, line, word, outf);
			}
		}

	}


}

void storeWordList(istream& wordlistfile, hashTable& wordTable)
{
	string line;
	getline(wordlistfile, line); //first line is a number indicating # of words
	while (!wordlistfile.eof())
	{
		getline(wordlistfile, line);
		wordTable.insert(line);
	}
}

//return true if the word IS in wordTable; return false otherwise
bool checkOneWord(string word, hashTable& wordTable)
{
	return wordTable.hasKey(word);
}

void findSuggestions(string word, hashTable& wordTable, vector<string>& sgts)
{
	string psblWord;
	string psblWord2;

	//Swapping each adjacent pair of characters in the word.
	for (int i = 0; i < word.length() - 1; i++)
	{
		psblWord = word;
		swap(psblWord[i], psblWord[i + 1]);
		if (wordTable.hasKey(psblWord))
		{
			convertToCap(psblWord);
			sgts.push_back(psblWord);
			addSuggestion(psblWord, sgts);
		}	
	}

	//In between each adjacent pair of characters in the word 
	//(also before the first character and after the last character),
	//each letter from 'A' through 'Z' is inserted.
	for (int i = 0; i <= word.length(); i++)
	{
		for (char c = 'a'; c <= 'z'; c++)
		{
			psblWord = word.substr(0, i) + string(1,c) + word.substr(i);
			if (wordTable.hasKey(psblWord))
			{
				convertToCap(psblWord);
				addSuggestion(psblWord, sgts);
			}
		}
	}

	//Deleting each character from the word.
	for (int i = 0; i < word.length(); i++)
	{
		psblWord = word.substr(0, i) + word.substr(i + 1);
		if (wordTable.hasKey(psblWord))
		{
			convertToCap(psblWord);
			addSuggestion(psblWord, sgts);
		}
	}

	//Replacing each character in the word with each letter from 'A' through 'Z'.
	for (int i = 0; i < word.length(); i++)
	{
		for (char c = 'a'; c <= 'z'; c++)
		{
			psblWord = word.substr(0, i) + string(1, c) + word.substr(i + 1);
			if (wordTable.hasKey(psblWord))
			{
				convertToCap(psblWord);
				addSuggestion(psblWord, sgts);
			}
		}
	}

	//Splitting the word into a pair of words by adding a space in between each adjacent pair of characters in the word.
	//It should be noted that this will only generate a suggestion if both words in the pair are found in the wordlist.
	for (int i = 1; i < word.length(); i++)
	{
		psblWord = word.substr(0, i);
		psblWord2 = word.substr(i);
		if (wordTable.hasKey(psblWord) && wordTable.hasKey(psblWord2))
		{
			convertToCap(psblWord);
			convertToCap(psblWord2);
			addSuggestion(psblWord + " " + psblWord2, sgts);
		}
	}

}

void print(vector<string>& sgts, string line, string word, ostream& outf)
{
	outf << line << endl;
	outf << "word not found: " << word << endl;
	outf << "perhaps you meant:" << endl;
	while (!sgts.empty())
	{
		int min = 0;
		for (int i = 0; i < sgts.size(); i++)
		{
			if (sgts[i] <= sgts[min])
				min = i;
		}
		swap(sgts[min], sgts[sgts.size() - 1]);
		outf << "  " << sgts.back() << endl;
		sgts.pop_back();
	}
	outf << endl;
}

void convertToCap(string& s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] <= 'z' && s[i] >= 'a')
			s[i] = s[i] - 'a' + 'A';
	}
}

void addSuggestion(string& s, vector<string>& sgts)
{
	for (int i = 0; i < sgts.size(); i++)
	{
		if (sgts[i] == s)
			return;
	}
	sgts.push_back(s);
}
