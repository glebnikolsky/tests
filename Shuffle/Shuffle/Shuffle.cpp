// Shuffle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/chrono.hpp>
#include <boost/format.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/timer/timer.hpp>


const size_t low_limit = 1;
const size_t upper_limit = 10;



struct EAN13{
    EAN13(){};
    EAN13(long h,long n):head_(h), no_(n)
    { 
        CalcCC(); 
    }
    EAN13(int blanktype, int exam_type, long n):no_(n)
    {
            head_ = 27000 + blanktype*100 + exam_type; 
            CalcCC();
    }
    operator std::pair<long,long>()
    {
        return std::make_pair(head_, no_);
    }
    operator std::string(){
        std::string fmt(13,'0');
        long tmp = head_;
        for(int i = 4; tmp && i >=0; --i, tmp /= 10 ) fmt[i] += tmp%10;
        tmp = no_;
        for(int i = 12; tmp; --i, tmp /= 10 ) fmt[i] += tmp%10;
        return fmt;
    }

protected:
    long head_, no_;

    void CalcCC()
    {
        static int veight[13] = {1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1};
        long tmp = head_;
        int code(0);
        for(int i =4; tmp && i >= 0; --i, tmp /= 10 ) code += veight[i]*(tmp%10);
        tmp = no_;
        for(int i =11; tmp && i > 4; --i, tmp /= 10 ) code += veight[i]*(tmp%10);
        no_ *= 10;
        no_ += code%10? 10 - code%10: 0;
    }
};


std::pair<long,long>ToEAN13N(std::pair<long,long> &no)
{

    int veight[13] = {1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1};
    long tmp = no.first;
    int code(0);
    for(int i =4; tmp && i >= 0; --i, tmp /= 10 ) code += veight[i]*(tmp%10);
    tmp = no.second;
    for(int i =11; tmp && i > 4; --i, tmp /= 10 ) code += veight[i]*(tmp%10);
    no.second *= 10;
    no.second += code%10? 10 - code%10: 0;
    return no;
}

std::string ToEAN13(std::string& scode)
{
    int code(0);
    int veight[13] = {1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1};
    for(int i =0; i < 12; ++i ) code += veight[i]*(scode[i]-'0');

    scode += '0' + ((code%10) ? 10 - (code%10): 0);
    return scode;
}


std::pair<long,long>GenBarcodeN(int blanktype, int examtype, int no)
{
    return std::make_pair(27000 + blanktype*100+examtype,no);
}

std::string GenBarcode(int blanktype, int exam_type, int no)
{
    std::ostringstream str;
    str<<boost::format("27%1%%2$02d%3$07d") % blanktype % exam_type % no;
    std::string s = str.str();
    return s;
}

void GenSeq(std::vector<int> &seq, int blanktype, int exam_type, int low=1000000, int high=7999999)
{
    std::vector<int> tmp(high-low+1);
    std::swap(tmp,seq);
    for(int i = low; i <= high; ++i) seq[i-low]=i;
    int seed = 27000 + blanktype*100 + exam_type;
    boost::minstd_rand gen(seed);
    boost::random::uniform_int_distribution<long long> dist(0, seq.size()-1);
    for(auto &i:seq) std::swap(i,seq[dist(gen)]);
}


