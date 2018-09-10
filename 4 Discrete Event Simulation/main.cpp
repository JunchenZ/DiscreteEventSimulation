//
//  main.cpp
//  4 Discrete Event Simulation
//
//  Created by Junchen Zhang on 2/16/18.
//  Copyright © 2018 Junchen Zhang. All rights reserved.
//

#include <iostream>
#include <queue>
#include <vector>
#include <time.h>
#include <math.h>
#include <map>
using namespace std;

struct customer{
    long arrival;
    int time;
    int count;

    customer(long val, int t){
        arrival = val;
        time = t;
        count = 0;
    }
    bool operator>(const customer& rhs) const{
        return arrival > rhs.arrival;
    }
};

void bank(long total){
    vector<customer> teller;
    long timer = 0;
    double percent = 0;
    srand((unsigned int)time(NULL));
    priority_queue<customer, vector<customer>, greater<customer> > queue;
    vector<long> timeVec;
    while(timer <= total){
        timer ++;
        if(teller.size() != 10){
            percent++;
        }
        if(timer % 32 == 0){
            customer person = customer(timer, rand() % 571 + 30);
            queue.push(person);
        }
        for(int i = 0; i < teller.size(); i++){
            teller.at(i).count++;
            if(teller.at(i).count == teller.at(i).time){
                timeVec.push_back(timer - teller.at(i).arrival);
                teller.erase(teller.begin() + i);
            }
        }
        while(teller.size() < 10 && !queue.empty()){
            teller.push_back(queue.top());
            queue.pop();
        }
    }
    long totalTime = 0, variance = 0;
    for(int i = 0; i < timeVec.size(); i++){
        totalTime += timeVec.at(i);
    }
    int avgTime = int(totalTime / timeVec.size());
    for(int i = 0; i < timeVec.size(); i++){
        variance += pow((timeVec.at(i) - avgTime), 2);
    }
    cout << "average time: " << avgTime << endl;
    cout << "variance: " << variance / timeVec.size() << endl;
    cout << "total served number: " << timeVec.size() << endl;
    cout << "time percent of idle: " << 100 * percent / timer << "%\n";
}

void supermarket(long total){
    vector<vector<customer>> cashier;
    long timer = 0;
    double percent = 0;
    srand((unsigned int)time(NULL));
    vector<priority_queue<customer, vector<customer>, greater<customer>>> queues;
    int qtime[10] = {};
    priority_queue<customer, vector<customer>, greater<customer> > q;
    for(int i = 0; i < 10; i++){
        qtime[i] = 0;
        cashier.push_back(vector<customer>());
        queues.push_back(q);
    }
    vector<long> timeVec;
    while(timer <= total){
        timer ++;
        //check idle
        for(int i = 0; i < 10; i++){
            if(cashier.at(i).size() == 0){
                percent++;
                break;
            }
        }
        //enqueue
        if(timer % 32 == 0){
            customer person = customer(timer, rand() % 571 + 30);
            int min = 0;
            for(int i = 0; i < 10; i++){
                if(cashier.at(i).size() == 0){
                    min = i;
                    break;
                }
                if(qtime[i] + cashier.at(i).at(0).time - cashier.at(i).at(0).count < qtime[min] + cashier.at(min).at(0).time - cashier.at(min).at(0).count){
                    min = i;
                }
            }
            queues.at(min).push(person);
            qtime[min] += person.time;
        }
        //finish service
        for(int i = 0; i < 10; i++){
            if(cashier.at(i).size() != 0){
                cashier.at(i).at(0).count++;
                if(cashier.at(i).at(0).count == cashier.at(i).at(0).time){
                    timeVec.push_back(timer - cashier.at(i).at(0).arrival);
                    cashier.at(i).erase(cashier.at(i).begin());
                }
            }
        }
        //to service
        for(int i = 0; i < 10; i++){
            if(cashier.at(i).size() == 0 && !queues.at(i).empty()){
                cashier.at(i).push_back(queues.at(i).top());
                qtime[i] -= queues.at(i).top().time;
                queues.at(i).pop();
            }
        }
    }
    long totalTime = 0, variance = 0;
    for(int i = 0; i < timeVec.size(); i++){
        totalTime += timeVec.at(i);
    }
    int avgTime = int(totalTime / timeVec.size());
    for(int i = 0; i < timeVec.size(); i++){
        variance += pow((timeVec.at(i) - avgTime), 2);
    }
    cout << "average time: " << avgTime << endl;
    cout << "variance: " << variance / timeVec.size() << endl;
    cout << "total served number: " << timeVec.size() << endl;
    cout << "time percent of idle: " << 100 * percent / timer << "\n";
}

int main(int argc, const char * argv[]) {
    if(string(argv[1]) == "bank"){
        bank(stoi(argv[2]));
    }else if(string(argv[1]) == "supermarket"){
        supermarket(stoi(argv[2]));
    }else{
        cout << "invalid input argument\n";
    }
}
