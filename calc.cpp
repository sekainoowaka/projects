#include "dlist.h"
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

class NotEnough {};
class DivideZero {};
class BadInput {};

void PrintAll(Dlist<int> &l) {
    Dlist<int> _l = l; // perform printing without changing the original list
    while (!_l.isEmpty()) {
        int *lTmp = _l.removeFront();
        cout << lTmp << " ";
        delete lTmp;
    }
    cout << endl;
}

void RemoveAll(Dlist<int> &l) {
    while (!l.isEmpty()) delete l.removeFront(); // simply the passing of *lTmp
}

void Print(Dlist<int> &l) {
    NotEnough op;
    if (l.isEmpty()) throw op;
    int *lTmp = l.removeFront();
    cout << lTmp << endl;
    l.insertFront(lTmp);
}

void Reverse(Dlist<int> &l) {
    NotEnough op;
    if (l.isEmpty()) throw op; // both 0 and 1 are not enough integers
    int *lTmp1 = l.removeFront();
    if (l.isEmpty()) {
        l.insertFront(lTmp1); // return the remove operation and wait for the next command
        throw op; // automatically quit
    }
    int *lTmp2 = l.removeFront();
    l.insertFront(lTmp1); // a newly allocated space will be created for the pointer to object
    l.insertFront(lTmp2);
}

void Duplicate(Dlist<int> &l) {
    NotEnough op;
    if (l.isEmpty()) throw op;
    int *lTmp = l.removeFront();
    int *lCopy = new int (*lTmp);
    l.insertFront(lTmp);
    l.insertFront(lCopy);
}

void Negate(Dlist<int> &l) {
    NotEnough op;
    if (l.isEmpty()) throw op;
    int *lTmp = l.removeFront();
    *lTmp = -*lTmp;
    l.insertFront(lTmp);
//    int *i = new int (*lTmp);
//    *i = 0-(*i);
//    l.insertFront(i);
//    delete lTmp;
}

void Divide(Dlist<int> &l) {
    NotEnough op;
    DivideZero dz;
    int *lTmp1 = l.removeFront();
    if ((*lTmp1) == 0) throw dz;
    if (l.isEmpty()) throw op;
    int *lTmp2 = l.removeFront();
    int *i = new int;
    *i = (*lTmp1) * (*lTmp2);
    l.insertFront(i);
    delete lTmp1;
    delete lTmp2;
}

void Multiply(Dlist<int> &l) {
    NotEnough op;
    int *lTmp1 = l.removeFront();
    if (l.isEmpty()) throw op;
    int *lTmp2 = l.removeFront();
    int *i = new int;
    *i = (*lTmp1) * (*lTmp2);
    l.insertFront(i);
    delete lTmp1;
    delete lTmp2;
}

void Subtract(Dlist<int> &l) {
    NotEnough op;
    if (l.isEmpty()) throw op;
    int *lTmp1 = l.removeFront();
    if (l.isEmpty()) {
        l.insertFront(lTmp1);
        throw op;
    }
    int *lTmp2 = l.removeFront();
    int *i = new int;
    *i = (*lTmp2) - (*lTmp1);
    l.insertFront(i);
    delete lTmp1;
    delete lTmp2;
}

void Plus(Dlist<int> &l) {
    NotEnough op;
    if (l.isEmpty()) throw op;
    int *lTmp1 = l.removeFront();
    if (l.isEmpty()) {
        l.insertFront(lTmp1);
        throw op;
    }
    int *lTmp2 = l.removeFront();
    int *i = new int;
    *i = (*lTmp2) + (*lTmp1);
    l.insertFront(i);
    delete lTmp1;
    delete lTmp2;
}

int main(){
    string s;
    Dlist<int> stack;
    while (true) {
        cin >> s;
        if (s == "q") {
            RemoveAll(stack);
            break;
        }
        else if (s == "+") {
            try { Plus(stack); }
            catch (NotEnough) { cout << "Not enough operands\n"; }
        }
        else if (s == "-") {
            try { Subtract(stack); }
            catch (NotEnough) { cout << "Not enough operands\n"; }
        }
        else if (s == "*") {
            try { Multiply(stack); }
            catch (NotEnough) { cout << "Not enough operands\n"; }
        }
        else if (s == "/") {
            try { Divide(stack); }
            catch (DivideZero) { cout << "Divide by zero\n"; }
            catch (NotEnough) { cout << "Not enough operands\n"; }
        }
        else if (s == "n") {
            try { Negate(stack); }
            catch (NotEnough) { cout << "Not enough operands\n"; }
        }
        else if (s == "d") {
            try { Duplicate(stack); }
            catch (NotEnough) { cout << "Not enough operands\n"; }
        }
        else if (s == "r") {
            try { Reverse(stack); }
            catch (NotEnough) { cout << "Not enough operands\n"; }
        }
        else if (s == "p") {
            try { Print(stack); }
            catch (NotEnough) { cout << "Not enough operands\n"; }
        }
        else if (s == "c") RemoveAll(stack);
        else if (s == "a") PrintAll(stack);
        else {
            try {
                BadInput bi;
                int *iTmp = new int; // allocate memory
                int minus = s.find('-');
                if (minus != 0) throw bi;
                else {
                    for (int i = minus+1; i < s.size(); i++) {
                        if (s[i] < 0 || s[i] > 9) {
                            delete iTmp; // either delete
                            throw bi;
                        }
                        else {
                            *iTmp = stoi(s);
                            stack.insertFront(iTmp); // or assign to container
                        }
                    }
                }
            }
            catch (BadInput) { cout << "Bad input\n"; }
        }
    }
}
