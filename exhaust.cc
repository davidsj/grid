#include <cstdio>
#include <iostream>
#include <cmath>
#include <vector>

#include <unistd.h>

using namespace std;

typedef unsigned int uint;

uint nth_fib(uint n, uint x, uint y) {
  float sqrt5 = sqrt(5.);
  float phi = (1. + sqrt5)/2.;
  float psi = (1. - sqrt5)/2.;

  float a = (x - psi*y)/sqrt5 * pow(phi, n);
  float b = (phi*y - x)/sqrt5 * pow(psi, n);

  return round(a*phi + b*psi);
}

struct point {
  int i, j;
};

template <uint N>
class board {
public:
  board() {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
	vals[i][j] = (i == j) ? 1 : 0;
      }

      row_max[i] = 1;
      col_max[i] = 1;
      row_init[i] = false;
      col_init[i] = false;
    }

    max[0] = max[1] = 1;
    n_free = N * (N - 1);

    last_place.i = -1;
    last_place.j = -1;
  }

  void place(int i, int j) {
    uint &new_val = vals[i][j];
    new_val = row_max[i] + col_max[j];

    // We only ever place in globally ascending order, so we know this
    // increases the max.
    row_max[i] = new_val;
    col_max[j] = new_val;

    max[1] = max[0];
    max[0] = new_val;
    n_free--;

    row_init[i] = true;
    col_init[j] = true;

    last_place.i = i;
    last_place.j = j;
  }

  int free_count() const {
    return n_free;
  }

  bool done() const {
    return n_free == 0;
  }

  uint score() const {
    return max[0];
  }

  uint max_possible() const {
    return nth_fib(n_free, max[0], max[1]);
  }

  bool avail(int i, int j) const {
    if (vals[i][j] != 0) return false;
    uint new_val = row_max[i] + col_max[j];

    // Only allow globally ascending placement.
    if (new_val < max[0]) return false;
      
    // The remaining logic prevents certain placements which would be
    // permutation-equivalent to others.

    // Only allow placing the same number twice if the second point
    // comes after the first point.
    if (new_val == max[0]) {
      if (i < last_place.i) return false;
      if (i == last_place.i && j <= last_place.j) return false;
    }

    if (row_init[i] && col_init[j]) return true;

    // If row is initialized but not column, only allow first
    // uninitialized column (excluding diagonal).
    if (row_init[i]) {
      for (int j2 = 0; j2 < N; j2++) {
	if (i != j2 && !col_init[j2]) return j == j2;
      }
    }

    // If column is initialized but not row, only allow first
    // uninitialized row (excluding diagonal).
    if (col_init[j]) {
      for (int i2 = 0; i2 < N; i2++) {
	if (j != i2 && !row_init[i2]) return i == i2;
      }
    }

    // If neither row nor column is initialized, only allow placement at
    // first square in both an uninitialized row and column. TODO: Make
    // this not quadratic by tracking position of next free slot.
    for (int i2 = 0; i2 < N; i2++) {
      if (row_init[i2]) continue;

      for (int j2 = 0; j2 < N; j2++) {
	if (i2 != j2 && !col_init[j2]) return (i == i2) && (j == j2);
      }
    }
  }

  void print() const {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
	if (vals[i][j] != 0) printf(" %3d ", vals[i][j]);
	else if (avail(i, j)) printf("[%3d]", row_max[i] + col_max[j]);
	else printf("   _ ");
      }
      printf("\n");
    }
    printf("\n");
  }

private:
  uint vals[N][N];

  bool row_init[N];
  bool col_init[N];

  uint row_max[N];
  uint col_max[N];

  uint max[2];
  uint n_free;

  point last_place;
};

template <uint N>
void search(const board<N> &b, uint &best, unsigned long &count) {
  if (count % 100000 == 0) {
    cout << "Searching ... " << count << endl;
    b.print();
  }
  count++;

  // if (b.max_possible() < best) {
  //   return;
  // }
  if (b.done() && b.score() >= best) {
    cout << "Found score: " << b.score() << endl;
    best = b.score();
    b.print();
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (b.avail(i, j)) {
	board<N> b2 = b;
	b2.place(i, j);
	if (b.free_count() == 11) {
	  cout << "PLACING: " << i << ", " << j << endl;
	  b2.print();
	}
	search(b2, best, count);
      }
    }
  }
}

int main(int argc, char **argv) {
  // uint best = 0;
  // unsigned long count = 0;
  // board<4> b;
  // search(b, best, count);

  board<4> b;
  b.place(0, 1);
  b.place(1, 0);
  b.print();
}
