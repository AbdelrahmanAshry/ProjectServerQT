#include<iostream>
using namespace std;
//base class
class Square{
    private :
    int x;
    public:
    Square(int _x):x(_x){}
    virtual ~Square(){};
    void setX(int _x){x=_x;}
    int getX()const {return x;}
    virtual void print() const{cout<<"Square \n";}    
};
//rectangle 
class Rectangle :public  Square
{
private:
    int y;
public:
    Rectangle(int _x,int _y):Square(_x){y=_y;}
    void setY(int _y){y=_y;}
    int getY()const{return y;}
    void print() const{cout<<"Rectangle \n";}    
    virtual ~Rectangle(){};
};
class Foo :public  Square
{

public:
    Square::Square;
    void print() const{cout<<"Foo \n";}    
    virtual ~Foo(){}
};
void foo(const Square & s){//without refrence it will be static
 //   cout<<s.getX()<<"\n";
 s.print();
}
int main ()
{
    Square s(7);
    Rectangle r(2,4);
//bound at compile time to the parent functions
//solve by adding virtual only in base class so it is now overriden
    foo(r);
    Square * Sptr=&r;
    Sptr->print();
    Square * squares[]={new Square(3),new Rectangle(3,5),new Foo(1)};
    for (Square* s: squares)
        s->print();
    for(Square * s : squares)
        delete s;
}