# 002: Lists at Compile Time

Recommended reading: https://en.wikipedia.org/wiki/Variadic_template#C.2B.2B

* Part 1: [values.cpp](https://github.com/grenewode/grenewodes_programming_puzzlers/blob/master/002/challenges/values.cpp)
  This first part will introduce you to using C++ template parameter packs. You will be using non-type template parameters here. If you are not familiar with these, they let you write templates like the following:
  ```c++
  template<size_t I>
  size_t ident() {
    return I;
  }

  // Prints 17 to stdout
  int main() { std::cout << ident<17>() << std::endl; }
  ```

  For this challenge, you will be using them in variadic templates. For example, you can define a function which takes a sum of integers at compile time with the following:
  ```c++
  template<size_t... I>
  struct sum {
    static constexpr size_t value = 0;
  };

  template<size_t H, size_t... Tail>
  struct sum<H, Tail...> {
    static constexpr size_t value = H + sum<Tail...>::value;
  };
  ```
  **Bonus points if you find the same clang bug I did!**
* Part 2: [types.cpp](https://github.com/grenewode/grenewodes_programming_puzzlers/blob/master/002/challenges/types.cpp)
  For this challenge, you are using lists of types instead of values, and you will be defining a cartesian product instead of implementing reduction.
