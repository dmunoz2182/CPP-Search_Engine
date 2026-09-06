// Project 2 Code - Search Engine
// Author: David Munoz
// Date: Sep 21, 2022
// Program Overview: In this Program we create a search engine, using a couple of
// of functions and using a inverted index, as the main ADT to accomplish this.
// Overall, when the user looks up a word, they will get a list of url's that contain
// the word the user searched for.

//Citation for stop.txt, the original author of the .txt file is Kavgan and she 
//listed hundreds of stop words in the english language and I used her .txt file
//as a resource to remove stop words from the index in whatever file is used by the 
//user. link to the original file
//(https://github.com/kavgan/stop-words/blob/master/terrier-stop.txt)

#include <iostream>
#include "search.h"
using namespace std;

// the purpose of this function is to test the cleanToken function.
// within the fucntion I send a couple of words to be cleaned, and if they
// are cleaned properly each if-statement should return "Worked".
void ctTest(){
  // TESTING FOR CLEAN TOKEN
  if (cleanToken("}}A") == "a"){
    cout << "Worked" << endl;
  }
  else{
    cout << "failed" << endl;
    cout << cleanToken("}}A") << endl;
  }
  if (cleanToken("...).The") == "the"){
    cout << "Worked" << endl;
  }
  else{
    cout << "failed" << endl;
    cout << cleanToken("...).The") << endl;
  }
  if (cleanToken("#@#@.David@uic.edu%@#.") == "david@uic.edu"){
    cout << "Worked" << endl;
  }
  else{
    cout << "failed" << endl;
    cout << cleanToken("David@uic.edu") << endl;
  }
}

// the purpose of this function is to test wether my gatherTokens function works.
// The way I test it, is at first I send a sentence filled with tokens to gatherTokens
// and in return it should return the same cleaned tokens within the answer key set,
// if the amount of tokens are the same, and the sets are the same then gatherTokens
// works.
void gtTest(){
  // TESTING FOR GATHER TOKENS
  set<string> tokens = gatherTokens("one two #three $four@");
  set<string> tokenANS;
  tokenANS.insert("one");
  tokenANS.insert("two");
  tokenANS.insert("three");
  tokenANS.insert("four");
  if ((tokens.size() == tokenANS.size()) && (tokens == tokenANS)){
    cout << "worked" << endl;
  }
  else{
    cout << "failed" << endl;
  }
  tokens = gatherTokens("#$@SAuce $@$CHICKEN... @CODE; $$COLD@#");
  tokenANS.clear();
  tokenANS.insert("sauce");
  tokenANS.insert("chicken");
  tokenANS.insert("code");
  tokenANS.insert("cold");
  if ((tokens.size() == tokenANS.size()) && (tokens == tokenANS)){
    cout << "worked" << endl;
  }
  else{
    cout << "failed" << endl;
    for(auto p: tokens){
      cout << p << endl;
    }
  }
}

// The purpose of this function is to test buildIndex function, first it test with the 
// tile "tiny.txt", and the index should have 20 words and find 4 urls, so we test the size,
// and if the size is the same as 20 and 4 urls then the function worked, and since tiny.txt
// is small we print out the index. Then we test for "cplusplus.txt", and within this function,
// there should be 86 pages found, and 1498 should be the size of the index, if those match,
// the function works.
void biTest(){
  // TESTING FOR BUILD INDEX
  map<string, set<string>> index2;
  int webCount = 0;
  webCount = buildIndex("tiny.txt", index2);
  cout << "matches " << index2.size() << endl;
  cout << "pages found " << webCount << endl;
  for(auto p: index2){
    cout << p.first << ": ";
    for (auto x: p.second){
      cout << x << ", ";
    }
    cout << endl;
  }
  if ((webCount == 4) && (index2.size() == 20)){
    cout << "--Worked--" << endl;
  }
  else{
    cout << "Failed";
  }
  index2.clear();
  webCount = buildIndex("cplusplus.txt", index2);
  cout << "pages found " << webCount << endl;
  cout << "matches " << index2.size() << endl;
  if ((webCount == 86) && (index2.size() == 1498)){
    cout << "--Worked--" << endl;
  }
  else{
    cout << "Failed";
  }
}

// The purpose of this function is to test the function FindQueryMatches, the way this
// is done is that we create a temp index, and fill it with the contetns of tiny.txt and input
// a query sentence then we should populate the set "match", with the url matches of that
// query sentence. if the number of matches match, then the function works, and we do this 
// multiple times, and then once with another file. 
void fqmTest(){
  // TESTING FOR FINDING QUERY MATCHES
  set<string> match;
  map<string, set<string>> index3;
  int webCount = 0;
  webCount = buildIndex("tiny.txt", index3);
  match = findQueryMatches(index3, "fish +blue");
  
  if(match.size() == 1){
    cout << "Worked" << endl;
  }
  else{
    cout << "false" << endl;
  }
  match = findQueryMatches(index3, "fish blue red");
  if(match.size() == 3){
    cout << "Worked" << endl;
  }
  else{
    cout << "false" << endl;
  }
  match = findQueryMatches(index3, "fish -eggs");
  if(match.size() == 1){
    cout << "Worked" << endl;
  }
  else{
    cout << "false" << endl;
  }
  webCount = buildIndex("cplusplus.txt", index3);
  match = findQueryMatches(index3, "vector +container -pointer");
  if(match.size() == 6){
    cout << "Worked" << endl;
  }
  else{
    cout << "false" << endl;
  }
}

// this is the function where we call all our test functions and we also test the 
// search engine function here, by calling it with different files loaded, and we input, 
// different query sentences, if the output from the program matches the description then the 
// search engine function works as expected. 
#include "search.h"

#include <iostream>

int main(int argc, char* argv[]) {

    if (argc < 2) {
            cerr
            << "Usage: "
            << argv[0]
            << " <data-file>\n";

        return 1;
    }

    searchEngine(argv[1]);

    return 0;
}
