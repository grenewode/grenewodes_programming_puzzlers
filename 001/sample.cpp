#include <type_traits>

// This defines a single mapping of a key value to a value type.
template<class K, class V>
struct entry_t {};

// The mapping object
template<class...>
struct map_t {};

template<class... K, class... V>
struct map_t<entry_t<K, V>...> {
    template<class Needle>
    using get = void; // You need to fill this in
};

// Some dummy structs to use as keys
struct A {};
struct B {};

// Now we actually create the map!
using my_map_t = map_t<
	entry_t<A, int>, // A maps to int
	entry_t<B, float> // B maps to float
>;

// Run the checks!
static_assert(std::is_same<my_map_t::get<A>, int>::value, "A should map to int");
static_assert(!std::is_same<my_map_t::get<A>, float>::value, "A should not map to float");
static_assert(!std::is_same<my_map_t::get<A>, A>::value, "A should not map to A");
static_assert(std::is_same<my_map_t::get<B>, float>::value, "B should map to float");
static_assert(!std::is_same<my_map_t::get<B>, int>::value, "B should not map to int");
static_assert(!std::is_same<my_map_t::get<B>, A>::value, "B should not map to A");


int main() {}
