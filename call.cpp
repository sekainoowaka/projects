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
    int count = 0;
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

    bool occupied = false;

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
        while (!allMbr_tmp.isEmpty()) { // deallocate the tmp queue
            delete allMbr_tmp.removeFront();
        }

        while (!platinumMbr.isEmpty()) {
            Event *pTmp = platinumMbr.removeFront();
            if (pTmp->count > 0 && pTmp->count < pTmp->duration) {
                pTmp->count++;
                // if we use pCount, there will be double counting problems
                platinumMbr_tmp.insertBack(pTmp);
            } else if (pTmp->count == pTmp->duration) {
                occupied = false;
                delete pTmp;
            } else platinumMbr_tmp.insertBack(pTmp);
        }
        platinumMbr = platinumMbr_tmp;
        while(!platinumMbr_tmp.isEmpty()){
            delete platinumMbr_tmp.removeFront();
        }

        while (!goldMbr.isEmpty()) {
            Event *gTmp = goldMbr.removeFront();
            if (gTmp->count > 0 && gTmp->count < gTmp->duration) {
                gTmp->count++;
                goldMbr_tmp.insertBack(gTmp);
            } else if (gTmp->count == gTmp->duration) {
                occupied = false;
                delete gTmp;
            } else goldMbr_tmp.insertBack(gTmp);
        }
        goldMbr = goldMbr_tmp;
        while(!goldMbr_tmp.isEmpty()){
            delete goldMbr_tmp.removeFront();
        }

        while (!silverMbr.isEmpty()) {
            Event *sTmp = silverMbr.removeFront();
            if (sTmp->count > 0 && sTmp->count < sTmp->duration) {
                sTmp->count++;
                silverMbr_tmp.insertBack(sTmp);
            } else if (sTmp->count == sTmp->duration) {
                occupied = false;
                delete sTmp;
            } else silverMbr_tmp.insertBack(sTmp);
        }
        silverMbr = silverMbr_tmp;
        while(!silverMbr_tmp.isEmpty()){
            delete silverMbr_tmp.removeFront();
        }

        while (!regularMbr.isEmpty()) {
            Event *rTmp = regularMbr.removeFront();
            if (rTmp->count > 0 && rTmp->count < rTmp->duration) {
                rTmp->count++;
                regularMbr_tmp.insertBack(rTmp);
            } else if (rTmp->count == rTmp->duration) {
                occupied = false;
                delete rTmp;
            } else regularMbr_tmp.insertBack(rTmp);
        }
        regularMbr = regularMbr_tmp;
        while(!regularMbr_tmp.isEmpty()){
            delete regularMbr_tmp.removeFront();
        }

        if (!occupied) { // intuitively, "if not occupied"
            if (!platinumMbr.isEmpty()) {
                while (!platinumMbr.isEmpty()) {
                    Event *pTmp = platinumMbr.removeFront();
                    if (!occupied && pTmp->count == 0) {
                        // flag of !occupied ensures operation on only the first object
                        // circumvent the operation once flag is turned
                        pTmp->count++;
                        occupied = true;
                        cout << "Answering call from " << pTmp->name << endl;
                    }
                    platinumMbr_tmp.insertBack(pTmp); // but this process can still be finished
                }
                platinumMbr = platinumMbr_tmp;
                while (!platinumMbr_tmp.isEmpty()) {
                    delete platinumMbr_tmp.removeFront();
                }
            }
            else if (!goldMbr.isEmpty()) {
                while (!goldMbr.isEmpty()) {
                    Event *gTmp = goldMbr.removeFront();
                    if (!occupied && gTmp->count == 0) {
                    	 gTmp->count++;
                        occupied = true;
                        cout << "Answering call from " << gTmp->name << endl;
                    }
                    goldMbr_tmp.insertBack(gTmp);
                }
                goldMbr = goldMbr_tmp;
                while(!goldMbr_tmp.isEmpty()){
                    delete goldMbr_tmp.removeFront();
                }
            }
            else if (!silverMbr.isEmpty()) {
                while (!silverMbr.isEmpty()) {
                    Event *sTmp = silverMbr.removeFront();
                    if (!occupied && sTmp->count == 0) {
                        sTmp->count++;
                        occupied = true;
                        cout << "Answering call from " << sTmp->name << endl;
                    }
                    silverMbr_tmp.insertBack(sTmp);
                }
                silverMbr = silverMbr_tmp;
                while(!silverMbr_tmp.isEmpty()){
                    delete silverMbr_tmp.removeFront();
                }
            }
            else if (!regularMbr.isEmpty()) {
                while (!regularMbr.isEmpty()) {
                    Event *rTmp = regularMbr.removeFront();
                    if (!occupied && rTmp->count == 0) {
                        rTmp->count++;
                        occupied = true;
                        cout << "Answering call from " << rTmp->name << endl;
                    }
                    regularMbr_tmp.insertBack(rTmp);
                }
                regularMbr = regularMbr_tmp;
                while(!regularMbr_tmp.isEmpty()){
                    delete regularMbr_tmp.removeFront();
                }
            }
        }
        tick++;
    }
}
