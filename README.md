# C++ Search Engine
A command-line search engine written in C++ that builds an inverted index
from document data and supports multi-term searches using set operations.

Successfully demonstrating the use of C++ data structures, file processing,
tokenization, indexing, and query evaluation.

## Features
- Builds an inverted index from document text
- Maps words to the pages where they appear
- Cleans and normalizes input tokens
- Supports multi-word queries
- Supports union, intersection, and difference operations
- Uses C++ STL containers and algorithms
- Handles invalid or missing input files
- Supports optional stop-word removal

## Query Syntax

The search engine supports three types of query operations.

### Union

Entering multiple words without a prefix returns pages containing any of
the terms.

Example:
python react

This returns pages containing either python or react.

### Intersection

Prefix a word with a "+" and that means that you only want pages with both terms in 
the same page.

Example: 
python +machine

This returns pages containing both python and react.

### Difference

Prefix a word with a "-" and that means that you dont want pages that include that 
word at all. 

Example: 
Python -machine

This retruns pages that include the word python but exclude machine. 

#### Technologies 
- C++
- C++ STL
- map
- sets
- File I/O
- STL algorithms 

#### Building the Project
This project requires a C++17-compatible compiler.

Clang/MacOS: 
clang++ -std=c++17 src/main.cpp src/search.cpp -o search_engine

GCC/linux: 
g++ -std=c++17 src/main.cpp src/search.cpp -o search_engine

#### Running the engine: 
./search_engine data/sample.txt

Output should be: 
    Building search index...
    Indexed 5 pages containing ... unique terms.

    Enter query (or press Enter to quit): 

#### Input file format 

- The input file stores alternating lines containing a URL and its
corresponding page text.

Example: 
https://example.com/python
Python is a popular programming language used for data science and machine learning.
https://example.com/web
Web development commonly uses JavaScript React HTML and CSS.

#### How it works 

The application first reads the document dataset and tokenizes each page.
Each token is normalized before being stored in an inverted index:

word --> set of URLs

Example: 
python -> {
    https://example.com/python,
    https://example.com/ai
}

Queries are evaluated using C++ set operations:
- Union uses std::set_union
- Intersection uses std::set_intersection
- Difference uses std::set_difference
This allows complex searches to be constructed from combinations of
search terms.

#### Future Improvements
- Command-line options for stop-word removal
- Ranked search results
- Term-frequency scoring
- Larger document datasets
- Unit tests
- Improved query parsing
- Web-based interface

#### What I learned 
- Designing and using inverted indexes
- Working with maps and sets in C++
- Parsing and normalizing text
- Processing files
- Implementing Boolean-style search operations
- Refactoring code into separate header and implementation files
- Improving program robustness and error handling
