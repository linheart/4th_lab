#include <barrier>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <semaphore>

#include "primitives.h"

using namespace std;

class StopWatch {
public:
  void start() { startTime = chrono::high_resolution_clock::now(); }
  void elapsed() {
    auto endTime = chrono::high_resolution_clock::now();
    cout << fixed << setprecision(9)
         << chrono::duration_cast<chrono::duration<double>>(endTime - startTime)
                .count()
         << endl;
  }

private:
  chrono::time_point<chrono::high_resolution_clock> startTime;
};

class chGen {
public:
  void mutexMethod(bool lock) { lock ? mu.lock() : mu.unlock(); }

  void semaphoreMethod(bool lock) { lock ? sem.acquire() : sem.release(); }

  void semaphoreSlimMethod(bool lock) { lock ? sem.acquire() : sem.release(); }

  void barrierMethod(bool lock) {
    if (lock) {
      mu.lock();
    } else {
      mu.unlock();
      myBarrier.arrive_and_wait();
    }
  }

  void spinLockMethod(bool lock) { lock ? spinLock.lock() : spinLock.unlock(); }

  void spinWaitMethod(bool lock) { lock ? spinWait.lock() : spinWait.unlock(); }

  void monitorMethod(bool lock) {
    lock ? monitor.produce() : monitor.consume();
  }

  void gen(const function<void(bool)> &method, const string &name) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(32, 126);

    for (size_t i = 0; i < charCount; i++) {
      method(true);
      char ch = dis(gen);
      cout << name << ": " << ch << "\n";
      method(false);
    }
  }

private:
  mutex mu;
  binary_semaphore sem{1};
  SemaphoreSlim semSlim{1};
  barrier<> myBarrier{THREAD_COUNT};
  SpinLock spinLock;
  SpinWait spinWait;
  Monitor monitor;
  static constexpr size_t charCount = 1;
};

void execution(const function<void(bool)> &method, chGen &generator,
               const string &name) {
  StopWatch sp;
  thread th[THREAD_COUNT];
  sp.start();
  for (size_t i = 0; i < THREAD_COUNT; i++) {
    th[i] =
        thread([&generator, &method, &name]() { generator.gen(method, name); });
  }

  for (size_t i = 0; i < THREAD_COUNT; i++) {
    th[i].join();
  }

  cout << "Time taken by " << name << " threads: ";
  sp.elapsed();
}

int main() {
  chGen generator;

  execution([&generator](bool lock) { generator.mutexMethod(lock); }, generator,
            "Mutex");
  execution([&generator](bool lock) { generator.semaphoreMethod(lock); },
            generator, "Semaphore");
  execution([&generator](bool lock) { generator.semaphoreSlimMethod(lock); },
            generator, "SemaphoreSlim");
  execution([&generator](bool lock) { generator.barrierMethod(lock); },
            generator, "Barrier");
  execution([&generator](bool lock) { generator.spinLockMethod(lock); },
            generator, "SpinLock");
  execution([&generator](bool lock) { generator.spinWaitMethod(lock); },
            generator, "SpinWait");
  execution([&generator](bool lock) { generator.monitorMethod(lock); },
            generator, "Monitor");
  return 0;
}
