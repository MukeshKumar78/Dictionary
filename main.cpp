#include <iostream>
#include <stdlib.h>
// #include <conio.h>
#include "Trie.hh"
#include <fstream>
using namespace std;

// #define clearScreen system("cls")
#define getFileName(str) "word"+str+".txt"
#define allWordsFile "AllWords.txt"
#define doStringHaveAlphasOnly(str) all_of(str.begin(), str.end(), ::isalpha)

// colors
#define BLUE system("color 01")
#define GREEN system("color 02")
#define RED system("color 04")
#define YELLOW system("color 06")
#define GREY system("color 08")
#define INVALID_RED system("color 4F")
#define CLEARSCREEN system("cls");

// functions

void menu(Trie &Dictionary);
void createBox(vector<string>);
void printProgramName(string str);
int takeChoice(int noOfChoice);
string getInputFor(string statement);
string takeChoiceFor(string statement);
void printInvalidStatement(string statement);
void printStatement(string statement);
void outputNumberOfTabs(int);
void printStatementWithData(string statement, string data);
void addDataFromFile(Trie &Dictionary);
void addDataToFile(Trie &Dictionary);
void addAWord(Trie &Dictionary);
void deleteAWord(Trie &Dictionary);
void searchAWord(Trie &Dictionary);
void Pause();


//

int main() {
    // clearScreen;
    Trie Dictionary;
    addDataFromFile(Dictionary);
    menu(Dictionary);
    addDataToFile(Dictionary);
}

void menu(Trie &Dictionary) {
    // clearScreen;

    printProgramName("Dictionary");

    vector<string> menu;
    menu.push_back("1.Add a word");
    menu.push_back("2.Delete a word");
    menu.push_back("3.Search a word");
    menu.push_back("4.Exit");
    createBox(menu);

    int choice = takeChoice(menu.size());
    switch(choice) {
        case 1:
            addAWord(Dictionary);
            break;
        case 2:
            deleteAWord(Dictionary);
            break;
        case 3:
            searchAWord(Dictionary);
            break;
        case 4:
            addDataToFile(Dictionary);
            exit(0);
        default:
            break;
    }
}
void addDataToFile(Trie &Dictionary) {
    fstream outFile;
    outFile.open(allWordsFile, ios::out);
    if(!outFile) {
        cout << "Could not open file " << allWordsFile << ", exiting...";
        exit(3);
    }
    vector <pair<string, int>> allWords = Dictionary.getAllWords();

    for(pair<string, int> word : allWords) {
        outFile << word.first << ' ' << word.second << endl;
    }
}

void addDataFromFile(Trie &Dictionary) {
    fstream wordFile;
    wordFile.open(allWordsFile, ios::in);
    if(!wordFile) {
        cout << "Could not open file AllWords.txt, exiting...";
        exit(2);
    }
    string temp;
    getline(wordFile, temp);
    while(!wordFile.eof()) {
        string word;
        int search;
        wordFile >> word;
        wordFile >> search;
        Dictionary.insert(word, search);
    }
}


void addAWord(Trie &Dictionary) {
    string input = getInputFor("Enter a word to insert : ");
    stringToUpper(input);
    Dictionary.insert(input);
    string filename = getFileName(input);
    fstream outFile;
    outFile.open(filename, ios::out);
    if(!outFile) {
        cout << "Could not open "<< getFileName(input) << " , exiting";
        exit(3);
    }
    // meaning of the word
    string meaning = takeChoiceFor("Enter word's meaning : ");
    outFile << input << endl;
    outFile << "Meaning : " << meaning << endl;

    // other details
    string details = takeChoiceFor("Enter other details : ");
    outFile << "Other details : " << details << endl;
    printStatement("Word added");
    Pause();
    menu(Dictionary);
}

void deleteAWord(Trie& Dictionary) {
    string input = getInputFor("Enter a word to delete : ");
    Dictionary.remove(input);
    printStatement(input + " is Deleted");
    string filename = getFileName(input);
    remove(filename.c_str());
    menu(Dictionary);
}

