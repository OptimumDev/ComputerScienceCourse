#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <random>
using namespace std;

random_device dev;
mt19937 rng(dev());
uniform_int_distribution<int> dist(1000, 2000);

mutex count_mutex;

void think(int philosopher_num) {
    unique_lock<mutex> lock(count_mutex);
    cout << philosopher_num << " is thinking" << endl;
    lock.unlock();
    this_thread::sleep_for(chrono::milliseconds(dist(rng)));
}

void take_fork(int philosopher_num, int fork_num) {
    unique_lock<mutex> lock(count_mutex);
    cout << "\t" << philosopher_num << " took fork " << fork_num << endl;
    lock.unlock();
}

void eat(int philosopher_num) {
    unique_lock<mutex> lock(count_mutex);
    cout << "\t\t" << philosopher_num << " is eating" << endl;
    lock.unlock();
    this_thread::sleep_for(chrono::milliseconds(dist(rng)));
}

void return_fork(int philosopher_num, int fork_num) {
    unique_lock<mutex> lock(count_mutex);
    cout << "\t\t\t" << philosopher_num << " returned fork " << fork_num << endl;
    lock.unlock();
}

void try_eat(int philosopher_num, int left_fork_num, int right_fork_num, vector<mutex>& forks) {
    auto first_fork_num = min(left_fork_num, right_fork_num);
    auto second_fork_num = max(left_fork_num, right_fork_num);

    unique_lock<mutex> first_lock(forks[first_fork_num]);
    take_fork(philosopher_num, first_fork_num);

    unique_lock<mutex> second_lock(forks[second_fork_num]);
    take_fork(philosopher_num, second_fork_num);

    eat(philosopher_num);

    return_fork(philosopher_num, second_fork_num);
    second_lock.unlock();

    return_fork(philosopher_num, first_fork_num);
    first_lock.unlock();
}

void live(int philosopher_num, int philosophers_count, vector<mutex>& forks) {
    int left_fork_num = (philosopher_num - 1 + philosophers_count) % philosophers_count;
    int right_fork_num = philosopher_num;
    for (int i = 0; i < 100; i++) {
        think(philosopher_num);
        try_eat(philosopher_num, left_fork_num, right_fork_num, forks);
    }
}

int main() {
    int philosophers_count;
    cout << "Enter philosophers count: ";
    cin >> philosophers_count;

    vector<mutex> forks(philosophers_count);
    vector<thread> philosophers(philosophers_count);

    for (int i = 0; i < philosophers_count; i++) {
        philosophers[i] = thread(live, i, philosophers_count, ref(forks));
    }

    for (thread& philosopher : philosophers) {
        philosopher.join();
    }
}