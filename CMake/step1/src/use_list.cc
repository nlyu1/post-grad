#include <iostream>
#include <vector>
// We can use use_list.h as if it's immediately present because cmake will do that for us
#include "use_list.h"

using namespace std; 

int main() {
    std::vector<int> v = {0, 1, 2, 3, 4, 5}; 

    for (const auto& i : v) {
        cout << i << " ";
    }
    cout << endl;

    cout << "VERSION_MAJOR: " << VERSION_MAJOR << endl;
    cout << "VERSION_MINOR: " << VERSION_MINOR << endl;
}