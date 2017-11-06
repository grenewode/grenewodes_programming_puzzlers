#include <type_traits>

// This is a one way to define a function which executes at compile time.
// It is conceptually equivalent to
// [](U V) { return v * 2; }
template <typename U, U V>
struct doubler {
  // Doubles the value V which was passed into the struct
  static constexpr auto value = V * 2;
};

// Similar to doubler above, but this is a binary summation operator
template <typename U, U A, U B>
struct add {
  // Take the sum of A + B
  static constexpr auto value = A + B;
};

// A struct which contains a static list of values of type T
template <typename T, T... Values>
struct list {
  // Maps the values in this list into a new list.
  // such that if L = {L_1, L_2, L_3, ...}
  // then L::map<Map> ={Map<T, L_1>::value, Map<T, L_2>::value, Map<T,
  // L_3>::value, ...}
  template <template <typename U, U> class Map>
  using map = /* Fill this in */ void;

  // "reduce" the list. This is a way of collapsing a list into a single value.
  // It is essentially a generalization of summation
  template <template <typename U, U, U> class Op>
  static constexpr auto reduce = /* Fill this in */ 0;

  // Use reduce defined above to implement summation
  static constexpr auto sum = /* Fill this in using the above reduce */ 0;
};

static_assert(std::is_same<list<std::size_t, 1, 2, 3>::map<doubler>,
                           list<std::size_t, 2, 4, 6>>::value,
              "The list is doubled");

static_assert(list<std::size_t, 1, 2, 3>::sum == 6, "Sum of {1, 2, 3} is 6");

int main() {}
