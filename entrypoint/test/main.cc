//
// Created by rplaz on 2025-02-24.
//
#include "engine/base.h"

#include <iostream>

using namespace std;
using namespace nycatech;

int main() {
  Matrix<3, 3> a({1, 2, 3, 4, 5, 6, 7, 8, 9});
  Matrix<3, 3> b({1, 2, 3, 4, 5, 6, 7, 8, 9});
  auto c = a * b;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      cout << c.at(i, j) << " ";
    }
    cout << endl;
  }
  return 0;
}