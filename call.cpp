#include "dlist.h"
#include <iostream>

using namespace std;

enum Status {Regular,Silver,Gold,Platinum};
const string StatusName[] = {"regular","silver","gold","platinum"};

struct Event{
    int timestamp;
    string name;
    Status stat;
    int duration;
    bool is_calling = false;
    bool is_answered = false;
};

int main() {
    int N; // first line: number of events
    cin >> N; // read from cin, not fstream

    ////implement double-linked list as queue
    Dlist<Event> platinumMbr;
    Dlist<Event> platinumMbr_tmp;
    Dlist<Event> goldMbr;
    Dlist<Event> goldMbr_tmp;
    Dlist<Event> silverMbr;
    Dlist<Event> silverMbr_tmp;
    Dlist<Event> regularMbr;
    Dlist<Event> regularMbr_tmp;
    Dlist<Event> allMbr;
    Dlist<Event> allMbr_tmp;

    int pCount = 0;
    int gCount = 0;
    int sCount = 0;
    int rCount = 0;
    bool is_answering = false;

    ////read file input, and insert into call members
    for (int i = 0; i < N; i++) {
        Event *newEvent = new Event;
        string status;
        cin >> newEvent->timestamp >> newEvent->name >> status >> newEvent->duration; // no endl
        for (int j = 0; j < 4; j++) {
            if (StatusName[j] == status) newEvent->stat = Status(j);
            // if strings are equal, then assign the corresponding jth enum to stat
        }
        allMbr.insertBack(newEvent);
        // in insertBack, new memory space is allocated
        // but after insertion, new Event is not dangling
        // since the new node created is /pointing to/ the newEvent!!!
        // delete newEvent; is wrong
    }

    ////discrete event simulation
    int tick = 0;
    while (!allMbr.isEmpty() || !platinumMbr.isEmpty() || !goldMbr.isEmpty() ||
           !silverMbr.isEmpty() || !regularMbr.isEmpty()) {
        cout << "Starting tick #" << tick << endl;

        while (!allMbr.isEmpty()) { // if we don't use allMbr_tmp
            // to separate the != tick Events from the original queue,
            // this will become an infinite loop.
            Event *allTmp = allMbr.removeFront();
            if (allTmp->timestamp != tick) allMbr_tmp.insertBack(allTmp);
                // this enables emptying of the allMbr queue
            else {
                cout << "Call from " << allTmp->name << " a "
                     << StatusName[allTmp->stat] << " member" << endl;
                // store the called members into corresponding wait lists
                switch (allTmp->stat) {
                    case Regular:
                        regularMbr.insertBack(allTmp);
                        break;
                    case Silver:
                        silverMbr.insertBack(allTmp);
                        break;
                    case Gold:
                        goldMbr.insertBack(allTmp);
                        break;
                    default:
                        platinumMbr.insertBack(allTmp);
                        break;
                }
            }
        }
        allMbr = allMbr_tmp; // this is the rest of the queue
        while (!allMbr_tmp.isEmpty()) {
            delete allMbr_tmp.removeFront();
        }

        while (!platinumMbr.isEmpty()) {
            Event *pTmp = platinumMbr.removeFront();
            if (pCount > 0 && pCount < pTmp->duration) {
                pCount++;
                platinumMbr_tmp.insertBack(pTmp);
            } else if (pCount == pTmp->duration) {
                pCount = 0;
                is_answering = false;
                delete pTmp; // ?
            } else platinumMbr_tmp.insertBack(pTmp);
        }
        platinumMbr = platinumMbr_tmp;
        while(!platinumMbr_tmp.isEmpty()){
            delete platinumMbr_tmp.removeFront();
        }

//        if (!platinumMbr.isEmpty()) {
//            Event *pTmp = platinumMbr.removeFront(); // we only care about the first element...
//            if (pCount > 0 && pCount < pTmp->duration) {
//                pCount++;
//                platinumMbr.insertFront(pTmp);
//            } else if (pCount == pTmp->duration) {
//                pCount = 0;
//                is_answering = false;
//                delete pTmp;
//            }
//        } // makes this not a queue?

        while (!goldMbr.isEmpty()) {
            Event *gTmp = goldMbr.removeFront();
            if (gCount > 0 && gCount < gTmp->duration) {
                gCount++;
                goldMbr_tmp.insertBack(gTmp);
            } else if (gCount == gTmp->duration) {
                gCount = 0;
                is_answering = false;
                delete gTmp;
            } else goldMbr_tmp.insertBack(gTmp);
        }
        goldMbr = goldMbr_tmp;
        while(!goldMbr_tmp.isEmpty()){
            delete goldMbr_tmp.removeFront();
        }

        while (!silverMbr.isEmpty()) {
            Event *sTmp = silverMbr.removeFront();
            if (sCount > 0 && sCount < sTmp->duration) {
                sCount++;
                silverMbr_tmp.insertBack(sTmp);
            } else if (sCount == sTmp->duration) {
                sCount = 0;
                is_answering = false;
                delete sTmp;
            } else silverMbr_tmp.insertBack(sTmp);
        }
        silverMbr = silverMbr_tmp;
        while(!silverMbr_tmp.isEmpty()){
            delete silverMbr_tmp.removeFront();
        }

        while (!regularMbr.isEmpty()) {
            Event *rTmp = regularMbr.removeFront();
            if (rCount > 0 && rCount < rTmp->duration) {
                rCount++;
                regularMbr_tmp.insertBack(rTmp);
            } else if (rCount == rTmp->duration) {
                rCount = 0;
                is_answering = false;
                delete rTmp;
            } else regularMbr_tmp.insertBack(rTmp);
        }
        regularMbr = regularMbr_tmp;
        while(!regularMbr_tmp.isEmpty()){
            delete regularMbr_tmp.removeFront();
        }

        // in !is_answering: if count == 0, count++
        // outside: if count > 0, continue to count++
        // until count = duration, reset count as 0, is_answering = false, destroy in waitList & member list

        if (!is_answering) { // once the call is answered, we can never go into this argument
            if (pCount == 0 && !platinumMbr.isEmpty()) { // think carefully: what if there are multiple platinum members?
                pCount++;
                is_answering = true;
                Event *pTmp = platinumMbr.removeFront();
                cout << "Answering call from " << pTmp->name << endl;
                platinumMbr.insertFront(pTmp);
            }
            else if (gCount == 0 && !goldMbr.isEmpty()) {
                gCount++;
                is_answering = true;
                Event *gTmp = goldMbr.removeFront();
                cout << "Answering call from " << gTmp->name << endl;
                goldMbr.insertFront(gTmp);
            }
            else if (sCount == 0 && !silverMbr.isEmpty()) {
                sCount++;
                is_answering = true;
                Event *sTmp = silverMbr.removeFront();
                cout << "Answering call from " << sTmp->name << endl;
                goldMbr.insertFront(sTmp);
            }
            else if (rCount == 0 && !regularMbr.isEmpty()) {
                rCount++;
                is_answering = true;
                Event *rTmp = regularMbr.removeFront();
                cout << "Answering call from " << rTmp->name << endl;
                regularMbr.insertFront(rTmp);
            }
        }
        tick++;
    }
}
