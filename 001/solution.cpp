#include <type_traits>
#include <tuple>

// This struct is the important part. It is a helper which will let you find the index of a type in a tuple
template<typename, typename, std::size_t = 0>
struct tuple_find;


// Mached specialization
// This specialization is infered when Needle is the same type as the first element of the tuple we are searching.
// If the C++ compiler can use this specialization, it will, since it always preferes the most specific specialization
// when resolving the specializations.
template<typename Needle, typename... Haystack, std::size_t I>
struct tuple_find<Needle, std::tuple<Needle, Haystack...>, I> : 
    // std::integral_constant is basically just a wrapper for a number.
    // In this case, since we assume that I is the index of the first element in 
    // the tuple currently testing, we can simply "return" it, since Needle and 
    // the start of tuple are identical
    std::integral_constant<std::size_t, I> {};

// Unmached specialization
// This specialization is infered when Needle and HaystackStart do not match.
// It is chosen after the matching specialization because it is less specific, 
// since HaystackStart could be anything.
template<typename Needle, typename HaystackStart, typename... Haystack, std::size_t I>
struct tuple_find<Needle, std::tuple<HaystackStart, Haystack...>, I> :
    // Since the Needle and the first element of the tuple do *not* match in this 
    // case, we remove the first element of the tuple, and then do a new search 
    // into the resulting smaller tuple, and update the index accordingly.
    tuple_find<Needle, // The needle stays the same regardless
               std::tuple<Haystack...>, I + 1> {};

// This specialization will only be used when Haystak is EMPTY.
// This is because, again, C++ preferes to use the most specific it can. In most cases, 
// this means that one of the above specializations will work better.
// However, if the tuple has no elements, then neither of the above specializations 
// will work, and this one will be selected, since parameter packs can contain zero 
// or more elements.  
template<typename Needle, typename... Haystack, std::size_t I>
struct tuple_find<Needle, std::tuple<Haystack...>, I> {
    // why can't this be static_assert(false, ...)? It turns out that the C++ compiler will
    // ALWAYS fail on that, even if the code is never actually used.
    static_assert(sizeof...(Haystack) == 0, "tuple does not contain the requested type");
};

// This defines a single mapping of a key value to a value type.
template<class K, class V>
struct entry_t {};

// The mapping object
template<class...>
struct map_t {};

template<class... K, class... V>
struct map_t<entry_t<K, V>...> {
private:
    // Construct a tuple of the key types 
    using keys_type = std::tuple<K...>;
    // ... And a tuple of the value types
    using values_type = std::tuple<V...>;
    
    // Notice that both these tuples have exactly the same size, and they are arranged such that
    // if a key type K has an associated type V, then the index K in keys_type will be the same 
    // as the index of V in values_type 
public:
    template<class Needle>
    using get = std::tuple_element_t< // This lets us get they type a tuple contains that the given index
        tuple_find<Needle, keys_type>::value, // Use our tuple_find helper to search for the index of the key we want
        values_type>; // Then use that index to find the type at that index in the values_tuple
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
