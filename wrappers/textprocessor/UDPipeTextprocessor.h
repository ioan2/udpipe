#include <string>
#include "udpipe.h"
#include <iostream>



using namespace ufal::udpipe;
using std::string;
using std::ostream;
//using std::iostream;

class UDPipeTokenizer {

 public:
    UDPipeTokenizer(const string &modelfilename);
    ~UDPipeTokenizer();

    ostream & tokenize(const string &text, ostream &out, bool newpar = false);
    sentence * tokenize(const string &text);
 private:
    model *udpipemodel;
    input_format *tokenizer;
    string errors;
    output_format *udpipeoutput;


};


class UDPipeTagParser {
 public:
    UDPipeTagParser(const string &modelname, const string &lexmodelname = 0);
    ~UDPipeTagParser();

    ostream & tagparse(sentence* sent, ostream &out);
 private:
    model *udpipemodel;
    //pipeline *tagparser;
    output_format *udpipeoutput;
    string errors;

};


class UDPipeTextprocessor {
 public:
    UDPipeTextprocessor(const char *tokmodel, const char *tagmodel, const char *lexmodel);
    ~UDPipeTextprocessor();

    /** tokenise a sentence */
    ostream &tokenize(const string &text, ostream &out);

    /** tokenise, tag and parse a sentence */
    ostream &process_sentence(const string &text, ostream &out, bool newpar = false);
    
    /** process file (one sentence per line, empty line between paragraph) */
    ostream &process_file(const char *filename, ostream &out);

 private:

    UDPipeTokenizer *tokenizer;
    UDPipeTagParser *tagparser;

;
    
};
