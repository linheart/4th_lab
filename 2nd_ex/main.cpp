#include <algorithm>
#include <atomic>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_map>
#include <vector>

using namespace std;

class StopWatch {
public:
  void start() { startTime = chrono::high_resolution_clock::now(); }
  void end() { endTime = chrono::high_resolution_clock::now(); }
  void duration() {
    cout << "Execution time: ";
    cout << fixed << setprecision(9)
         << chrono::duration_cast<chrono::duration<double>>(endTime - startTime)
                .count()
         << " seconds" << endl;
  }

private:
  chrono::time_point<chrono::high_resolution_clock> startTime;
  chrono::time_point<chrono::high_resolution_clock> endTime;
};

class Store {
public:
  void addRecords(size_t records) {
    for (size_t i = 0; i < records; i++) {
      Receipt reciept = genReceipt();
      {
        lock_guard<mutex> lock(mu);
        items.push_back(reciept);
      }
    }
  }

  void printProductDetails() const {
    uniform_int_distribution<> dis(0, productNames.size() - 1);
    string targetProduct = productNames[dis(gen)];

    int totalSold = 0;
    cout << "Product: " << targetProduct << endl;
    cout << "Receipts containing the product:" << endl;

    for (const auto &item : items) {
      auto product = find_if(item.products.begin(), item.products.end(),
                             [targetProduct](const Product &p) {
                               return p.name == targetProduct;
                             });
      if (product != item.products.end()) {
        totalSold += product->quantity;
        cout << "Receipt: " << item.receiptNum << endl;
        cout << "  Quantity: " << product->quantity
             << ", Price: " << product->price
             << ", Total: " << product->quantity * product->price << endl;
      }
    }

    cout << "Total sold: " << totalSold << " units with total price: "
         << productPrices.at(targetProduct) * totalSold << endl;
    cout << "------------------" << endl;
  }

private:
  struct Product {
    string name;
    double price;
    unsigned quantity;
  };

  struct Receipt {
    int receiptNum;
    vector<Product> products;
  };

  const vector<string> productNames = {"Apple", "Banana", "Milk",      "Bread",
                                       "Eggs",  "Cheese", "Chocolate", "Juice",
                                       "Pasta", "Coffee"};

  unordered_map<string, double> productPrices = genPrices();

  Receipt genReceipt() {
    Receipt reciept;
    reciept.receiptNum = counter++;
    uniform_int_distribution<> dis(1, 5);
    size_t productNums = dis(gen);

    for (size_t i = 0; i < productNums; i++) {
      Product product = genProduct();
      auto p = find_if(
          reciept.products.begin(), reciept.products.end(),
          [product](const Product &p) { return p.name == product.name; });
      if (p != reciept.products.end()) {
        p->quantity += product.quantity;
      } else {
        reciept.products.push_back(product);
      }
    }

    return reciept;
  }

  Product genProduct() {
    Product product;
    uniform_int_distribution<> dis(0, productPrices.size() - 1);
    string name = productNames[dis(gen)];
    double price = productPrices.at(name);
    unsigned quantity = dis(gen) + 1;

    return {name, price, quantity};
  }

  unordered_map<string, double> genPrices() {
    unordered_map<string, double> productPrices;
    uniform_real_distribution<> dis(0.5, 20.0);

    for (auto product : productNames) {
      productPrices[product] = round(dis(gen) * 10) / 10;
    }

    return productPrices;
  }

private:
  vector<Receipt> items;
  mutex mu;
  static thread_local mt19937 gen;
  static atomic<int> counter;
};

thread_local mt19937 Store::gen = mt19937(random_device{}());
atomic<int> Store::counter = 1;

int main() {
  Store ex1;
  StopWatch sp;
  vector<thread> threads;

  size_t records;
  cout << "Enter the number of entries: ";
  cin >> records;

  size_t numThreads;
  cout << "Enter the number of threads: ";
  cin >> numThreads;

  cout << "Multithread" << endl;
  sp.start();
  for (size_t i = 0; i < numThreads; i++) {
    threads.push_back(thread(&Store::addRecords, &ex1, records / numThreads));
  }

  for (auto &it : threads) {
    it.join();
  }
  sp.end();
  ex1.printProductDetails();
  sp.duration();

  cout << "single-thread" << endl;
  Store ex2;
  sp.start();
  ex2.addRecords(records);
  sp.end();
  ex2.printProductDetails();
  sp.duration();
  return 0;
}
