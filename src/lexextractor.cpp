#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>

#include "common.h"

#include "model/model_morphodita_parsito.h"
#include "morphodita/morpho/morpho.h"
#include "morphodita/morpho/morpho_ids.h"
#include "morphodita/morpho/generic_morpho.h"
#include "utils/new_unique_ptr.h"

using namespace std;

using namespace ufal::udpipe;
using namespace ufal::udpipe::morphodita;

int main(int argc, char* argv[]) {
    if (argc < 2) {
	cerr << "extract lexicon from UDPipe model" << endl
	     << "usage: " << argv[0] << " udpipe.model" << endl;
        return 1;
    }

    ifstream newlex(argv[1]);
    if (!newlex) {
	cerr << "Cannot open external '" << argv[1] << "'" << endl;
	return 1;
    }
    cerr << "Reading external '" << argv[1] << "'" << endl;
    unique_ptr<model> model;

    model::load(argv[1], "__LEXOUT__");


    //delete res;

}
