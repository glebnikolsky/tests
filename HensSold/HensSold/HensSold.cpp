// HensSold.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <boost/timer/timer.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <iomanip>

const int Total = 10000;


void InitLog()
{
    
    typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend > file_sink;
    boost::shared_ptr< file_sink > sink(new file_sink(
        boost::log::keywords::file_name = _T("Testlog%Y%m%d_%H%M%S.log"), //При каждом запуске новый файл с датой и временем в имени
        boost::log::keywords::rotation_size = 16*1024*1024                   // максимальный размер
    ));

    sink->locked_backend()->set_file_collector(boost::log::sinks::file::make_collector(
        boost::log::keywords::target = "logs",                                      // директория от cwd
        boost::log::keywords::max_size = 64 * 1024 * 1024,                          // максимальный суммарный объём
        boost::log::keywords::min_free_space = 100 * 1024 * 1024                    // минимальное свободное место на диске - не проверял - что и как он сообщит
                                                                                    // проверю позже, скорее всего бросит исключение
     ));

    sink->locked_backend()->scan_for_files();                                       // посмотрит - не стереть ли чего

    boost::log::core::get()->set_filter
        (
        boost::log::trivial::severity >= 0//logging::trivial::info
        /*
        namespace trivial {

        //! Trivial severity levels
        enum severity_level
        {
        trace,
        debug,
        info,
        warning,
        error,
        fatal
        };
*/
            
        );
    boost::log::core::get()->add_sink(sink);
}



bool CheckPrice(int p1, int p2, int hens, int &v)
{
    for(v=0; v <= hens; ++v ){
        int val = p1*v + p2*(hens-v);
        if( val == Total) return true;
        if ( val > Total ) return false; // перебор
    }
    return false;
}

bool CheckPrice(int p1, int p2, int hens, int &v, int &rep_count)
{
    for(v=0; v <= hens; ++v ){
        ++rep_count;
        int val = p1*v + p2*(hens-v);
        if( val == Total) return true;
        if ( val > Total ) return false; // перебор
    }
    return false;
}


//a - price after launch
void calculate_price(float x, float y, float z)
{
    //float b = (35 * x) / (26 * x - x * z + 6 * z - z * y + z);
    float b = (35 * x - 35 * y) / (6 * z + 27 * x - z * x - 26 * y);
    float a = b * 3;
    std::cout << "prices:" << std::setprecision(4) << b << " " << a << std::endl;
}


const int RepeatCount = 1;


void test(int i)
{
    boost::timer::auto_cpu_timer t;
    boost::log::core::get()->set_filter
        (
        boost::log::trivial::severity >= i
        );    
    boost::log::sources::severity_logger< boost::log::trivial::severity_level > lg;
    std::cout <<"Log "<<i<<std::endl;
    int v1,v2,v3, p1, p2;
    bool found = false;
    for(int times=0; times < RepeatCount; ++times)
        for( p1 = Total - 25; p1 >1; --p1){
            BOOST_LOG_SEV(lg, boost::log::trivial::error) << __FILE__<<" "<<__FUNCTION__<<" "<<__LINE__<<" "<<"p1="<<p1;
            for(p2 = 1; p2 < p1; ++p2){
                //                BOOST_LOG_SEV(lg, boost::log::trivial::warning) <<"p2="<< p2;
                for(v1 = 0; v1 <= 10; v1++){
                    //                    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "v1="<< v1;
                    for(v2 = 0; v1*p1 +(10-v1)*p2 == Total && v2 <= 16 ; v2++){
                        //                        BOOST_LOG_SEV(lg, boost::log::trivial::debug) <<"v2="<< v2;
                        for(v3 = 0; v2*p1 + (16-v2)*p2 == Total && v3 <= 26; v3++){
                            //                            BOOST_LOG_SEV(lg,boost::log::trivial::trace) << "v3="<< v3;
                            if ( v3*p1 + (26-v3)*p2 == Total )
                                if ( !found ){
                                    found = true;
                                    std::cout<<"p1="<<p1 <<" p2="<<p2 <<" v1="<<v1 <<" v2="<<v2 <<" v3="<<v3<<std::endl;
                                }
                        }
                    }
                }
            }
        }
}

