//CITATION for stop.txt, the original author of the .txt file is Kavgan and she 
//listed hundreds of stop words in the english language and I used her .txt file
//as a resource to remove stop words from the index in whatever file is used by the 
//user. link to the original file
//(https://github.com/kavgan/stop-words/blob/master/terrier-stop.txt)

#pragma once
#include <map>
#include <set>
#include <string>

using SearchIndex = std::map<std::string, std::set<std::string>>;

std::string cleanToken(std::string token);

std::set<std::string> gatherTokens(const std::string& text);

int buildIndex(
    const std::string& filename,
    SearchIndex& index
);

std::set<std::string> findQueryMatches(
    const SearchIndex& index,
    const std::string& query
);

void removeStopWords(
    SearchIndex& index,
    const std::string& stopWordsFile
);

void searchEngine(const std::string& filename);
