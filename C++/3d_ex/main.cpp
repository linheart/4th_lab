#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#define s1 50
#define s2 100
#define s3 1000
#define interval 10.

using namespace std;

vector<int> init(int size);
double chi_sqr(vector<int> arr);

int main() {
  cout << "50 nums: " << chi_sqr(init(s1)) << endl;
  cout << "100 nums: " << chi_sqr(init(s2)) << endl;
  cout << "1000 nums: " << chi_sqr(init(s3)) << endl;
  return 0;
}

double chi_sqr(vector<int> arr) {
  double Vexp = arr.size() / interval;
  double chi_sqr = 0.;

  for (size_t i = 0; i < interval; i++) {
    double Vn = 0.;

    size_t start = arr.size() / interval * i;
    size_t end = start + arr.size() / interval;

    for (size_t j = start; j < end; j++) {
      if (arr[j] >= start && arr[j] <= end)
        Vn++;
    }
    chi_sqr += pow(Vn - Vexp, 2) / Vexp;
  }
  return chi_sqr;
}

vector<int> init(int size) {
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  knuth_b engine(seed);
  uniform_int_distribution<int> distribution(1, 100);
  vector<int> arr = {};

  for (int i = 0; i < size; i++)
    arr.push_back(distribution(engine));

  return arr;
}
