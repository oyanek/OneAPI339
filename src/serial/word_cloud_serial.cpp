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
const size_t DEF_BITS = 1200000;
const int DEF_WIN_SIZE = 20000;

string dict = DEF_DICT;
size_t bits = DEF_BITS;
int win_size = DEF_WIN_SIZE;

vector<vector<size_t>> buffers;
//vector<unordered_map<size_t, size_t> buffers

void hash_count(string word, vector<size_t> buffer){
    hash<string> h;
    size_t hash = h(word);
    buffer[hash % bits]++;
}

int main(const int argc, const char *const argv[]){
    CLI::App app{"Serial word cloud"};
    app.option_defaults()->always_capture_default(true);
    app.add_option("-w,--window-size", win_size, "window size")->check(CLI::PositiveNumber.description(" >= 1"));
    app.add_option("-b,--bit-size", bits, "bit size (greater than 1000000 recommend)")->check(CLI::PositiveNumber.description(" >= 1"));
    app.add_option("-d,--dict", dict, "set dictionary real path");
    CLI11_PARSE(app,argc,argv);

    ifstream ifs(dict);
    if (!ifs) {
        cerr << "Failed to open dictionary file: [" << dict << "] (suggest changing code to reflect exact path on your machine)" << endl;
        return 1;
    }

    auto start = chrono::high_resolution_clock::now();

    int count1 = 0;
    string word;
    while (ifs >> word){
        if(count1%win_size==0){
            cout << "Checkpoint " << count1 << endl;
            buffers.emplace_back(bits, 0);
        }
        hash_count(word, buffers.back());
        count1++;
    }
    ifs.close();

    cout << count1 << endl;

    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end-start);
    cout << "Elapsed time: " << elapsed.count()/1000 << " seconds" << endl;

    int count2 = 0;
    // for(auto b : buffers){
    //     count2++;
    //     cout << "Buffer " << count2 << ":" << endl;
    //     int i = 0;
    //     for(const auto hash : b){
    //         if(hash > 0) cout << i << ": " << hash << endl;
    //         i++;
    //     }
    // }
}