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

namespace __impl {
// This is a helper struct which list will use to apply reduction operations to
// itself. The template struct expects the following parameters:
// T: The type of the elements in the parameter pack to be reduced.
// Op: The operator to apply to elements of the pack
// Values...: The pack of values to be reduced
// Also, notice that reduction in this case is as follows:
// Given a list<T, 1, 2, 3, 4>
// it will reduced as (1 op (2 op (3 op 4)))
template <typename T, template <typename U, U, U> class Op, T... Values>
struct reducer {
  // We don't want to deal with empty lists here, because that needs either
  // 1) a default value, which can be nasty and ill defined
  // or 2) an initial value, but that makes doing the recursive unfolding a
  // little bit more complicated. It should be noted that this is all trival
  // with the introduction of c++17 fold expresions:
  // http://en.cppreference.com/w/cpp/language/fold
  static_assert(sizeof...(Values) == 0,
                "reducing an empty list is not supported");
};

// Define reducer for parameter packs containing a single value.
template <typename T, template <typename U, U, U> class Op, T Value>
struct reducer<T, Op, Value> {
  // Simply default to the single value in the parameter pack
  static constexpr auto value = Value;
};

// Define reducer for parameters packs containing only two values
template <typename T, template <typename U, U, U> class Op, T A, T B>
struct reducer<T, Op, A, B> {
  // Now the value is the result of A op B.
  static constexpr auto value = Op<T, A, B>::value;
};

// Define the reducer for the case where there are more than two values.
// The compiler will not use this definition unless the others have failed to
// match. See
// http://en.cppreference.com/w/cpp/language/definition#One_Definition_Rule
// for more information
template <typename T, template <typename U, U, U> class Op, T Head, T... Tail>
struct reducer<T, Op, Head, Tail...> {
  // Recursively take the sum of the elements in the pack
  // If we have a list called L,
  // Then this is equivelent to saying:
  // 1. let Head = the first element in L
  // 2. let Tail = L with the first element removed
  // 3. let value = Head Op reducer(Op, Tail...)
  static constexpr auto value = Op<
      T,
      Head,  // Apply Op to Head and the reduction of all the remaining values
      reducer<T, Op,
              Tail...>::value  // Reduce all the remaining values
      >::value;
};

}  // namespace __impl

// A struct which contains a static list of values of type T
template <typename T, T... Values>
struct list {
  // Maps the values in this list into a new list.
  // such that if L = {L_1, L_2, L_3, ...}
  // then L::map<Map> ={Map<T, L_1>::value, Map<T, L_2>::value, Map<T,
  // L_3>::value, ...}
  template <template <typename U, U> class Map>
  using map =
      list<T,  // This does not change the type of the list
           Map<T, Values>::value...  // Here is where we apply the map. Notice
                                     // that we do this just by using the ...
                                     // operator to expand the parameter pack
           >;

  // "reduce" the list. This is a way of collapsing a list into a single value.
  // It is essentially a generalization of summation
  template <template <typename U, U, U> class Op>
  static constexpr auto reduce = __impl::reducer<T, Op, Values...>::value;

  // Use reduce defined above to implement summation
  static constexpr auto sum = list::reduce<add>;
};

static_assert(std::is_same<list<std::size_t, 1, 2, 3>::map<doubler>,
                           list<std::size_t, 2, 4, 6>>::value,
              "The list is doubled");

static_assert(list<std::size_t, 1, 2, 3>::sum == 6, "Sum of {1, 2, 3} is 6");

int main() {}