int _tmain(int argc, _TCHAR* argv[])
{
    //{   // в лоб
    //    boost::timer::auto_cpu_timer t;
    //    int v1,v2,v3, p1, p2;
    //    for( p1 = 3475; p1 >1; --p1)
    //        for(p2 = 1; p2 < p1; ++p2)
    //            for(v1 = 0; v1 <= 10; v1++)
    //                for(v2 = 0; v2 <= 16; v2++)
    //                    for(v3 = 0; v3 <= 26; v3++)
    //                        if ( (v1*p1 + (10-v1)*p2 == 3500) && (v2*p1 + (16-v2)*p2 == 3500) && (v3*p1 + (26-v3)*p2 == 3500) )
    //                            std::cout<<"p1="<<p1 <<" p2="<<p2 <<" v1="<<v1 <<" v2="<<v2 <<" v3="<<v3<<std::endl;
    //}

    InitLog();
    for(int i = 3; i <=6; i++)
        {// проверяем следующего продавца только если у предыдущего всё ОК   
        boost::timer::auto_cpu_timer t;
        boost::log::core::get()->set_filter
            (
            boost::log::trivial::severity >= i
            );    
        boost::log::sources::severity_logger< boost::log::trivial::severity_level > lg;
        std::cout <<"Log "<<i<<std::endl;
        int v1,v2,v3, p1, p2;
        bool found = false;
        for(int times=0; times < RepeatCount; ++times)
        for( p1 = Total - 25; p1 >1; --p1){
            BOOST_LOG_SEV(lg, boost::log::trivial::error) << __FILE__<<" "<<__FUNCTION__<<" "<<__LINE__<<" "<<"p1="<<p1;
            for(p2 = 1; p2 < p1; ++p2){
//                BOOST_LOG_SEV(lg, boost::log::trivial::warning) <<"p2="<< p2;
                for(v1 = 0; v1 <= 10; v1++){
//                    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "v1="<< v1;
                    for(v2 = 0; v1*p1 +(10-v1)*p2 == Total && v2 <= 16 ; v2++){
//                        BOOST_LOG_SEV(lg, boost::log::trivial::debug) <<"v2="<< v2;
                        for(v3 = 0; v2*p1 + (16-v2)*p2 == Total && v3 <= 26; v3++){
//                            BOOST_LOG_SEV(lg,boost::log::trivial::trace) << "v3="<< v3;
                            if ( v3*p1 + (26-v3)*p2 == Total )
                                if ( !found ){
                                    found = true;
                                    std::cout<<"p1="<<p1 <<" p2="<<p2 <<" v1="<<v1 <<" v2="<<v2 <<" v3="<<v3<<std::endl;
                                }
                        }
                    }
                }
            }
        }
    }

    for(int i = 4; i < 7; ++i)
        test(i);
    {// проверяем следующего продавца только если у предыдущего всё ОК   
        boost::timer::auto_cpu_timer t;
        std::cout <<"No Log "<<std::endl;
        int v1,v2,v3, p1, p2;
        bool found = false;
        for(int times=0; times < RepeatCount; ++times)
            for( p1 = Total - 25; p1 >1; --p1){
                for(p2 = 1; p2 < p1; ++p2){
                    for(v1 = 0; v1 <= 10; v1++){
                        for(v2 = 0; v1*p1 +(10-v1)*p2 == Total && v2 <= 16 ; v2++){
                            for(v3 = 0; v2*p1 + (16-v2)*p2 == Total && v3 <= 26; v3++){
                                if ( v3*p1 + (26-v3)*p2 == Total )
                                    if ( !found ){
                                        found = true;
                                        std::cout<<"p1="<<p1 <<" p2="<<p2 <<" v1="<<v1 <<" v2="<<v2 <<" v3="<<v3<<std::endl;
                                    }
                            }
                        }
                    }
                }
            }
    }

    //{// Воспользуемся оптимизацией логических выражений + сокращение перебора в функции
    //    boost::timer::auto_cpu_timer t;
    //    int v1,v2,v3, p1, p2;
    //    bool found = false;
    //    for(int times=0; times < RepeatCount; ++times)
    //    for( p1 = Total-25; p1 > 1; --p1)
    //        for(p2 = 1; p2 < p1; ++p2)
    //            if( CheckPrice(p1, p2, 10, v1) && CheckPrice(p1, p2, 16, v2) && CheckPrice(p1, p2, 26, v3) )
    //                if ( !found ){
    //                    found = true;
    //                    std::cout<<"p1="<<p1 <<" p2="<<p2 <<" v1="<<v1 <<" v2="<<v2 <<" v3="<<v3<<std::endl;
    //                }
    //}
    //
    //{// То же + предварительный отсев
    //    boost::timer::auto_cpu_timer t;
    //    int v1,v2,v3, p1, p2;
    //    bool found = false;
    //    for(int times=0; times < RepeatCount; ++times)
    //    for( p1 = Total-25; p1 > 1; --p1)
    //        for(p2 = 1; p2 < p1; ++p2)
    //            if( p1+p2 < Total && CheckPrice(p1, p2, 10, v1) && CheckPrice(p1, p2, 16, v2) && CheckPrice(p1, p2, 26, v3) )
    //                if ( !found ){
    //                    found = true;
    //                    std::cout<<"p1="<<p1 <<" p2="<<p2 <<" v1="<<v1 <<" v2="<<v2 <<" v3="<<v3<<std::endl;
    //                }
    //}
    //{// Вычислим вторую цену
    //    boost::timer::auto_cpu_timer t;
    //    int rep_count(0);
    //    int v1,v2,v3, p1, p2;
    //    bool found = false;
    //    for(int times=0; times < RepeatCount; ++times)
    //    for( p1 = Total-25; p1 > 1; --p1)
    //        for(v1 = 0; v1 <= 10; ++v1){
    //            ++rep_count;
    //            p2 = Total - p1*v1;
    //            if ( (v1 == 10 && p2) || p2 <= 0 || p2 %(10-v1) ) continue;
    //            p2 /= 10-v1;
    //            if( CheckPrice(p1, p2, 16, v2, rep_count) && CheckPrice(p1, p2, 26, v3, rep_count) )
    //                if ( !found ){
    //                    found = true;
    //                    std::cout<<"x="<<p1<<" y="<<p2<<" v1="<<v1<<" v2="<<v2<<" v3="<<v3<<std::endl;
    //                }
    //        }
    //    std::cout<<"Repeat count = "<<rep_count<<std::endl;
    //} 
    //{// Вычислим вторую цену
    //    boost::timer::auto_cpu_timer t;
    //    int rep_count(0);
    //    int v1,v2,v3, p1, p2;
    //    bool found = false;
    //    for(int times=0; times < RepeatCount; ++times)
    //        for( p1 = Total-25; p1 > 1; --p1)
    //            for(v1 = 0; v1 <= 10; ++v1){
    //                ++rep_count;
    //                p2 = Total - p1*v1;
    //                if ( (v1 == 10 && p2) || p2 <= 0 || p2 %(10-v1) ) continue;
    //                p2 /= 10-v1;
    //                if( CheckPrice(p1, p2, 16, v2, rep_count) && CheckPrice(p1, p2, 26, v3, rep_count) )
    //                    if ( !found ){
    //                        found = true;
    //                        std::cout<<"x="<<p1<<" y="<<p2<<" v1="<<v1<<" v2="<<v2<<" v3="<<v3<<std::endl;
    //                    }
    //            }
    //            std::cout<<"Repeat count = "<<rep_count<<std::endl;
    //} 
    //{
    //    boost::timer::auto_cpu_timer t;
    //    //sold by first before lunch
    //    int x = 0;
    //    //sold by second before lunch
    //    int y = 0;
    //    //sold by first before lunch
    //    int z = 0;
    //    //non optimized
    //    long long nCount1 = 0;
    //    for(x = 10; x > 0; x--)
    //    {
    //        for(y = 9; y > 0; y--)
    //        {
    //            for(z = 8; z > 0; z--)
    //            {
    //                nCount1++;
    //                int left = 5 * x;
    //                int right = (8 * y) - (3 * z);
    //                if( (left == right) && (x > y) && (y > z))
    //                {
    //                    std::cout << std::left << std::endl;
    //                    std::cout << x << " " << y << " " << z << std::endl;
    //                    calculate_price((float)x, (float)y, (float)z);

    //                }
    //            }
    //        }
    //    }
    //    std::cout << "total iteration - " << nCount1 << std::endl;

    //}

	return 0;
}

