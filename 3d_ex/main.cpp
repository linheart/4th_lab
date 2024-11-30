#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

class ReadersWriters {
 public:
  bool writersPriority{true};

  void write(size_t id, size_t iters) {
    for (size_t i = 0; i < iters; i++) {
      startWrite();
      {
        lock_guard<mutex> lock(mu);
        cout << "Writer " << id << " writing\n";
      }
      this_thread::sleep_for(chrono::milliseconds(1000));
      stopWrite();
    }
  }

  void read(size_t id, size_t iters) {
    for (size_t i = 0; i < iters; i++) {
      startRead();
      {
        lock_guard<mutex> lock(mu);
        cout << "Reader " << id << " reading\n";
      }
      stopRead();
    }
  }

 private:
  void startWrite() {
    unique_lock<mutex> lock(mu);
    while (writers > 0 || (!writersPriority && readers > 0)) {
      cv.wait(lock);
    }
    writers++;
  }

  void stopWrite() {
    unique_lock<mutex> lock(mu);
    writers--;
    if (writers == 0) {
      cv.notify_all();
    }
  }

  void startRead() {
    unique_lock<mutex> lock(mu);
    while (writers > 0) {
      cv.wait(lock);
    }
    readers++;
  }

  void stopRead() {
    unique_lock<mutex> lock(mu);
    readers--;
    cv.notify_all();
  }

 private:
  mutex mu;
  condition_variable cv;
  int writers{0};
  int readers{0};
};

void execute(ReadersWriters& rw) {
  size_t iterations = 1;
  size_t write_limit = 2;
  size_t read_limit = 4;

  cout << (rw.writersPriority ? "Writer's" : "\nReader") << " priority" << endl;

  vector<thread> threadsWriter;
  vector<thread> threadsReader;

  for (size_t i = 0; i < write_limit; i++) {
    threadsWriter.push_back(
        thread(&ReadersWriters::write, &rw, i + 1, iterations));
  }

  for (size_t i = 0; i < read_limit; i++) {
    threadsReader.push_back(
        thread(&ReadersWriters::read, &rw, i + 1, iterations));
  }

  for (auto& t : threadsWriter) {
    t.join();
  }

  for (auto& t : threadsReader) {
    t.join();
  }
}

int main() {
  ReadersWriters rw;
  execute(rw);

  rw.writersPriority = false;
  execute(rw);

  return 0;
}
