#ifndef PRIMITIVES_H // PRIMITIVES_H
#define PRIMITIVES_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

#define THREAD_COUNT 5

class SemaphoreSlim {
public:
  explicit SemaphoreSlim(size_t initialPermits) : permits(initialPermits) {}

  void acquire() {
    std::unique_lock<std::mutex> lock(mu);
    cv.wait(lock, [this]() { return permits > 0; });
    permits--;
  }

  void release() {
    std::unique_lock<std::mutex> lock(mu);
    permits++;
    cv.notify_one();
  }

private:
  std::mutex mu;
  std::condition_variable cv;
  size_t permits;
};

class SpinLock {
public:
  void lock() {
    for (;;) {
      if (!lock_.exchange(true, std::memory_order_acquire)) {
        break;
      }
      while (lock_.load(std::memory_order_relaxed)) {
        __builtin_ia32_pause();
      }
    }
  }

  void unlock() { lock_.store(false, std::memory_order_release); }

private:
  std::atomic<bool> lock_{false};
};

class SpinWait {
public:
  void lock() {
    int attempts = 0;
    for (;;) {
      if (!lock_.exchange(true, std::memory_order_acquire)) {
        break;
      }

      ++attempts;
      if (attempts < spinThreshold) {
        __builtin_ia32_pause();
      } else {
        std::this_thread::yield();
        if (attempts >= yieldThreshold) {
          std::this_thread::sleep_for(sleepDuration);
          attempts = 0;
        }
      }
    }
  }

  void unlock() { lock_.store(false, std::memory_order_release); }

private:
  static constexpr int spinThreshold = 10;
  static constexpr int yieldThreshold = 50;
  static constexpr std::chrono::milliseconds sleepDuration{1};
  std::atomic<bool> lock_{false};
};

class Monitor {
public:
  void produce() {
    std::unique_lock<std::mutex> lock(mu);
    waitingThreads++;
    if (waitingThreads == totalThreads) {
      ready = true;
      cv.notify_all();
    }
  }

  void consume() {
    std::unique_lock<std::mutex> lock(mu);
    cv.wait(lock, [this]() { return ready; });
    waitingThreads--;
    if (waitingThreads == 0) {
      ready = false;
    }
  }

private:
  std::mutex mu;
  std::condition_variable cv;
  bool ready{false};
  size_t waitingThreads{0};
  const size_t totalThreads{THREAD_COUNT};
};

#endif // PRIMITIVES_H
