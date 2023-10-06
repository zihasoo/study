#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <future>
#include <utility>

using namespace std;
using chrono::duration_cast;
using chrono::microseconds;

using hc = chrono::high_resolution_clock;
using i64 = long long;

const int num_st = 1'000'000;
const int num_ed = 5'000'000;
int thc = 2, range;

i64 p_sum(int st, int ed) {
    i64 sum = 0;
    for (int i = st; i < ed; ++i) {
        sum += i;
    }
    return sum;
}


i64 single_sum() {
    i64 sum = 0;
    for (int i = num_st; i <= num_ed; ++i) {
        sum += i;
    }
    return sum;
}

i64 multi_sum() { 
    vector<thread> threads;
    vector<i64> results(thc);
    i64 sum = 0;
    threads.reserve(thc);

    for (int i = 0; i < thc; ++i) {
        int st = num_st + i * range;
        int ed = (i == thc - 1) ? num_ed + 1 : num_st + (i + 1) * range;
        threads.emplace_back([i, st, ed, &results]() { results[i] = p_sum(st, ed); });
    }

    for (int i = 0; i < thc; i++) {
        threads[i].join();
        sum += results[i];
    }
    return sum;
}

i64 multi_sum_pf() { //promise, future
    vector<future<i64>> results(thc);
    i64 sum = 0;

    for (int i = 0; i < thc; ++i) {
        int st = num_st + i * range;
        int ed = (i == thc - 1) ? num_ed + 1 : num_st + (i + 1) * range;
        promise<i64> p;
        results[i] = p.get_future();
        thread([lp = move(p), st, ed]()mutable {lp.set_value(p_sum(st, ed));}).detach();
    }

    for (int i = 0; i < thc; i++) {
        sum += results[i].get();
    }
    return sum;
}

i64 multi_sum_pkt() { //packaged_task
    vector<future<i64>> results(thc);
    i64 sum = 0;

    for (int i = 0; i < thc; ++i) {
        int st = num_st + i * range;
        int ed = (i == thc - 1) ? num_ed + 1 : num_st + (i + 1) * range;
        packaged_task<i64(int, int)> task(p_sum);
        results[i] = task.get_future();
        thread(move(task), st, ed).detach();
    }

    for (int i = 0; i < thc; i++) {
        sum += results[i].get();
    }
    return sum;
}

i64 multi_sum_async() { //async
    vector<future<i64>> results(thc);
    i64 sum = 0;

    for (int i = 0; i < thc; ++i) {
        int st = num_st + i * range;
        int ed = (i == thc - 1) ? num_ed + 1 : num_st + (i + 1) * range;
        results[i] = async(launch::async, p_sum, st, ed);
    }

    for (int i = 0; i < thc; i++) {
        sum += results[i].get();
    }
    return sum;
}

i64 multi_sum_atomic() { //atomic<i64>
    vector<thread> threads;
    atomic<i64> sum = 0;
    threads.reserve(thc);

    for (int i = 0; i < thc; ++i) {
        int st = num_st + i * range;
        int ed = (i == thc - 1) ? num_ed + 1 : num_st + (i + 1) * range;
        threads.emplace_back([st, ed, &sum](){sum += p_sum(st, ed);});
    }

    for (int i = 0; i < thc; i++) {
        threads[i].join();
    }
    return sum;
}

i64 multi_sum_mutex() { //mutex
    vector<thread> threads;
    i64 sum = 0;
    mutex m;
    threads.reserve(thc);

    for (int i = 0; i < thc; ++i) {
        int st = num_st + i * range;
        int ed = (i == thc - 1) ? num_ed + 1 : num_st + (i + 1) * range;
        threads.emplace_back([st, ed, &sum, &m](){
            i64 r = p_sum(st, ed);
            m.lock();
            sum += r;
            m.unlock();
        });
    }

    for (int i = 0; i < thc; i++) {
        threads[i].join();
    }
    return sum;
}

double get_exe_time(const function<i64(void)>& func) {
    static i64 ans = single_sum();
    i64 result;
    auto start = hc::now();
    result = func();
    auto end = hc::now();
    if (result != ans)
        cout << "error: " << result << '\n';
    return duration_cast<microseconds>(end - start).count() / 1000.0;
}

void print_avg_exe_times(vector<function<i64(void)>> funcs, vector<string> func_names) {
    const int cnt = 10;
    for (int i = 0; i < func_names.size(); i++)
    {
        cout << "| ";
        cout.width(14);
        cout << func_names[i] << ' ';
    }
    cout << "|\n";
    for (int i = 0; i < funcs.size(); i++)
    {
        cout << "| ";
        double exc_t;
        double sum = 0;
        for (int j = 0; j < cnt; ++j) {
            exc_t = get_exe_time(funcs[i]);
            sum += exc_t;
        }
        cout.width(12);
        cout << sum / cnt << "ms ";
    }
    cout << "|\n";
}

int main() {
    for (int i = 2; i <= 20; i+=2)
    {
        thc = i;
        range = (num_ed - num_st) / thc;
        cout << "| thread count: " << i << '\n';
        print_avg_exe_times({single_sum,
                            multi_sum,
                            multi_sum_pf,
                            multi_sum_pkt,
                            multi_sum_async,
                            multi_sum_atomic,
                            multi_sum_mutex}, 
                            {"ssum()",
                            "msum()",
                            "msum_pf()",
                            "msum_pkt()",
                            "msum_async()",
                            "msum_atomic()",
                            "msum_mutex()"});
    }
}
