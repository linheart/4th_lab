#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

using namespace std;

int main() {
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  knuth_b engine(seed);

  uniform_int_distribution<int> distribution1(-300, -150);
  uniform_int_distribution<int> distribution2(150, 300);
  uniform_int_distribution<int> selector(0, 1);

  int n = 10;
  vector<int> arr = {};
  for (int i = 0; i < n; i++)
    arr.push_back(selector(engine) ? distribution2(engine)
                                   : distribution1(engine));

  cout << "-----------------------------------" << endl;

  for (auto it : arr)
    cout << it << ' ';

  cout << "\n-----------------------------------" << endl;

  cout << "mean - " << accumulate(arr.begin(), arr.end(), 0) / (double)n
       << endl;
  cout << "min - " << *min_element(arr.begin(), arr.end()) << endl;
  cout << "max - " << *max_element(arr.begin(), arr.end()) << endl;

  cout << "-----------------------------------" << endl;

  reverse(arr.begin(), arr.end());
  cout << "reverse massive - ";
  for (auto it : arr)
    cout << it << ' ';
  cout << "\nsort massive - ";
  sort(arr.begin(), arr.begin() + n / 2);
  sort(arr.begin() + n / 2, arr.end(), greater<>());
  for (auto it : arr)
    cout << it << ' ';

  cout << "\n-----------------------------------" << endl;

  uniform_int_distribution<int> symbols(33, 126);
  vector<char> symb_arr = {};

  for (int i = 0; i < n; i++)
    symb_arr.push_back(symbols(engine));

  cout << "before - ";
  for (auto it : symb_arr)
    cout << it << ' ';

  cout << "\nafter - ";
  symb_arr.erase(remove_if(symb_arr.begin(), symb_arr.end(),
                           [](char x) {
                             return (x >= '0' && x <= '9') ||
                                    (x >= 'a' && x <= 'z');
                           }),
                 symb_arr.end());
  for (auto it : symb_arr)
    cout << it << ' ';

  cout << "\n-----------------------------------" << endl;
  cout << "before:" << endl;

  vector<int> arr1 = {};
  vector<int> arr2 = {};
  n = 3;

  uniform_int_distribution<int> digits(0, 9);

  for (int i = 0; i < n; i++) {
    arr1.push_back(digits(engine));
    arr2.push_back(digits(engine));
  }

  for (auto it : arr1)
    cout << it << ' ';
  cout << endl;
  for (auto it : arr2)
    cout << it << ' ';

  cout << "\nafter;" << endl;

  char first_arr1 = arr1.front();
  char first_arr2 = arr2.front();

  for (int i = 0; i < n - 1; i++) {
    arr1[i] = arr1[i + 1];
    arr2[i] = arr2[i + 1];
  }
  arr1[n - 1] = first_arr2;
  arr2[n - 1] = first_arr1;

  for (auto it : arr1)
    cout << it << ' ';
  cout << endl;
  for (auto it : arr2)
    cout << it << ' ';
  cout << endl;

  return 0;
}
