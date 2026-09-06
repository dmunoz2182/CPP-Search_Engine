#include "search.h"
#include <iostream>
#include <iterator>
#include <cctype>
#include <sstream>
#include <locale> 
#include <fstream>
#include <utility>
#include <algorithm>

// this is the creative component that I decided to go with, and the functions purpose is
// to remove all stop words found in the "stop.txt" file from the built in already index,
// which is the only parameter it takes. The function then opens the file and reads in every 
// word and if the word it currently has, is in the index it removes it from the index, since
// it is a stop word. This is a helper function and is called in one location only, near the 
// end of the build index function. this function does mess with the original solution for this
// project, so I have commented it out.
void removeStopWords(
    SearchIndex& index,
    const std::string& stopWordsFile
) {
    std::ifstream file(stopWordsFile);
    if (!file.is_open()) {
        std::cerr
            << "Warning: unable to open stop-word file '"
            << stopWordsFile
            << "'.\n";
        return;
    }

    std::string word;
    while (file >> word) {
        word = cleanToken(word);

        if (!word.empty()) {
            index.erase(word);
        }
    }
}

// this function's purpose is to clean a string that is given within the function call. It 
// takes in a string as a parameter and uses it within the function. The way the function works
// is that we first check wether the string has a alphabetical letter, if it does then the word
// is valid and then we clean it, otherwise the word is invalid and thus the function returns a
// empty string. The cleaning process goes into effect if the word is valid, and the way it
// does this is we go through the size of the word and if there is punctuation at the end or
// beggining of the word we remove it. once all punctuation from the front or the back is 
// removed, we make all letters in the string lowercase and return that new cleaned token. 
std::string cleanToken(std::string token) {
    bool containsLetter = false;

    for (char ch : token) {
        if (std::isalpha(static_cast<unsigned char>(ch))) {
            containsLetter = true;
            break;
        }
    }

    if (!containsLetter) {
        return "";
    }

    while (
        !token.empty() &&
        std::ispunct(static_cast<unsigned char>(token.front()))
    ) {
        token.erase(token.begin());
    }

    while (
        !token.empty() &&
        std::ispunct(static_cast<unsigned char>(token.back()))
    ) {
        token.pop_back();
    }

    for (char& ch : token) {
        ch = static_cast<char>(
            std::tolower(static_cast<unsigned char>(ch))
        );
    }
    return token;
}

// this function's purpose is to take in a string filled with tokens, and clean each token
// within the string and return a set of tokens, each token being cleaned of any unwanted 
// punctuations, and being a simple word. The functions takes in one parameter being a string
// called "text", that is filled with tokens, that must be cleaned. The way I do this is to parse 
// through the string and get each word within the string separated by whitespace using string stream
// and clean that word, and i repeat this for the rest of the words in the string, one specification
// is that I do not put empty strings in the set taht is returned in the end. The end result is 
// a set of cleaned tokens all lowercase. 
std::set<std::string> gatherTokens(const std::string& text) {
    std::set<std::string> tokens;
    std::stringstream stream(text);
    std::string currentWord;

    while (stream >> currentWord) {
        currentWord = cleanToken(currentWord);

        if (!currentWord.empty()) {
            tokens.insert(currentWord);
        }
    }

    return tokens;
}

// this function's purpose is to create a reverse index based of the files contents. The parameters 
// taken in are fileName, wich is the file the user set up, and the map index which will be populated 
// in this function. The way we do this function is we set up a file stream, and we get the first  
// line which will be the url, and then we get the second line which will be the body text of the 
// url. We gather the tokens within the body of text, and store them within a set, we then traverse the
// set and if the word does not exist in the map we store it in there along with the respective url. if
// the word does exist then we store only the url of the word into the set of urls, that accompanies each
// word within the map. this function returns a count of how many url's were found in the file. 
int buildIndex(const std::string& filename, SearchIndex& index ) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr
            << "Error: unable to open '"
            << filename
            << "'\n";

        return 0;
    }

    std::string url;
    std::string pageText;
    int pageCount = 0;

    while (std::getline(file, url)) {
        
         // Ignore blank lines between entries.
        if (url.empty()) {
            continue;
        }

        if (!std::getline(file, pageText)) {
            break;
        }
        ++pageCount;
        const std::set<std::string> tokens =
            gatherTokens(pageText);
        for (const std::string& token : tokens) {
            index[token].insert(url);
        }
    }
    return pageCount;
}


