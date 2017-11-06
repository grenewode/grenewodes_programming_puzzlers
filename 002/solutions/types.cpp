#include <iostream>
#include <type_traits>
#include <utility>
using std::pair;

// Some structs for testing purposes
struct A {};
struct B {};
struct C {};
struct D {};
struct E {};

struct W {};
struct X {};
struct Y {};
struct Z {};

// I am giving you this. Basically, this just gives you a pack of types.
template <class... Items>
struct list {
  // The number of types in the pack of types
  static constexpr auto size =
      // Notice the sizeof...([parameter pack]) operator.
      // This is just a simple way to count the number of items in a parameter
      // pack
      sizeof...(Items);
};

// This is the first thing you will need to fill out!
// This is allows you to join two lists.
template <class, class>
struct join;

template <class... A, class... B>
struct join<list<A...>, list<B...>> {
  // This is one of the times when parameter packs are really nice.
  // I can just exand one, then exand the other
  using type = list<A..., B...>;
};

static_assert(std::is_same<typename join<list<A, B, C>, list<X, Y, Z>>::type,
                           list<A, B, C, X, Y, Z>>::value,
              "Joining list<A, B, C> and list<X, Y, Z> should yield list<A, B, "
              "C, X, Y, Z>");

// Now for the fun part!
// You will defined a cartesian product of two lists.
template <class, class>
struct cartesian_product;

// Defines a base case.
// The cartesian product of an empty list and any other list is an empty list
template <class... L>
struct cartesian_product<list<>, list<L...>> {
  using type = list<>;
};

// Now we do the recursive case
// This works by the following algorithm:
// 1) Given a lists A and B
// 2) let a = pop the first element out of A
// 3) let a_product = the list of pairs (a, every element of B)
// 4) let rest_product = cartesian_product of the remainging elements of A and B
// 5) then return a_product joined to rest_product
template <class A, class... ATail, class... B>
struct cartesian_product<list<A, ATail...>, list<B...>> {
  using type = typename join<
      list<pair<A, B>...>,  // Construct a list of pairs (A, every element of B)
      typename cartesian_product<list<ATail...>, list<B...>>::
          type  // Take the cartesian_product of the remainging elements of A
                // will all the elements of B
      >::type;  // Join the two resulting lists
};

static_assert(
    cartesian_product<list<A, B, C, D>, list<W, X, Y, Z>>::type::size == 16,
    "A cartesian_product between two lists of size 4 should have size 16");

static_assert(
    std::is_same<
        typename cartesian_product<list<A, B, C, D>, list<W, X, Y, Z>>::type,
        list<pair<A, W>, pair<A, X>, pair<A, Y>, pair<A, Z>,  // A
             pair<B, W>, pair<B, X>, pair<B, Y>, pair<B, Z>,  // B
             pair<C, W>, pair<C, X>, pair<C, Y>, pair<C, Z>,  // C
             pair<D, W>, pair<D, X>, pair<D, Y>, pair<D, Z>   // D
             >>::value,
    "The cartesian_product of {A, B, C, D} and {W, X, Y, Z} should "
    "be {(A, W), (B, W), (C, W), (D, W), (A, X), (B, X), (C, X), (D, "
    "X), (A, Y), (B, Y), (C, Y), (D, Y)}");

int main() {}
