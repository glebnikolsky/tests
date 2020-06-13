// Numbers.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
//#include <algorithm>
//#include <iostream>
//#include <map>
//#include <set>
//#include <string>
//#include <vector>
//#include <boost/timer/timer.hpp>
//
//inline bool gen_comb_norep_lex_next(std::vector<int> &comb, const int n, const int k)
//{
//    if(comb[k - 1] < n - 1){
//        comb[k - 1]++;
//        return true;
//    }
//    int j; 
//    for(j = k - 2; j >= 0; j--)
//        if(comb[j] < n - k + j) break;
//    if(j < 0) return false;
//    comb[j]++;
//    while(j < k - 1)
//        comb[j + 1] = comb[j++] + 1;
//    return true;
//}
//
//
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//    for(int base = 8; base <= 32; base++){
//        std::cout<<base<<'\t';
//        int n=0;
//        {
//            boost::timer::auto_cpu_timer t("\t%ws\t");
//            for(int r=0; r < base; ++r)
//                for(int q=0; q < base; ++q){
//                    if ( q == r ) continue;
//                        for(int v=0; v < base; ++v){
//                            if ( v == r || v == q ) continue;
//                            for(int w=0; w < base; ++w){
//                                if ( w == q || w == r || w == v ) continue;
//                                for( int z =0; z < base; ++z ){
//                                    if ( z == w || z == v || z == q || z == r ) continue;
//                                    int a = (base*r + q)*base + r,
//                                        b = (base*z + v)*base + v,
//                                        c = ((base*r + q)*base + v)*base + w;
//                                    if ( a+b ==c ) ++n;
//                                }
//                            }
//                        }
//                }
//            std::cout<<n<<'\t';
//        }
//        
//        n = 0;
//        {
//            boost::timer::auto_cpu_timer t("\t%ws\n");
//            int probes=0;
//            int tmp[] = {0,1,2,3,4};
//            std::vector<int> comb(tmp, tmp+5);
//            int &r = comb[0], &q = comb[1], &v = comb[2], &w = comb[3], &z = comb[4];
//            do{
//                do{
//                    int a = (base*r + q)*base + r,
//                        b = (base*z + v)*base + v,
//                        c = ((base*r + q)*base + v)*base + w;
//                    ++probes;
//                    if ( a + b == c) ++n;
//                }while( std::next_permutation(comb.begin(),comb.end()));
//            }while( gen_comb_norep_lex_next(comb, base, 5) );
//            std::cout<<n<<'\t'<<probes;
//        }
//    }    
//
//
//    return 0;
//}
//


// Numbers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <boost/timer/timer.hpp>


int Primes(int maxPrime, std::vector<int> &primes) 
{
    primes.push_back(2);
    primes.push_back(3);
    for( int i = 6; i <= maxPrime; i +=6)
        for(int j = -1; j <= 1; j+=2  ){
            int is_prime = i+j;
            bool found = true;
            for(int n=0; found && n < primes.size(); n++)
                found = is_prime % primes[n];
            if ( found) primes.push_back(is_prime);
        }
        return primes.size();
}


int _tmain(int argc, _TCHAR* argv[])
{

    int maxNo;
    int maxPow;
    std::cout<<"Max number= ";
    std::cin>>maxNo;
    std::cout<<"Max power= ";
    std::cin>>maxPow;
    std::vector< int> primes;
    
    {
        boost::timer::auto_cpu_timer t("\t%ws\n");
        int noOfPrimes = Primes(maxNo, primes);
        std::set<std::map<int,int> > result;
        int n =0, m = 0;
        for(int pow = 3; pow < maxPow; ++pow ){
            for(int i= 3; i < maxNo; i++ ){
                std::map<int,int> tmp;
                int probe = i;
                ++n;
                for(int j = 0; probe != 1 && j < noOfPrimes; ){
                    if ( !(probe % primes[j]) ){
                        tmp[primes[j]] += pow;
                        probe /= primes[j];
                    }else ++j;
                }
                auto fnd = result.find(tmp);
                if ( fnd == result.end() ){
//                    std::cout<<'<'<<i<<','<<pow<<std::endl;
                    result.insert(tmp);
                }else {
                    ++m;
                    std::cout<<'<'<<i<<','<<pow<<' ';
                    for(auto kk = tmp.begin(); kk != tmp.end(); ++kk)
                        std::cout<<kk->first<<' '<<kk->second<<'|';
                    std::cout<<std::endl;
                }
            }
        }
        std::cout<<n<<' '<<result.size()<<' '<<m<<std::endl;
    }

    return 0;
}

