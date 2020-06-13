//#include <iostream>
//#include <algorithm>
//#include <vector>
//
//using namespace std;
//
////enum class Gender {
////    FEMALE,
////    MALE
////};
////
////struct Person {
////    int age;  // возраст
////    Gender gender;  // пол
////    bool is_employed;  // имеет ли работу
////    Person ( int a, Gender g, bool i):age(a), gender(g), is_employed(i){}
////};
////
//////template <typename InputIt>
////int ComputeMedianAge(vector<Person>::iterator range_begin, vector<Person>::iterator range_end)
////{
////    for(auto i = range_begin; i != range_end; ++i)
////        cout<< i->age<<' '<<(i->gender == Gender::FEMALE ? "F":"M")<<' '<<i->is_employed<<endl;
////    return 100;
////}
////
////void PrintStats(vector<Person> persons)
////{
////    cout << "Median age = "<<ComputeMedianAge(persons.begin(),persons.end())<<endl;
////    Gender sex = Gender::FEMALE;
////    bool employed = true;
////    auto BySex = [&sex](const Person &p){
////        return p.gender == sex;
////    };
////    auto eop = partition(persons.begin(),persons.end(), BySex);
////    cout << "Median age for females = "<<ComputeMedianAge(persons.begin(),eop)<<endl;
////    sex = Gender::MALE;
////    eop = partition(persons.begin(),persons.end(), BySex);
////    cout << "Median age for males = "<<ComputeMedianAge(persons.begin(),eop)<<endl;
////    auto BySexEmpl = [&sex,&employed](const Person &p){
////        return p.gender == sex && p.is_employed == employed;
////    };
////    sex = Gender::FEMALE;
////    eop = partition(persons.begin(),persons.end(), BySexEmpl);
////    cout << "Median age for employed females = "<<ComputeMedianAge(persons.begin(),eop)<<endl;
////    employed = false;
////    eop = partition(persons.begin(),persons.end(), BySexEmpl);
////    cout << "Median age for unemployed females = "<<ComputeMedianAge(persons.begin(),eop)<<endl;
////    sex = Gender::MALE;
////    employed = true;
////    eop = partition(persons.begin(),persons.end(), BySexEmpl);
////    cout << "Median age for employed males = "<<ComputeMedianAge(persons.begin(),eop)<<endl;
////    employed = false;
////    eop = partition(persons.begin(),persons.end(), BySexEmpl);
////    cout << "Median age for unemployed males = "<<ComputeMedianAge(persons.begin(),eop)<<endl;
////}
////
////
////int main()
////{
////    vector<Person> persons;
////    persons.push_back(Person(31, Gender::MALE, false));
////    persons.push_back(Person(40, Gender::FEMALE, true));
////    persons.push_back(Person(24, Gender::MALE, true));
////    persons.push_back(Person(20, Gender::FEMALE, true));
////    persons.push_back(Person(80, Gender::FEMALE, false));
////    persons.push_back(Person(78, Gender::MALE, false));
////    persons.push_back(Person(10, Gender::FEMALE, false));
////    persons.push_back(Person(55, Gender::MALE, true));
////    PrintStats(persons);	
////    return 0;
////}
////
//// 0 1 2 3 4 5 6 7 8
//// 
//template <typename RandomIt>
//void MergeSort(RandomIt range_begin, RandomIt range_end)
//{
//    if ( range_end-range_begin > 1 ){
//        auto split = range_begin + (range_end-range_begin)/2;
//        MergeSort(range_begin, split);
//        MergeSort(split, range_end);
//        vector<RandomIt::value_type> tmp(range_end-range_begin);
//        RandomIt i = range_begin;
//        RandomIt j = split;
//        RandomIt k = tmp.begin();
//        for( ; i != split && j != range_end; ++k ){
//            if ( *i < *j && i != split) 
//                *k = *i++;
//            else if( j != range_end) 
//                *k = *j++;
//        }
//        while( i != split ) *k++ = *i++;
//        while( j != range_end ) *k++ = *j++;
//
//        for( RandomIt i = tmp.begin(), j = range_begin; i != tmp.end(); ++i,++j) *j = *i;
//    }
//}
//
//
//int main()
//{
//    int a[] = {6, 4, 7, 6, 4, 4, 0, 1};
//    vector<int> v(a,a+8);
//    MergeSort(v.begin(), v.end());
//    for (int x : v) {
//        cout << x << " ";
//    }
//    cout << endl;
//    return 0;
//
//}



#include <iostream>
#include <string>

using namespace std;

class Animal {
public:
    Animal(const string n): Name(n){

    }

    const string Name;
};


class Dog: public Animal {
public:
    Dog(const string g):Animal(g){

    }

    void Bark() {
        cout << Name << " barks: woof!" << endl;
    }
};

int main()
{
    Dog fox("Rency");
    fox.Bark();
    return 0;
}
