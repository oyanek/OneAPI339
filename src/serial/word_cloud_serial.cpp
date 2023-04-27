#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <functional>
#include <sycl/sycl.hpp>
#include <CLI/CLI.hpp>

using namespace std;

// change to users exact path
const string DEF_DICT = "/home/u189146/OneAPI339/WOTW.txt";
const size_t DEF_BITS = 12000000;

string dict = DEF_DICT;
size_t bits = DEF_BITS;

vector<size_t> hash_words(bits, 0);

void hash_count(string word){
    hash<string> h;
    size_t hash = h(word);
    hash_words[hash % bits]++;
}

int main(const int argc, const char *const argv[]){
    CLI::App app{"Serial word cloud"};
    app.set_help_flag("-h,--help", "we need help too");
    CLI11_PARSE(app,argc,argv);

    ifstream ifs(dict);
    if (!ifs) {
        cerr << "Failed to open dictionary file: [" << dict << "] (suggest changing code to reflect exact path on your machine)" << endl;
        return 1;
    }

    int count1 = 0;
    string word;
    while (ifs >> word){
        hash_count(word);
        count1++;
    }
    ifs.close();

    //cout << "count1: " << count1 << endl;
    for (const auto& elem : hash_words) cout << elem;
    cout << endl << count1 << endl;
}