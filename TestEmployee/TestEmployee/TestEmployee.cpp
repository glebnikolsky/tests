// TestEmployee.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "math.h"


//double operator()(double x);{ a+b*sin(x*c)}

class Func
{
public:
    double a, b, c;
    Func()
    {
    }
    Func(double a, double b, double c) : a(a), b(b), c(c) {}
    //double operator()(double x) { return a+b*sin(x*c); }
    double operator()(double x) { return sin(x); }
    //double operator()(double a, double b, double c, double x) { return a+b*sin(x*c); }
};
//double operator()(double x);{ a+b*sin(x*c)}

template<typename T>
//bool ZeroFind(T a, T b, T c, double &x)
//bool ZeroFind(T a, T b, T c, double &x, double x0=-1, double x1=1)
//bool ZeroFind(T a, T b, T c, double &x, double x0=-1, double x1=1, Func func())
//bool ZeroFind(T a, T b, T c, double &x, double x0=-1, double x1=1)
bool ZeroFind(double a, double b, double c, double &x, double epsilon=0.001, double x0=-1, double x1=1)
{
    T func(a, b, c);
    double y0=func(x0);
    double y1=func(x1);
    double y=func(x);
    while(x1-x0>epsilon)
    {
        //if(y0>y1)
        if(abs(y0)>abs(y1))
        {
            x1=x;
            y1=y;
            //x=(x0+x1)/2;
            //y=T(a, b, c, x);
        }
        else
        {
            x0=x;
            y0=y;
            //x=(x0+x1)/2;
            //y=T(a, b, c, x);
        }
        x=(x0+x1)/2;
        y=func(x);
    }
    return y==0;
}

class Point
{
public:
//protected:
    double x, y;
//public:
    Point()
    {
        //x=0;
        //y=0;
    }
    Point(double x, double y)
    {
        this->x=x;
        this->y=y;
    }
    //double GetX(){return x;}
    //double GetY(){return y;}
    double Distance(Point p)
    {
        return sqrt((p.x-x)*(p.x-x)+(p.y-y)*(p.y-y));
    }
    double Distance(double x, double y)
    {
        return sqrt((x-this->x)*(x-this->x)+(y-this->y)*(y-this->y));
    }
    virtual ~Point() {}
};

class Line
{
    //Point* p0, *p1;
    Point p0, p1;
public:
    Line()
    {
        /*p0=new Point();
        p1=new Point();*/
    }
    Line(double x0, double y0, double x1, double y1) : p0(x0, y0), p1(x1, y1) {}
    /*{
        p0=new Point(x0, y0);
        p1=new Point(x1, y1);
    }*/
    double Length()
    {
        //return sqrt((p1->x-p0->x)*(p1->x-p0->x)+(p1->y-p0->y)*(p1->y-p0->y));
        return sqrt((p1.x-p0.x)*(p1.x-p0.x)+(p1.y-p0.y)*(p1.y-p0.y));
    }
    /*~Line()
    {
        delete p0;
        delete p1;
    }*/
};

class Circle : public Point
{
public:
    double r;
    Circle() : Point() {}
    /*Circle(double x, double y, double r=1.0) : Point(x, y)
    {
        this->r=r;
    }*/
    Circle(double x, double y, double r=1.0) : Point(x, y), r(r) {}
    bool Connected(const Circle& c)
    {
        //return Line(x, y, c.x, c.y).Length();
        return Distance(c.x, c.y)<r+c.r;
    }
};

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    Point* p=new Circle(1,1,1);
    delete p;
    char c;
    double x0, y0, x1, y1;
    cout<<"(lenLine=l, CircleConnected=c, ZeroFind=f, exit=q) :"; cin>>c;
    switch(c)
    {
    case 'l':
        {
            do 
            {
                cout<<"x0="; cin>>x0;
                cout<<"y0="; cin>>y0;
                cout<<"x1="; cin>>x1;
                cout<<"y1="; cin>>y1;
                Line line(x0, y0, x1, y1);
                cout<<endl<<"length="<<line.Length()<<endl;
                cout<<"exit[y/n]?"; cin>>c;
            } while(c!='y');
            break;
        }
    case 'c':
        {
            break;
        }
    case 'f':
        {
            double a, b, C, epsilon;
            cout<<"(a+b*sin(x*c))"<<endl;
            cout<<"a="; cin>>a;
            cout<<"b="; cin>>b;
            cout<<"c="; cin>>C;
            cout<<"x0="; cin>>x0;
            cout<<"x1="; cin>>x1;
            cout<<"epsilon="; cin>>epsilon;
            double X;
            cout<<"x="; cin>>X;
            //cout<<ZeroFind<double>()<<" x="<<X<<endl;
            cout<<ZeroFind<Func>(a, b, C, X, epsilon, x0, x1)<<" x="<<X<<endl;
            getchar();
            getchar();
            break;
        }
    case 'q':
        {
            break;
        }
    }
    return 0;
}

