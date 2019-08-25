#include<iostream>
#include<cstdlib>
#define default_value 50
using namespace std;
 
template< class T > class Stack
{
    public:
    Stack(int = default_value);//default constructor
    ~Stack()//destructor
    {delete [] values;}
    bool push( T );
    T pop();
    bool isEmpty();
    bool isFull();
    private:
    int size;
    T *values;
    int index;
 
};
 
template< class T > Stack<T>::Stack(int x):
    size(x),//ctor
    values(new T[size]),
    index(-1)
{ /*empty*/  }
 
template< class T > bool Stack<T>::isFull()
{
    if((index + 1) == size )
    return 1;
    else
    return 0;
}
 
template< class T > bool Stack<T>::push(T x)
{
    bool b = 0;
    if(!Stack<T>::isFull())
    {
    index += 1;
    values[index] = x;
    b = 1;
    }
    return b;
}
 
template< class T > bool Stack<T>::isEmpty()
{
    if( index  == -1 )//is empty
    return 1;
    else
    return 0; //is not empty
}
 
template< class T > T Stack<T>::pop()
{
    T val = -1;
    if(!Stack<T>::isEmpty())
    {
    val = values[index];
    index -=  1;
    }
    else
    {
    cerr << "Stack is Empty : ";
    }
    return val;
 
}