#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

#define stringToUpper(x) transform(x.begin(), x.end(), x.begin(), ::toupper)
#define AlphabetSize 26
#define CHAR_TO_INDEX(c) (int(c) - (int) 'A')

class Trie {
    class Node {
        public:
        Node* children[AlphabetSize]; // 26 letters in english alphabet
        bool isEndOfWord; // if letter is end of word 
        int searchedTimes = 0;
        public:
        Node() {
            for(int i = 0; i < AlphabetSize; i++) {
                children[i] = NULL;
            }
            isEndOfWord = false;
            searchedTimes = 0;
        }
    };

    void merge(vector<pair<string, int>> &allWords, int left, int mid, int right) {
        int n1 = mid-left+1;
        int n2 = right-mid;

        pair <string, int> leftArr[n1], rightArr[n2];

        for(int  i = 0; i < n1; i++) {
            leftArr[i] = allWords[left+i];
        }
        for(int i = 0; i < n2; i++) {
            rightArr[i] = allWords[mid + i + 1];
        }

        int i = 0;
        int j = 0;
        int k = left;

        while(i < n1 && j < n2) {
            if(leftArr[i].second >= rightArr[j].second) {
                allWords[k] = leftArr[i];
                i++;
            } else {
                allWords[k] = rightArr[j];
                j++;
            }
            k++;
        }

        while(i < n1) {
            allWords[k] = leftArr[i];
            i++;
            k++;
        }

        while(j < n2) {
            allWords[k] = rightArr[j];
            j++;
            k++;
        }
    }


    void mergeSort(vector<pair<string, int>> &allWords, int left, int right) {
        if(left < right) {
            int mid = (right+left)/2;
            mergeSort(allWords, left, mid);
            mergeSort(allWords, mid+1, right);
            merge(allWords, left, mid, right);
        }
    }



    bool hasNoChildren(Node* r) {
        for(int i = 0; i < AlphabetSize; i++) {
            if(r->children) {
                return false;
            }
        }
        return true;
    }
    Node* deleteNode(Node* tempRoot, string word, int index = 0) {
        if(!tempRoot) {
            return NULL;
        }
        if(index == word.size()) {
            if(tempRoot->isEndOfWord == true) {
                tempRoot->isEndOfWord = false;
            }
            
            if(hasNoChildren(tempRoot)) {
                delete tempRoot;
            }
            return tempRoot;
        }

        int hashIndex = CHAR_TO_INDEX(word[index]);
        tempRoot->children[hashIndex] = deleteNode(tempRoot->children[hashIndex], word, index+1);

        if(hasNoChildren(tempRoot) && tempRoot->isEndOfWord == false) {
            delete tempRoot;
        }
        return tempRoot;
    }

    void getAllWords(Node* r, vector<string> &vecString, string a = "", int level = 0) {
        if(r->isEndOfWord) {
            vecString.push_back(a);
            a.clear();
        }

        for(int i = 0; i < AlphabetSize; i++) {
            if(r->children[i]) {
                if(a.size() < level+1) {
                    a.resize(level+1);
                }
                a[level] = i+'A';
                getAllWords(r->children[i], vecString, a, level+1);
            }
        }
    }

    void deleteAllTrie(Node* tempRoot) {
        if(!tempRoot) 
        return;

        if(hasNoChildren(tempRoot)) {
            delete[] tempRoot;
        } else {
            for(int i = 0; i < AlphabetSize; i++) {
                deleteAllTrie(tempRoot->children[i]);
            }
        }
    }


    void getSuggestions(Node *tempRoot, vector<pair<string, int>> &allSuggestions, string prefix) {
        if(tempRoot->isEndOfWord) {
            allSuggestions.push_back(make_pair(prefix, tempRoot->searchedTimes));
        }

        if(hasNoChildren(tempRoot)) {
            return;
        }

        for(int i = 0; i < AlphabetSize; i++) {
            if(tempRoot->children[i]) {
                prefix.push_back(i+'A');
                getSuggestions(tempRoot->children[i],allSuggestions, prefix);
                prefix.pop_back();
            }   
        }
    }

    void getAutoSuggestions(Node *tempRoot, vector<pair<string, int>> &allSuggestions, const string word) {
        Node *traverse = root;

        int n = word.length();
        for(int level = 0; level < n; level++) {
            int index = CHAR_TO_INDEX(word[level]);
            if(!traverse->children[index]) {
                return;
            }
            traverse = traverse->children[index];
        }

        // checking if full word is present
        bool isWord = (traverse->isEndOfWord == true);

        // checking if last letter of word is last node of tree 
        bool isLast = hasNoChildren(traverse);

        if(isWord && isLast) {
            allSuggestions.push_back(make_pair(word, traverse->searchedTimes));
            return;
        }

        if(isLast == false) {
            string prefix = word;
            getSuggestions(traverse, allSuggestions, prefix);
        }
    }
    
    Node* root;
    public:
    Trie() {
        root = new Node();
    }
    ~Trie() {
        deleteAllTrie(root);
    }
    Trie& operator=(Trie&a) {
        if(this != &a) {
            deleteAllTrie(root);
            root = new Node();
            vector<string> allWords;
            getAllWords(a.root, allWords);
            for(string word : allWords) {
                this->insert(word);
            }
        }
        return *this;
    }
    Trie(Trie &a) {
        root = new Node();
        vector<string> allWords;
        getAllWords(a.root, allWords);
        for(string word : allWords) {
            this->insert(word);
        }
    }
    void insert(string word, int searchedTimes = 0) {
        stringToUpper(word);
        Node* traverse = root;
        for(int i = 0; i < word.length(); i++) {
            int index = CHAR_TO_INDEX(word[i]);
            if(!traverse->children[index]) {
                traverse->children[index] = new Node();
            }
            traverse = traverse->children[index];
        }
        traverse->isEndOfWord = true;
        traverse->searchedTimes = searchedTimes;
    }

    void printAllWords() {
        vector<string> allWords;
        getAllWords(root, allWords);
        for(string word : allWords) {
            cout << word << ' ';
        }
        cout << endl;
    }

    vector<pair<string, int>> getAllWords() {
        vector<pair<string, int>> allWords;
        getAutoSuggestions(root, allWords, "");
        return allWords;
    }

    vector < pair < string, int>> getAllSuggestion(string word) {
        stringToUpper(word);
        vector <pair <string, int>> allSuggestions;
        getAutoSuggestions(root, allSuggestions, word);
        
        while(allSuggestions.size() == 0 && word.size() > 0) {
            word.pop_back();
            getAutoSuggestions(root, allSuggestions, word);
        }
        
        mergeSort(allSuggestions, 0, allSuggestions.size()-1);
        return allSuggestions;
        // cout << endl;
    }

    bool search(string word) {
        stringToUpper(word);
        Node* traverse = root;
        for(int i = 0; i < word.length(); i++) {
            int index = CHAR_TO_INDEX(word[i]);
            if(!traverse->children[index]) {
                return false;
            }
            traverse = traverse->children[index];
        }

        // if node is last letter of word and it is not null return true
        if(traverse != NULL && traverse->isEndOfWord) {
            traverse->searchedTimes++;
            return true;
        } else {
            return false;
        }
    }  
    void remove(string word) {
        stringToUpper(word);
        root = deleteNode(root, word); // delete node recursively 
    }
};