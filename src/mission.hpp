#pragma once
#include <string>
using namespace std;

class Mission {
public:
    string name;
    string description;
    bool isCompleted;

    Mission() {
        name = "";
        description = "";
        isCompleted = false;
    }

    Mission(string n, string d) {
        name = n;
        description = d;
        isCompleted = false;
    }
};
