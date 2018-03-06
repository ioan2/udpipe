#ifndef __PRINTVECTORS_H__
#define __PRINTVECTORS_H__

#include <vector>
#include <set>
#include <map>
#include <unordered_map>


using std::ostream;
using std::vector;
using std::set;
using std::map;
using std::unordered_map;

namespace ufal {
    namespace udpipe {
	namespace morphodita {
	    
	    // JHE pretty print a vector
	    template <class T>ostream& operator<< (ostream &out, vector<T> &vs) {
		out << '[';
		typename vector<T>::iterator vi = vs.begin();
		if (vi != vs.end()) {
		    out << *vi;
		    for (++vi;
			 vi != vs.end(); ++vi) {
			out << ',' << *vi;
		    }
		}
		out << ']';
		return out;
	    }


	    // JHE pretty print a set
	    template <class T>ostream& operator<< (ostream &out, set<T> &vs) {
		out << '[';
		typename set<T>::iterator vi = vs.begin();
		if (vi != vs.end()) {
		    out << *vi;
		    for (++vi;
			 vi != vs.end(); ++vi) {
			out << ',' << *vi;
		    }
		}
		out << ']';
		return out;
	    }

	    template <class T, class U, class V, class W>ostream& operator<< (ostream &out, map<T,U,V,W> &vs) {
		out << '{';
		typename map<T,U,V,W>::iterator vi = vs.begin();
		if (vi != vs.end()) {
		    out << vi->first << ':' << vi->second;
		    for (++vi;
			 vi != vs.end(); ++vi) {
			out << ',' << vi->first << ':' << vi->second;
		    }
		}
		out << '}';
		return out;
	    }


	    template <class T, class U, class V, class W>ostream& operator<< (ostream &out, unordered_map<T,U,V,W> &vs) {
		out << '{';
		typename unordered_map<T,U,V,W>::iterator vi = vs.begin();
		if (vi != vs.end()) {
		    out << vi->first << ':' << vi->second;
		    for (++vi;
			 vi != vs.end(); ++vi) {
			out << ',' << vi->first << ':' << vi->second;
		    }
		}
		out << '}';
		return out;
	    }


	}
    }
}

#endif
