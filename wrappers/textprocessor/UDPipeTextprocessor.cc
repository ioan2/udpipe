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
    tokenizer = udpipemodel->new_tokenizer(model::DEFAULT); //model::TOKENIZER_PRESEGMENTED);
}

UDPipeTokenizer::~UDPipeTokenizer() {
    delete tokenizer;
    delete udpipemodel;
}


// ostream & UDPipeTokenizer::tokenize(const string &text, ostream &out) {
//     tokenizer->reset_document();
//     tokenizer->set_text(text);
//     sentence *sent = new sentence();
//     //sent->set_new_doc(false);
//     //sent->set_new_par(true);

//     //cerr << "qeeeee " << sent->get_new_par() <<  text << endl;
//     bool n;
//     do {
//     
// 	n = tokenizer->next_sentence(*sent, errors);
// 	sent->set_new_doc(false);
// 	if (!errors.empty()) {
// 	    cout << "ERRORS: <" << errors << '>' << endl;
// 	} else {
// 	    //cerr << "   aaa " << sent->get_new_par() <<  endl;
// 	    udpipeoutput->write_sentence(*sent, out);
// 	    //sent->set_new_par(false);
// 	}
//     } while(n);

//     delete sent;
//     return out;
// }


// sentence * UDPipeTokenizer::tokenize( const string &text) {
//     tokenizer->set_text(text);
//     sentence *sent = new sentence();
//     sent->set_new_par(false);
//     sent->set_new_doc(false);
//     tokenizer->next_sentence(*sent, errors);
//     return sent;
// }


size_t UDPipeTokenizer::tokenize(vector<sentence *> &sentences, const string &text) {
    tokenizer->reset_document();
    tokenizer->set_text(text);
    bool n;
    do {
	sentence *sent = new sentence();
	n = tokenizer->next_sentence(*sent, errors);
	sent->set_new_doc(false);
	sentences.push_back(sent);
    } while(n);

    return sentences.size();
}


// --------------------------------- 
UDPipeTagParser::UDPipeTagParser(const string &modelname, const string &lexmodelname) {
    udpipemodel = model::load(modelname.c_str(), lexmodelname.c_str());
    //tagparser = new pipeline(udpipemodel, "conllu", pipeline::DEFAULT, pipeline::DEFAULT, "conllu");
}

UDPipeTagParser::~UDPipeTagParser() {
    //delete tagparser;
    delete udpipemodel;
}


bool UDPipeTagParser::tagparse(sentence* sent) {
    bool ok = udpipemodel->tag(*sent, model::DEFAULT, errors);
    if (ok) {
	ok = udpipemodel->parse(*sent, model::DEFAULT, errors);
    }
    return ok;
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
    udpipeoutput = output_format::new_output_format("conllu");
}

UDPipeTextprocessor::~UDPipeTextprocessor() {
    delete tokenizer;
    if (tagparser != 0) delete tagparser;
}



ostream &UDPipeTextprocessor::process_sentence(const string &text, ostream &out) {
    vector<sentence *>sentences;
    tokenizer->tokenize(sentences, text);
    

    if (tagparser) {
	for (auto it = sentences.begin(); it != sentences.end(); ++it) {
	    bool ok = tagparser->tagparse(*it);
	}
	//sentence *sent = tokenizer->tokenize(text);
	//vector<sentence *>sentences;
	//tokenizer->tokenize(sentences, text);

	//if (newpar) sent->set_new_par(true);
	//tagparser->tagparse(sent, out);
	//delete sent;
	//return out;
    }

    for (auto it = sentences.begin(); it != sentences.end(); ++it) {
	udpipeoutput->write_sentence(*(*it), out);
	delete *it;
    }

    return out;
}

/** processes line groups */
// ostream &UDPipeTextprocessor::process_file(const char *filename, ostream &out) {
//     istream *ifp;
//     if (filename == 0 || strlen(filename) == 0) ifp = &cin;
//     else ifp = new ifstream(filename);

//     vector<string>paragraph;

//     if (*ifp) {
// 	string line;
// 	while(!ifp->eof()) {
// 	    getline(*ifp, line);
// 	    if (!line.empty()) {
// 		paragraph.push_back(line);
// 	    } else {
// 		if (!paragraph.empty()) {
// 		    for (auto it = paragraph.begin(); it != paragraph.end(); ++it) {
// 			process_sentence(*it, out, (it == paragraph.begin()));
// 		    }
// 		    paragraph.clear();
// 		}
// 	    }
// 	}
//     }

//     if (!paragraph.empty()) {
// 	for (auto it = paragraph.begin(); it != paragraph.end(); ++it) {
// 	    process_sentence(*it, out);
// 	}
// 	paragraph.clear();
//     }
//     if (filename != 0 && strlen(filename) > 0) delete ifp;
// }


ostream &UDPipeTextprocessor::process_file(const char *filename, ostream &out) {
    istream *ifp;
    if (filename == 0 || strlen(filename) == 0) ifp = &cin;
    else ifp = new ifstream(filename);

    if (*ifp) {
 	string line;
	while(!ifp->eof()) {
	    getline(*ifp, line);
	    if (!line.empty()) {
		process_sentence(line, out);
	    }
	}
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
