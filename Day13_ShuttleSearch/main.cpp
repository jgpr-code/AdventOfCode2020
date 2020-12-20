#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <variant>

using namespace std;
using ll = long long;

ll firstBiggerMultiple(ll increment, ll target)
{
    ll multiple = 0;
    while (multiple < target)
    {
        multiple += increment;
    }
    return multiple;
}

bool isValidTimeForAllBusses(ll time, const vector<tuple<ll, ll>>& busEquations)
{
    if (time < 0) return false;
    for (const auto& t : busEquations) {
        ll busId = get<0>(t);
        ll timeOffset = get<1>(t);
        if ((time + timeOffset) % busId != 0) return false;
    }
    return true;
}

bool isPrime(ll n) {
    if (n <= 3) {
        return n > 1;
    }
    if (n % 2 == 0 || n % 3 == 0) return false;
    ll i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
        i += 6;
    }
    return true;
}

int main()
{
    string line;
    getline(cin, line);
    ll targetDeparture = stoll(line);
    tuple<ll, ll> bestBus(numeric_limits<ll>::max(), -1);
    getline(cin, line);
    istringstream iss(line);
    string token;
    vector<tuple<ll, ll>> busses;
    vector<tuple<ll, ll>> busEquations;
    for (ll i = 0; getline(iss, token, ','); ++i)
    {
        if (token == "x")
        {
            continue;
        }
        ll busId = stoll(token);
        ll busDeparture = firstBiggerMultiple(busId, targetDeparture);
        auto bus = make_tuple(busDeparture, busId);
        bestBus = min(bestBus, bus);
        busses.push_back(bus);
        auto busEquation = make_tuple(busId, i);
        busEquations.push_back(busEquation);
    }
    
    ll waitTime = get<0>(bestBus) - targetDeparture;
    cout << "Part 1: " << get<1>(bestBus) * waitTime << endl;

    sort(busEquations.begin(), busEquations.end());
    for (const auto& t : busEquations) {
        cout << get<0>(t) << "(isPrime: " << isPrime(get<0>(t)) << "), " << get<1>(t) << endl; // all numbers are indeed prime -> chinese remainder
    }
    ll increment = get<0>(busEquations.back());
    ll tryTime = ((100000000000000 / increment) + 1) * increment;
    while (true) {
        // cout << "try: " << tryTime << endl;
        if (isValidTimeForAllBusses(tryTime, busEquations)) break;
        tryTime += increment;
    }

    cout << "Part 2: " << tryTime << endl;
}