void searchAWord(Trie& Dictionary) {
    string input = getInputFor("Enter a word to search : ");
    if(Dictionary.search(input)) {
        printStatement("Found!!");
        fstream inFile;
        inFile.open(getFileName(input), ios::in);
        if(!inFile) {
        }
        string line;
        while(getline(inFile, line)) {
            printStatement(line);
        }
        cout << endl;
        Pause();
        menu(Dictionary);
    } else {
        printStatement("Not Found!!");
        vector<pair<string, int>> suggestion = Dictionary.getAllSuggestion(input);
        if(suggestion.size() > 0) {
          printStatement("Do you mean : ");
        }
        for(int i = 0; i  < suggestion.size(); i++) {
          if(i) cout << ",";
          cout << "\"" << suggestion[i].first << "\"";
        }
        searchAWord(Dictionary);
    }
}


void Pause()
{
  printStatement("\t\t\t\tPress Any Key To Continue...");
  // getch();
}

string getInputFor(string statement)
{
  printStatement(statement);
  string choice;
  getline(cin, choice);
  
  bool hasAlpha = doStringHaveAlphasOnly(choice);

  while (choice.length() < 1 || !hasAlpha)
  { // in characters 0 is 48 and 1 is 49
    if(!hasAlpha) {
      printInvalidStatement("Only english letters will be accepted!!");
    } else {
      printInvalidStatement("Nothing Entered!");
    }
    GREY;
    printStatement(statement);
    getline(cin , choice);
    hasAlpha = doStringHaveAlphasOnly(choice);
  }
  stringToUpper(choice);
  return choice;
}

string takeChoiceFor(string statement) {
  printStatement(statement);
  string choice;
  getline(cin, choice);
  return choice;
}


int takeChoice(int noOfChoice)
{
  string choice;
  choice = takeChoiceFor("Your choice : ");
  while (choice[0] < '1' || choice[0] > '1' + noOfChoice - 1 || choice.length() != 1)
  { // in characters 0 is 48 and 1 is 49
    printInvalidStatement("Invalid Option. Please choose correct option");
    GREY;
    choice = takeChoiceFor("Your choice : ");
  }
  return choice[0]-48; // -48 to change to number // +1 to start from for range between (1-noOfChoice)
}

void printStatementWithData(string statement, string data)
{
  cout << "\t\t";
  cout << statement;
  cout << data;
  cout << endl;
}

void printInvalidStatement(string statement)
{
  INVALID_RED;
  cout << endl;
  outputNumberOfTabs(6);
  cout << statement << endl;
  cout << endl;
  outputNumberOfTabs(6);
  Pause();
}

void printStatement(string statement)
{
  cout << "\n\t\t";
  cout << statement;
}


void outputNumberOfTabs(int n)
{
  while (n--)
  {
    cout << '\t';
  }
}

void outputNumberOfBox(int n)
{

  while (n--)
  {
    cout << "\xB2";
  }
}

void createBox(vector<string> s)
{
  int maxLength = 0;
  for (int i = 0; i < s.size(); i++)
  {
    if (s[i].length() > maxLength)
    {
      maxLength = s[i].length();
    }
  }

  int dLength = maxLength + 4 + 3; // 3 for spaces
  cout << endl;

  outputNumberOfTabs(7);

  outputNumberOfBox(dLength);
  cout << endl;

  outputNumberOfTabs(7);
  // empty line

  outputNumberOfBox(2);
  for (int i = 0; i < maxLength + 3; i++)
  { // additional 3 spaces  // 2 before text and 1 after text
    cout << ' ';
  }
  outputNumberOfBox(2);
  for (int i = 0; i < s.size(); i++)
  {
    cout << endl;

    outputNumberOfTabs(7);
    outputNumberOfBox(2);

    cout << "  " << s[i];
    int spaces = maxLength - s[i].length() + 1;
    while (spaces--)
    {
      cout << ' ';
    }
    outputNumberOfBox(2);
  }
  cout << endl;
  outputNumberOfTabs(7);
  outputNumberOfBox(2);
  for (int i = 0; i < maxLength + 3; i++)
  {
    cout << ' ';
  }
  outputNumberOfBox(2);
  cout << endl;

  outputNumberOfTabs(7);
  outputNumberOfBox(dLength);
  cout << "\n\n";

  s.erase(s.begin(), s.end());
}

void printProgramName(const string str)
{
  cout << "\n*************************************************************************************************************************************************************\n";
  cout << "\t\t\t\t\t\t\t\t| Welcome to " << str << " |";
  cout << "\n*************************************************************************************************************************************************************\n";
}