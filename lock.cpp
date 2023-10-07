#include <atomic>

using namespace std;

class tas_lock {
    atomic<bool> value;

    tas_lock() : value(false) {}

    tas(bool new_value) {
        bool prior = value;
        
    }
}

class ttas_lock {

}