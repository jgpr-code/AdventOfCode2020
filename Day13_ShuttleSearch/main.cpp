#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

int firstBiggerMultiple(int increment, int target) {
    int multiple = 0;
    while (multiple < target) {
        multiple += increment;
    }
    return multiple;
}

int main()
{
    string line;
    getline(cin, line);
    int targetDeparture = stoi(line);
    tuple<int, int> bestBus(numeric_limits<int>::max(), -1);
    getline(cin, line);
    istringstream iss(line);
    for (string token; getline(iss, token, ',');)
    {
        if (token == "x") continue;
        int busId = stoi(token);
        int busDeparture = firstBiggerMultiple(busId, targetDeparture);
        bestBus = min(bestBus, make_tuple(busDeparture, busId));
    }

    int waitTime = get<0>(bestBus) - targetDeparture;
    cout << "Part 1: " << get<1>(bestBus) * waitTime << endl;
}