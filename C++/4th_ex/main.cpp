#include <cstdint>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int get_rounds();

void play_game(function<bool(int32_t, vector<bool>, vector<bool>)>,
               function<bool(int32_t, vector<bool>, vector<bool>)>);

void play_round(function<bool(int32_t, vector<bool>, vector<bool>)>, int32_t &,
                vector<bool> &,
                function<bool(int32_t, vector<bool>, vector<bool>)>, int32_t &,
                vector<bool> &, int32_t);

bool always_betray(int32_t, vector<bool>, vector<bool>);
bool always_cooperate(int32_t, vector<bool>, vector<bool>);
bool tit_for_tat(int32_t, vector<bool>, vector<bool>);

int main() {

  play_game(always_betray, always_cooperate);
  play_game(always_betray, always_betray);
  play_game(always_cooperate, always_cooperate);
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

void play_round(
    function<bool(int32_t, vector<bool>, vector<bool>)> self_strategy,
    int32_t &self_score, vector<bool> &self_choices,
    function<bool(int32_t, vector<bool>, vector<bool>)> enemy_strategy,
    int32_t &enemy_score, vector<bool> &enemy_choices, int32_t round_number) {

  bool self_action = self_strategy(round_number, self_choices, enemy_choices);
  bool enemy_action = enemy_strategy(round_number, self_choices, enemy_choices);

  self_choices.push_back(self_action);
  enemy_choices.push_back(enemy_action);

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

void play_game(
    function<bool(int32_t, vector<bool>, vector<bool>)> self_strategy,
    function<bool(int32_t, vector<bool>, vector<bool>)> enemy_strategy) {
  int32_t num_rounds = get_rounds();
  vector<bool> self_choices, enemy_choices;
  int32_t self_score = 0;
  int32_t enemy_score = 0;

  for (int i = 1; i <= num_rounds; i++)
    play_round(self_strategy, self_score, self_choices, enemy_strategy,
               enemy_score, enemy_choices, i);

  cout << num_rounds << " rounds\nself score - " << self_score
       << "\nenemy score - " << enemy_score << endl;
}

int get_rounds() {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(100, 200);
  return dis(gen);
}
