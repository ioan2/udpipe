// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// Author: Johannes Heinecke johannes.heinecke(at)orange.fr

#include <iostream>
#include <fstream>
#include <sstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>

#include "common.h"
#include "morphodita/morpho/morpho_ids.h"
#include "morphodita/morpho/generic_morpho_encoder.h"

using namespace std;

using namespace ufal::udpipe;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "usage: " << argv[0] << " in:lexicon.txt out:lexicon.model" << endl;
        return 1;
    }

    unordered_set<string> dictionary_entries;
    ifstream ifp(argv[1]); // open lexicon outfile: entry list
    string line;
    while (!ifp.eof()) {
        getline(ifp, line);

        if (!line.empty()) {
            if (line.at(0) != '#') {
                dictionary_entries.insert(line);
            }
        }
    }
    ifp.close();
    cerr << dictionary_entries.size() << " entries read from '" << argv[1] << "'." << endl;

    string guesserfile = string(argv[1]) + ".guesser";
    ifstream ifp2(guesserfile); // open lexicon outfile: guesser description
    stringstream guesser_description;
    while (!ifp2.eof()) {
        getline(ifp2, line);
        if (!line.empty()) {
            if (line.at(0) != '#') {
                guesser_description << line << endl;
            }
        }
    }
    cerr << guesser_description.str().size() << " B of guesser_description read from '" << guesserfile << "'." << endl;
    ifp2.close();


    string specialtagfile = string(argv[1]) + ".special_tags";
    ifstream ifp3(specialtagfile); // open lexicon outfile: special tags

    morphodita::generic_morpho_encoder::tags dictionary_special_tags;
    dictionary_special_tags.unknown_tag = "~X";
    getline(ifp3, dictionary_special_tags.number_tag);
    getline(ifp3, dictionary_special_tags.punctuation_tag);
    getline(ifp3, dictionary_special_tags.symbol_tag);
    cerr << "special tags read from '" << specialtagfile << "'." << endl;
    ifp3.close();


    // Create the compiled dictionary
    vector<string> sorted_dictionary(dictionary_entries.begin(), dictionary_entries.end());

    sort(sorted_dictionary.begin(), sorted_dictionary.end());

    stringstream morpho_input;
    for (auto&& entry : sorted_dictionary) {
      morpho_input  << entry << '\n';
    }

    ofstream morpho_description(argv[2]);
    // taken as is from trainer_morphodita_parsito.cpp
    morpho_description.put(morphodita::morpho_ids::GENERIC);


    int dictionary_suffix_len = 8;
    morphodita::generic_morpho_encoder::encode(morpho_input, dictionary_suffix_len, dictionary_special_tags, guesser_description, morpho_description);
    morpho_description.close();


}