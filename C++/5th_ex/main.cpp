#include <iostream>
#include <random>
#include <vector>

using namespace std;

#define series 10

void err();
vector<double> init_arr(int bound);
vector<double> fib(vector<double> arr, int a, int b);

int main() {
  int a, b;
  int m = 16;

  cout << "Input a and b: ";
  cin >> a >> b;

  if (b <= 0 || b >= a)
    err();

  vector<double> arr = init_arr(a);
  vector<double> res = fib(arr, a, b);

  for (auto it : res)
    cout << it << ' ';

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
  for (int i = 0; i <= bound; i++)
    arr.push_back(unif(gen));

  return arr;
}

vector<double> fib(vector<double> arr, int a, int b) {
  vector<double> res = {};
  for (int i = a + 1; i <= series + a; i++) {
    double tmp = arr[i - a] - arr[i - b];
    if (tmp < 0)
      tmp++;
    arr.push_back(tmp);
    res.push_back(tmp);
  }
  return res;
}
