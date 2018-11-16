#include <string>
#include "udpipe.h"
#include <iostream>
#include <vector>


using namespace ufal::udpipe;
using std::string;
using std::ostream;
using std::vector;

class UDPipeTokenizer {

 public:
    UDPipeTokenizer(const string &modelfilename);
    ~UDPipeTokenizer();

    ostream & tokenize(const string &text, ostream &out);
    //sentence * tokenize(const string &text);
    size_t tokenize(vector<sentence *> &sentences, const string &text, bool newdoc = false);
 private:
    model *udpipemodel;
    input_format *tokenizer;
    string errors;


};


class UDPipeTagParser {
 public:
    UDPipeTagParser(const string &modelname, const string &lexmodelname = 0);
    ~UDPipeTagParser();

    bool tagparse(sentence* sent);
 private:
    model *udpipemodel;
    //pipeline *tagparser;
    string errors;

};


class UDPipeTextprocessor {
 public:
    UDPipeTextprocessor(const char *tokmodel, const char *tagmodel, const char *lexmodel);
    ~UDPipeTextprocessor();

    /** tokenise, tag and parse a sentence */
    ostream &process_line(const string &text, ostream &out, bool newdoc = false);
    
    /** process file (one sentence per line, empty line between paragraph) */
    ostream &process_file(const char *filename, ostream &out);

 private:

    UDPipeTokenizer *tokenizer;
    UDPipeTagParser *tagparser;
    output_format *udpipeoutput;
;
    
};
