#include <functional>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int get_rounds();
void update_score(bool, int32_t &, bool, int32_t &);
bool always_betray(int32_t, vector<bool>, vector<bool>);
bool always_cooperate(int32_t, vector<bool>, vector<bool>);
bool tit_for_tat(int32_t, vector<bool>, vector<bool>);

int main() {
  function<bool(int32_t, vector<bool>, vector<bool>)> alg1 = always_cooperate;
  function<bool(int32_t, vector<bool>, vector<bool>)> alg2 = always_betray;
  function<bool(int32_t, vector<bool>, vector<bool>)> alg3 = tit_for_tat;

  int32_t num_rounds = get_rounds();
  vector<bool> self_choices, enemy_choices;
  int32_t self_score = 0;
  int32_t enemy_score = 0;

  for (int i = 0; i < num_rounds; i++) {
    bool self_action = alg1(i, self_choices, enemy_choices);
    bool enemy_action = alg2(i, self_choices, enemy_choices);

    self_choices.push_back(self_action);
    enemy_choices.push_back(enemy_action);

    update_score(self_action, self_score, enemy_action, enemy_score);
  }

  cout << num_rounds << " rounds\nself score - " << self_score
       << "\nenemy score - " << enemy_score << endl;

  return 0;
}

bool always_betray(int32_t round_number, vector<bool> self_choices,
                   vector<bool> enemy_choices) {
  return false;
}

bool always_cooperate(int32_t round_number, vector<bool> self_choices,
                      vector<bool> enemy_choices) {
  return true;
}

bool tit_for_tat(int32_t round_number, vector<bool> self_choices,
                 vector<bool> enemy_choices) {
  if (enemy_choices.empty() || enemy_choices.back())
    return true;
  return false;
}

void update_score(bool self_action, int32_t &self_score, bool enemy_action,
                  int32_t &enemy_score) {
  if (!self_action && !enemy_action) {
    self_score += 4;
    enemy_score += 4;
  } else if (self_action && enemy_action) {
    self_score += 24;
    enemy_score += 24;
  } else if (self_action && !enemy_action)
    enemy_score += 20;
  else
    self_score += 20;
}
int get_rounds() {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(100, 200);
  return dis(gen);
}
