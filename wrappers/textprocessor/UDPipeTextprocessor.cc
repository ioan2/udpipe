#include "UDPipeTextprocessor.h"
#include <iostream>
#include <fstream>
#include <vector>

using std::ostream;
using std::istream;
using std::ifstream;
using std::vector;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

UDPipeTokenizer::UDPipeTokenizer(const string &modelfilename) {
    udpipemodel = model::load(modelfilename.c_str(), 0);
    tokenizer = udpipemodel->new_tokenizer(model::TOKENIZER_PRESEGMENTED);
    udpipeoutput = output_format::new_output_format("conllu");
}

UDPipeTokenizer::~UDPipeTokenizer() {
    delete udpipeoutput;
    delete tokenizer;
    delete udpipemodel;
}

ostream & UDPipeTokenizer::tokenize(const string &text, ostream &out) {
    tokenizer->set_text(text);
    sentence *sent = new sentence();
    //sentence sent; // = new sentence();
    tokenizer->next_sentence(*sent, errors);
    if (!errors.empty()) {
	cout << "ERRORS: <" << errors << '>' << endl;
    } else {
	udpipeoutput->write_sentence(*sent, out);
    }
    delete sent;
    return out;
}

sentence * UDPipeTokenizer::tokenize(const string &text) {
    tokenizer->set_text(text);
    sentence *sent = new sentence();
    tokenizer->next_sentence(*sent, errors);
    return sent;
}


// --------------------------------- 
UDPipeTagParser::UDPipeTagParser(const string &modelname, const string &lexmodelname) {
    udpipemodel = model::load(modelname.c_str(), lexmodelname.c_str());
    //tagparser = new pipeline(udpipemodel, "conllu", pipeline::DEFAULT, pipeline::DEFAULT, "conllu");
    udpipeoutput = output_format::new_output_format("conllu");
}

UDPipeTagParser::~UDPipeTagParser() {
    delete udpipeoutput;
    //delete tagparser;
    delete udpipemodel;
}


ostream & UDPipeTagParser::tagparse(sentence* sent, ostream &out) {
    bool ok = udpipemodel->tag(*sent, model::DEFAULT, errors);
    if (ok) {
	ok = udpipemodel->parse(*sent, model::DEFAULT, errors);
	if (ok)  udpipeoutput->write_sentence(*sent, out);
    }
    return out;
}




// --------------------------------- 
UDPipeTextprocessor::UDPipeTextprocessor(const char *tokmodel,
					 const char *tagmodel,
					 const char *lexmodel) {
    tokenizer = new UDPipeTokenizer(tokmodel);

    if (tagmodel != 0)
	tagparser = new UDPipeTagParser(tagmodel, lexmodel);
    else
	tagparser = 0;
}

UDPipeTextprocessor::~UDPipeTextprocessor() {
    delete tokenizer;
    if (tagparser != 0) delete tagparser;
}

ostream & UDPipeTextprocessor::tokenize(const string &text, ostream &out) {
    return tokenizer->tokenize(text, out);
}



ostream &UDPipeTextprocessor::process_sentence(const string &text, ostream &out) {
    if (tagparser) {
	sentence *sent = tokenizer->tokenize(text);
	tagparser->tagparse(sent, out);
	delete sent;
	return out;
    } else {
	return tokenizer->tokenize(text, out);
    }
}


ostream &UDPipeTextprocessor::process_file(const char *filename, ostream &out) {
    istream *ifp;
    if (filename == 0 || strlen(filename) == 0) ifp = &cin;
    else ifp = new ifstream(filename);

    vector<string>paragraph;

    if (*ifp) {
	string line;
	while(!ifp->eof()) {
	    getline(*ifp, line);
	    if (!line.empty()) {
		paragraph.push_back(line);
	    } else {
		if (!paragraph.empty()) {
		    for (auto it = paragraph.begin(); it != paragraph.end(); ++it) {
			process_sentence(*it, out);
		    }
		    paragraph.clear();
		}
	    }
	}
    }

    if (!paragraph.empty()) {
	for (auto it = paragraph.begin(); it != paragraph.end(); ++it) {
	    process_sentence(*it, out);
	}
	paragraph.clear();
    }
    if (filename != 0 && strlen(filename) > 0) delete ifp;
}


// --------------------------------- 
int main(int argc, char *argv[]) {
    if (argc < 2) {
	cerr << "usage: " << argv[0] << " [--tag parsemodel [--lex lexmodel]] tokmodel [fichier.txt]" << endl;
    } else {
	const char *tag = 0;
	const char *lex = 0;

	int i = 1;
	for( ; i<argc; ++i) { 
	    //cerr << "::: " << i <<" " << argv[i] << endl;
	    if (!strcmp(argv[i], "--tag")) {
		tag = argv[++i];
	    }
	    else if (!strcmp(argv[i], "--lex")) {
		lex = argv[++i];
	    }
	    else if (argv[i][0] == '-') {
		cerr << "invalid option '" << argv[i] << "'" << endl;
	    }
	    else break;
	}
	
	const char *fn = 0;
	if (i < argc-1) fn = argv[i+1];
// 	cout << endl <<  i << "\n tok:" << argv[i]
// 	     << "\n tag:" << (tag == 0 ? "NONE" : tag)
// 	     << "\n lex:" << (lex == 0 ? "NONE" : lex)
// 	     << "\n fich:" << (fn == 0 ? "NONE" : fn) << endl;
	
	try {
	    UDPipeTextprocessor *ud = new UDPipeTextprocessor(argv[i], tag, lex);
	    ud->process_file(fn, cout);

	    delete ud;
	} catch (std::logic_error e) {
	    cerr << "ERROR: " << e.what() << endl;
	}
	

    }
}
