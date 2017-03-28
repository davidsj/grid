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
    }

    square_used = 0;
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

    if (i >= square_used) square_used = i + 1;
    if (j >= square_used) square_used = j + 1;

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

    // Allow any placement within or just outside the allocated square.
    if (i <= square_used && j <= square_used) return true;

    // Otherwise only a single free space is allowed (for new 2's).
    return (i == square_used) && (j == square_used + 1);
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

  int square_used;

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
  if (count % 10000000 == 0) {
    cout << "Searching ... " << count << endl;
    b.print();
  }
  count++;

  if (b.max_possible() <= best) return;
  if (b.done()) {
    cout << "Found score: " << b.score() << endl;
    best = b.score();
    b.print();
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (b.avail(i, j)) {
	board<N> b2 = b;
	b2.place(i, j);
	search(b2, best, count);
      }
    }
  }
}

int main(int argc, char **argv) {
  uint best = 0;
  unsigned long count = 0;
  board<5> b;
  search(b, best, count);
}
