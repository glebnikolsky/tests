// cpp-school.cpp : Defines the entry point for the console application.
//

#include <ctype.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#ifdef _UNICODE || UNICODE || _MBCS
#error ANSI only!!!!! Not multybyte
#endif



using namespace std;

size_t topN = numeric_limits<size_t>::max();

void Usage( )
{
    cerr<<"Bad args!\n";
    cerr<<"cppschool [-n NNN] in.txt out.txt\n";
    exit(1);
}

void CantOpen(string const &fn)
{
    cerr<<"Can't open "<<fn;
    exit(2);
}

string GetObj(string &line, int &pos, char *list)
{
    string s;
    while(pos < line.length() && (isalnum(line[pos]) || strchr(list,line[pos]) != NULL))  s += line[pos++];
    return s;
}


int ProcLine(string &line, map<string, int> &domain, map<string, int>&path)
{
    int pos = 0;
    int url = 0;
    while( (pos = line.find("http", pos) ) != -1){
        if ( (pos && isspace(line[pos-1])) || !pos ){ 
            //string tmp = line.substr(pos,7);
            if( line.substr(pos,7) == string("http://") ) pos += 7;
            else if ( line.substr(pos,8) == string("https://")) pos += 8;
            else {
                pos += 4; 
                continue;
            }
            string d = GetObj(line, pos,".-");
            string p = GetObj(line, pos,".,/+_");
            p = p.empty() ? "/" : p;
            if( !d.empty() ){
                domain[d]++;
                path[p]++;
                url++;
            }
        }else pos+=4;
    }
    return url;
}

struct HelperStruct{
    int n_;
    string s_;

    HelperStruct(int n, string s):n_(n), s_(s){}
    bool operator<(HelperStruct const &r)
    {
        return n_ > r.n_ || (n_ == r.n_ && s_ < r.s_);
    }

    operator string(){
        ostringstream tmp;
        tmp<<n_<<' '<<s_;
        return tmp.str();
    }
};


void ToVec(map<string, int> &m, vector<HelperStruct> &v)
{
    v.clear();
    for(map<string, int>::iterator i = m.begin(); i != m.end(); ++i) v.push_back(HelperStruct(i->second, i->first));
    sort(v.begin(),v.end());
}

void Proc(istream &is, ostream &os)
{
    string line;
    map<string, int> domain, path;
    int url = 0;

    while( getline(is, line) ) url += ProcLine(line, domain, path);
    os<<"total urls "<<url<<", domains "<<domain.size()<<", paths "<<path.size()<<"\n\ntop domains\n";
    vector<HelperStruct> tmp;
    ToVec(domain, tmp);
    for(int i =0; i < min(topN, tmp.size()); ++i ) os<<(string)tmp[i]<<"\n";
    os<<"\ntop paths\n";
    ToVec(path, tmp);
    for(int i =0; i < min(topN, tmp.size()); ++i ) os<<(string)tmp[i]<<"\n";
    

}

int main(int argc, char* argv[])
{
    if ( !(argc == 3 || argc == 5) ) Usage();
    int narg =1;
    if ( argv[narg][0] == '-') {
        if ( argv[narg][1] != 'n') Usage();
        else {
            topN = atoi(argv[2]); 
            narg = 3;
        }
    }
    string ifn, ofn;
    if(narg < argc) ifn = argv[narg++]; else Usage();
    if(narg < argc) ofn = argv[narg++]; else Usage();
    ifstream ifs(ifn);
    ofstream ofs(ofn);
    if ( !ifs.is_open() ) CantOpen(ifn);
    if ( !ofs.is_open() ) CantOpen(ofn);
    Proc(ifs,ofs);
    
	return 0;
}