std::set<std::string> setUnion(
    const std::set<std::string>& first,
    const std::set<std::string>& second
) {
    std::set<std::string> result;

    std::set_union(
        first.begin(),
        first.end(),
        second.begin(),
        second.end(),
        std::inserter(result, result.begin())
    );

    return result;
}

std::set<std::string> setIntersection(
    const std::set<std::string>& first,
    const std::set<std::string>& second
) {
    std::set<std::string> result;

    std::set_intersection(
        first.begin(),
        first.end(),
        second.begin(),
        second.end(),
        std::inserter(result, result.begin())
    );

    return result;
}

std::set<std::string> setDifference(
    const std::set<std::string>& first,
    const std::set<std::string>& second
) {
    std::set<std::string> result;

    std::set_difference(
        first.begin(),
        first.end(),
        second.begin(),
        second.end(),
        std::inserter(result, result.begin())
    );

    return result;
}

std::set<std::string> getMatchesForWord(
    const SearchIndex& index,
    const std::string& word
) {
    auto it = index.find(word);

    if (it == index.end()) {
        return {};
    }

    return it->second;
}

// this functions purpose is to show matches to a sentence the user inputs, depending on the 
// operations the user selected. The function takes in parameters index, which is the reverse index map,
// populated by the build index function. The last parameter taken in is the string sentece, which is
// the query sentece the user input in. The way the function works is that it reads the sentence word 
// by word from left to right using string stream and if the word has no "+" or "-", the we call the 
// union hepler function which just inserts the url for that word, and if the word does begin with
// "+" or "-", then we call the respective intersection or difference helper function. In the end we 
// return a set with the matches found in the sets depending on the set operations the user chose.
std::set<std::string> findQueryMatches( const SearchIndex& index, 
    const std::string& query ) {

    std::stringstream stream(query);
    std::string term;
    std::set<std::string> results;

    bool firstTerm = true;

    while (stream >> term) {
        char operation = ' ';
        if (!term.empty() && (term[0] == '+' || term[0] == '-')) {
            operation = term[0];
            term.erase(term.begin());
        }

        term = cleanToken(term);
        if (term.empty()) {
            continue;
        }

        std::set<std::string> matches =
            getMatchesForWord(index, term);
        if (firstTerm) {
            results = matches;
            firstTerm = false;
            continue;
        }

        if (operation == '+') {
            results = setIntersection(results, matches);
        }
        else if (operation == '-') {
            results = setDifference(results, matches);
        }
        else {
            results = setUnion(results, matches);
        }
    }
    return results;
}

// this functions purpose is to serve as the menu of operations for the whole program. 
// It basically connects every function and lets the user operate the program as intended. 
// The parameter taken in this function is filename, which is the name of the file the user wants to
// load. once the file is loaded we output the url count and the amount of unique terms in the file's
// index. then we enter a loop where the user enters a query sentence and it outputs matches for that 
// sentence. The loop continues until the user presses the enter key, wich ends the loop and the program.
void searchEngine(const std::string& filename) {
    SearchIndex index;
    std::cout << "Building search index...\n";
    int pagesIndexed = buildIndex(filename, index);

    if (pagesIndexed == 0) {
        std::cerr << "No pages were indexed.\n";
        return;
    }

    std::cout
        << "Indexed "
        << pagesIndexed
        << " pages containing "
        << index.size()
        << " unique terms.\n";

    std::string query;

    while (true) {
        std::cout << "\nEnter query (or press Enter to quit): ";
        std::getline(std::cin, query);

        if (query.empty()) {
            break;
        }

        std::set<std::string> results = findQueryMatches(index, query);

        std::cout
            << "Found "
            << results.size()
            << " matching page(s).\n";

        for (const std::string& url : results) {
            std::cout << "  " << url << '\n';
        }
    }

    std::cout << "\nSearch engine closed.\n";
}