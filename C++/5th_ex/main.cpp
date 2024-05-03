#include <cmath>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

#define series 10

void err();
vector<double> init_arr(int bound);
void fib(vector<double> &arr, int a, int b);

int main() {
  int a, b;
  int m = 16;

  cout << "Input a and b: ";
  cin >> a >> b;

  if (b <= 0 || b >= a)
    err();

  vector<double> arr = init_arr(a);
  for (auto it : arr)
    cout << it << ' ';
  cout << endl;

  fib(arr, a, b);

  for (size_t i = a; i < arr.size(); i++)
    cout << arr[i] << ' ';

  return 0;
}

void err() {
  cerr << "Invalid input" << endl;
  exit(EXIT_FAILURE);
}

vector<double> init_arr(int bound) {
  vector<double> arr = {};
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<double> unif(0., 1.);
  for (int i = 0; i < bound; i++)
    arr.push_back(round(unif(gen) * 10) / 10);
  return arr;
}

void fib(vector<double> &arr, int a, int b) {
  for (int i = a; i < series + a; i++) {
    double tmp = arr[i - a] - arr[i - b];
    if (tmp < 0)
      tmp++;
    arr.push_back(tmp);
  }
}
