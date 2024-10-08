#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm> // For std::transform

/*
UserId=Moinul
Password=Moinul909
*/

const std::string USER_ID = "Moinul";
const std::string PASSWORD = "Moinul909";

// Function to convert a string to lowercase
std::string toLowerCase(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Trie Node class
class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children; // Child nodes of the current node
    bool isEndOfWord;                             // Indicates if a word ends at this node

    TrieNode() : isEndOfWord(false) {}
};

// Trie class for inserting and searching words
class Trie {
private:
    TrieNode* root;

public:
    Trie() { root = new TrieNode(); }

    // Insert a word into the Trie
    void insert(const std::string& word) {
        TrieNode* node = root;
        std::string lowerWord = toLowerCase(word); // Convert word to lowercase
        for (char c : lowerWord) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEndOfWord = true;
    }

    // Search for a prefix in the Trie and collect all words with that prefix
    void searchPrefix(TrieNode* node, std::string prefix, std::vector<std::string>& results) {
        if (node->isEndOfWord) {
            results.push_back(prefix);
        }
        for (auto& child : node->children) {
            searchPrefix(child.second, prefix + child.first, results);
        }
    }

    // Return a list of words in the Trie that start with the given prefix
    std::vector<std::string> autocomplete(const std::string& prefix) {
        TrieNode* node = root;
        std::string lowerPrefix = toLowerCase(prefix); // Convert prefix to lowercase
        for (char c : lowerPrefix) {
            if (node->children.find(c) == node->children.end()) {
                return {};  // Return an empty list if the prefix is not found
            }
            node = node->children[c];
        }

        // Collect all words that start with the given prefix
        std::vector<std::string> results;
        searchPrefix(node, lowerPrefix, results);
        return results;
    }

    // Function to collect all words stored in the Trie
    void collectAllWords(TrieNode* node, std::string currentWord, std::vector<std::string>& allWords) {
        if (node->isEndOfWord) {
            allWords.push_back(currentWord);
        }
        for (auto& child : node->children) {
            collectAllWords(child.second, currentWord + child.first, allWords);
        }
    }

    // Function to get all words in the Trie
    std::vector<std::string> getAllWords() {
        std::vector<std::string> allWords;
        collectAllWords(root, "", allWords);
        return allWords;
    }
};

// Login function to check user credentials
bool login() {
    std::string userId, password;
    std::cout << "Enter User ID: ";
    std::cin >> userId;
    std::cout << "Enter Password: ";
    std::cin >> password;

    return (userId == USER_ID && password == PASSWORD);
}

// Main function to demonstrate the Text Autocomplete System
int main() {
    if (!login()) {
        std::cout << "Invalid credentials. Access denied.\n";
        return 1;
    }

    std::cout << "Login successful. Welcome to the Text Autocomplete System.\n";

    Trie trie;
    int option;
    std::string word, prefix;

    // Menu for inserting words, using autocomplete, and viewing all words
    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. Insert a word into the Trie\n";
        std::cout << "2. Autocomplete using a prefix\n";
        std::cout << "3. View all words stored in the Trie\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> option;

        switch (option) {
            case 1:
                std::cout << "Enter a word to insert: ";
                std::cin >> word;
                trie.insert(word);
                std::cout << "Word '" << word << "' inserted successfully.\n";
                break;

            case 2:
                std::cout << "Enter a prefix to search for autocomplete suggestions: ";
                std::cin >> prefix;
                {
                    std::vector<std::string> suggestions = trie.autocomplete(prefix);
                    if (suggestions.empty()) {
                        std::cout << "No suggestions found for prefix '" << prefix << "'.\n";
                    } else {
                        std::cout << "Suggestions for prefix '" << prefix << "':\n";
                        for (const auto& suggestion : suggestions) {
                            std::cout << "- " << suggestion << "\n";
                        }
                    }
                }
                break;

            case 3:
                {
                    std::vector<std::string> allWords = trie.getAllWords();
                    if (allWords.empty()) {
                        std::cout << "The Trie is empty. No words stored.\n";
                    } else {
                        std::cout << "All words stored in the Trie:\n";
                        for (const auto& storedWord : allWords) {
                            std::cout << "- " << storedWord << "\n";
                        }
                    }
                }
                break;

            case 4:
                std::cout << "Exiting the Text Autocomplete System.\n";
                break;

            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }
    } while (option != 4);

    return 0;
}
