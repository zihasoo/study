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
    return p_sum(num_st, num_ed + 1);
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
        thread([lp = move(p), st, ed]()mutable { lp.set_value(p_sum(st, ed)); }).detach();
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
        threads.emplace_back([st, ed, &sum]() { sum += p_sum(st, ed); });
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
        threads.emplace_back([st, ed, &sum, &m]() {
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

double get_exe_time(const function<i64(void)> &func) {
    static i64 ans = single_sum();
    i64 result;
    auto start = hc::now();
    result = func();
    auto end = hc::now();
    if (result != ans)
        cout << "error: " << result << '\n';
    return duration_cast<microseconds>(end - start).count() / 1000.0;
}

void avg_exe_printmd(const vector<function<i64(void)>>& funcs, const vector<string>& names) {
    const int cnt = 10;
    cout << "| thread count ";
    for (const auto &name: names) {
        cout << "| ";
        cout << name << ' ';
    }
    cout << "|\n";
    for (int i = 0; i < names.size() + 1; ++i) {
        cout << "|------";
    }
    cout << "|\n";
    for (int i = 2; i <= 20; i += 2) {
        thc = i;
        range = (num_ed - num_st) / thc;
        cout << "| " << i;
        for (const auto &func: funcs) {
            cout << "| ";
            double exc_t;
            double sum = 0;
            for (int j = 0; j < cnt; ++j) {
                exc_t = get_exe_time(func);
                sum += exc_t;
            }
            cout << sum / cnt << "ms ";
        }
        cout << "|\n";
    }
}

void avg_exe_printpy(const vector<function<i64(void)>>& funcs, const vector<string>& names) {
    const int cnt = 10;
    for (int i = 0; i < funcs.size(); ++i) {
        cout << names[i] << " = [";
        for (int j = 2; j <= 20; j += 2) {
            thc = j;
            range = (num_ed - num_st) / thc;
            double exc_t;
            double sum = 0;
            for (int k = 0; k < cnt; ++k) {
                exc_t = get_exe_time(funcs[i]);
                sum += exc_t;
            }
            cout << sum / cnt;
            if (j != 20)
                cout << ", ";
        }
        cout << "]\n";
    }
}

int main() {
    vector<function<i64(void)>> funcs = {single_sum,
                                         multi_sum,
                                         multi_sum_pf,
                                         multi_sum_pkt,
                                         multi_sum_async,};
    vector<string> names = {"ssum",
                            "msum",
                            "msum_fp",
                            "msum_pt",
                            "msum_as",};
    avg_exe_printpy(funcs, names);
}