int _tmain(int argc, _TCHAR* argv[])
{

        boost::timer::auto_cpu_timer t;
        boost::minstd_rand gen(27000);
        std::vector<long> v(1000000);
        boost::random::uniform_int_distribution<long long> dist(0, 999999);
        for(int i =0; i < 1000000; ++i) v[i] = i;
        for(long i =0; i < 1000000; i+=3) std::swap(v[i],v[dist(gen)]);
        long p(0);
        for(int i =0; i < 1000000; ++i) if ( v[i] == i) ++p;
        std::cout<<1<<'\t'<<p/10000.<<std::endl;

    //std::vector<int> v1(upper_limit-low_limit+1),v2(upper_limit-low_limit+1);
    //for(int i = low_limit; i <= upper_limit; ++i) v1[i-low_limit]=v2[i-low_limit]=i;
    //{
    //    boost::minstd_rand gen(4201);
    //    boost::random::uniform_int_distribution<long long> dist(0, upper_limit-low_limit);
    //    for(auto &i:v1) {
    //        std::swap(i,v1[dist(gen)]);
    //        for(auto &i:v1) std::cout<<' '<<i;
    //        std::cout<<std::endl;
    //    }
    //    std::cout<<std::endl;
    //}
    //{
    //    boost::minstd_rand gen(4201);
    //    boost::random::uniform_int_distribution<long long> dist(0, upper_limit-low_limit);
    //    for(auto &i:v2) std::swap(i,v2[dist(gen)]);
    //    std::cout<<std::endl;
    //}
    //std::cout<<(v1==v2)<<std::endl;
    //for(auto &i:v2) std::cout<<'\t'<<i;



    //std::string prolog ="<PackNumber number=\"01%06d\">\n", epi = "</PackNumber>\n";
    //std::string rep1 ="\t<StuffCode StuffCode=\"%1%\"/>\n";
    //{
    //    boost::timer::auto_cpu_timer t;
    //    for(int i =0; i < 1000000; ++i){
    //        std::string q = ToEAN13(GenBarcode(3,1,1234567));
    //    }
    //}
    //{
    //    boost::timer::auto_cpu_timer t;
    //    for(int i =0; i < 1000000; ++i){
    //        std::pair<long,long> tmp = ToEAN13N(GenBarcodeN(3,1,1234567));
    //        std::ostringstream str;
    //        str<<boost::format("%1$05d%2$07d") % tmp.first % tmp.second;
    //        std::string q = str.str();
    //    }
    //}
    /*
    EAN13 a(27301, 50079);
    std::cout<<(std::string)a<<' '<<((std::pair<long,long>)a).first<<'-'<<((std::pair<long,long>)a).second<<std::endl;
    a = EAN13(27101, 50125);
    std::cout<<(std::string)a<<' '<<((std::pair<long,long>)a).first<<'-'<<((std::pair<long,long>)a).second<<std::endl;
    a = EAN13(2, 1, 50102);
    std::cout<<(std::string)a<<' '<<((std::pair<long,long>)a).first<<'-'<<((std::pair<long,long>)a).second<<std::endl;
    a = EAN13(6, 10, 501250);
    std::cout<<(std::string)a<<' '<<((std::pair<long,long>)a).first<<'-'<<((std::pair<long,long>)a).second<<std::endl;

    std::vector<int> r,c,ab,c2,r1;
    GenSeq(r, 3, 1 );
    GenSeq(ab, 3, 1 );
    GenSeq(c, 3, 1 );
    GenSeq(c2, 3, 1,1,399999);
    GenSeq(r1, 3, 1);


    //int pack = 1, stuff=1;
    //for(int i = 0; i < 6; ++i){
    //    std::cout<<boost::format(prolog)%pack;
    //    ++pack;
    //    for(int j =0; j<5; ++j){
    //        std::ostringstream str;
    //        str<<boost::format("001%07d")%stuff;
    //        std::cout<<boost::format(rep1)%str.str();
    //        stuff++;
    //    }
    //    std::cout<<epi;
    //}
    //for(int i = 0; i < 6; ++i){
    //    std::cout<<boost::format(prolog)%pack;
    //    ++pack;
    //    for(int j =0; j<15; ++j){
    //        std::ostringstream str;
    //        str<<boost::format("001%07d")%stuff;
    //        std::cout<<boost::format(rep1)%str.str();
    //        stuff++;
    //    }
    //    std::cout<<epi;
    //}
    //std::cout<<"Region\n";
    //for(int i = 0; i < 6; ++i){
    //    std::cout<<boost::format(prolog)%pack;
    //    ++pack;
    //    for(int j =0; j<5; ++j){
    //        std::ostringstream str;
    //        str<<boost::format("001%07d")%stuff;
    //        std::cout<<boost::format(rep1)%str.str();
    //        stuff++;
    //    }
    //    std::cout<<epi;
    //}
    //for(int i = 0; i < 6; ++i){
    //    std::cout<<boost::format(prolog)%pack;
    //    ++pack;
    //    for(int j =0; j<15; ++j){
    //        std::ostringstream str;
    //        str<<boost::format("001%07d")%stuff;
    //        std::cout<<boost::format(rep1)%str.str();
    //        stuff++;
    //    }
    //    std::cout<<epi;
    //}
    int variant = 101;
//    for(int pack = 1; pack<=240; pack ++){
//        std::cout<<boost::format(
//"<KimDatabase Region=\"%3%\" SubjectCode=\"1\" ExamDate=\"2017.11.01\" VariantCode=\"%1%\" ZoneCode=\"0\" \
//StuffCode=\"001%2$07d\" Barcode_R=\"%4%\" Barcode_AB=\"%5%\" Barcode_C=\"%6%\" Barcode_C2=\"%7%\"/>\n") 
//        % variant % pack % (pack <=120?2:12) % ToEAN13(GenBarcode(3,1,r[pack-1])) % ToEAN13(GenBarcode(1,1,ab[pack-1])) 
//        % ToEAN13(GenBarcode(2,1,c[pack-1])) % ToEAN13(GenBarcode(4,99,c2[pack-1]));
//    variant = variant < 105 ? ++variant:101;
    for(int pack = 1; pack <= 240; ++pack){
        std::cout<<boost::format("\
<Set Number=\"001%1$07d\" Variant=\"%2$03d\">\
<Blank NumInSet=\"1\" Number=\"%3%\"/><Blank NumInSet=\"2\" Number=\"%4%\"/><Blank NumInSet=\"3\" Number=\"%5%\"/><Blank NumInSet=\"4\" Number=\"%6%\"/></Set>\n") 
    % pack % variant % ToEAN13(GenBarcode(3,1,r[pack-1])) % ToEAN13(GenBarcode(1,1,ab[pack-1])) % ToEAN13(GenBarcode(2,1,c[pack-1])) % ToEAN13(GenBarcode(4,99,c2[pack-1]));
        variant = variant < 105 ? ++variant:101;
    }
    variant = 100;
    for(int pack = 241; pack <= 480; ++pack){
        std::pair<long,long> p  = ToEAN13N(GenBarcodeN(0,0,pack));
        std::cout<<boost::format("\
<Set Number=\"%1$07d\" Variant=\"%2$03d\"><Blank NumInSet=\"1\" Number=\"%3%\"/></Set>\n") 
                                 %  p.second % variant % ToEAN13(GenBarcode(9,29,r[pack-1]));
        variant = variant < 121 ? ++variant:100;
    }*/
    return 0;
}

