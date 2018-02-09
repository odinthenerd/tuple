#pragma once


#include <kvasir/mpl/mpl.hpp>

//detection thanks to from Simon Brand
#if (defined(_MSC_VER) && _MSC_VER == 1900)
#define KVASIR_TUPLE_MSVC2015
#endif

#if (defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ <= 9)
#define KVASIR_TUPLE_GCC49
#endif

#if (defined(__GNUC__) && __GNUC__ == 5 && __GNUC_MINOR__ <= 4)
#define KVASIR_TUPLE_GCC54
#endif

#if (defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ <= 9)
#define KVASIR_TUPLE_NO_CONSTRR
#endif

#if __cplusplus > 201103L
#define KVASIR_TUPLE_CXX14
#endif

#if (__cplusplus == 201103L || defined(KVASIR_TUPLE_MSVC2015) ||                \
     defined(KVASIR_TUPLE_GCC49)) &&                                            \
    !defined(KVASIR_TUPLE_GCC54)
/// \exclude
#define KVASIR_TUPLE_11_CONSTEXPR
#else
/// \exclude
#define KVASIR_TUPLE_11_CONSTEXPR constexpr
#endif



namespace kvasir {

template <typename...> class tuple;
namespace detail {

	template<typename I, typename T>
	struct ref_element{
		T __e;
		KVASIR_TUPLE_11_CONSTEXPR ref_element(T e):__e{std::forward<T>(e)}{};
		ref_element(const ref_element&) = delete;
	};
	template<typename...Es>
	struct ref_tuple : Es...{
		template<typename...Ts>
		KVASIR_TUPLE_11_CONSTEXPR ref_tuple(Ts&&...args) noexcept :Es{std::forward<Ts>(args)}...{}
		ref_tuple(const ref_tuple&) = delete;
	};

	template<typename I, typename T>
	KVASIR_TUPLE_11_CONSTEXPR T get(ref_element<I,T>&& e){ return std::forward<T>(e.__e); }
	
//non empty case
template <typename I, typename T, typename = mpl::call<mpl::is_empty<>,T>> 
struct element_holder {
public:
	KVASIR_TUPLE_11_CONSTEXPR element_holder() = default;
	template<typename R>
	KVASIR_TUPLE_11_CONSTEXPR element_holder(R&& __r):__d{get<I>(std::move(__r))}{} //TODO
private:
	T __d; 
	friend KVASIR_TUPLE_11_CONSTEXPR const T& get(const element_holder& t) {
		return t.__d;
	}
	friend KVASIR_TUPLE_11_CONSTEXPR T& get(element_holder& t) {
		return t.__d;
	}
	friend KVASIR_TUPLE_11_CONSTEXPR T&& get(element_holder&& t) {
		return t.__d;
	}
	//TODO const r-value case?
};

//empty case
template<typename I, typename T>
struct element_holder<I,T,mpl::true_>{
	KVASIR_TUPLE_11_CONSTEXPR element_holder()  = default;
	template<typename R>
	KVASIR_TUPLE_11_CONSTEXPR element_holder(R&&){}  //no need to do anything
	friend KVASIR_TUPLE_11_CONSTEXPR T get(const element_holder&) { return T{}; }
};

template <typename... Es> 
struct tuple_base : Es... {
  KVASIR_TUPLE_11_CONSTEXPR tuple_base() = default;
  template<typename R>
  KVASIR_TUPLE_11_CONSTEXPR tuple_base(R&& __r):Es(std::move(__r))...{}
  KVASIR_TUPLE_11_CONSTEXPR typename std::enable_if<mpl::call<mpl::all<mpl::unpack<mpl::at1<mpl::is_swappable<>>>>,Es...>::value>::type 
    swap(tuple_base& y) noexcept(mpl::call<mpl::all<mpl::unpack<mpl::at1<mpl::is_nothrow_swappable<>>>>,Es...>::value) {
	using std::swap;
    	int a[]{0,(swap(get(static_cast<Es&>(*this)),get(static_cast<Es&>(y))),0)...};
	(void)a;
  }
};
namespace {
using namespace mpl;
template <typename... Ts>
using make_base = call<
  zip_with_index<                   	// binary transform index and input list
    cfe<element_holder>,     		// into a pack of element_holders
      sort<                      	// sort the remaining elements
        transform<             		// extracting from each
          unpack<at1<alignment_of<>>>, 	// the allignment of the type parameter
        greater_than<> 			// and sorting in descending order of allignment
      >,
      cfe<tuple_base> 			// wrap everything in a tuple_base when done
    >>,
    Ts... 				// input lists are the input types
  >; 					// and a corresponding index of each
template <typename... Ts>
using make_ref = call<
  zip_with_index<                   	// binary transform index and input list
    cfe<ref_element>,     		// into a pack of ref_elements
    cfe<ref_tuple> 			// wrap everything in a ref_tuple when done
    >,
    Ts... 				// input lists are the input types
  >; 					// and a corresponding index of each


template <std::size_t I, typename T>
using tuple_element = call<unpack<at<uint_<I>,cfe<type_>>>, T>;

} // namespace
} // namespace detail

template <std::size_t I, typename T> 
using tuple_element = detail::tuple_element<I, T>;

template <std::size_t I, typename T>
using tuple_element_t = typename tuple_element<I, T>::type;

namespace detail{
	using namespace kvasir;
	using namespace mpl;
	template<std::size_t I, typename T>
	using return_of_get = std::enable_if<is_empty
}
template <typename... Es> 
class tuple : detail::make_base<Es...> {
  using __base_t = detail::make_base<Es...>;
  template <std::size_t I, typename... Ts>
  friend KVASIR_TUPLE_11_CONSTEXPR tuple_element_t<I, tuple<Ts...>>&
  get(tuple<Ts...>& __t) noexcept;

  template <std::size_t I, typename... Ts>
  friend KVASIR_TUPLE_11_CONSTEXPR tuple_element_t<I, tuple<Ts...>>&&
  get(tuple<Ts...>&& __t) noexcept;

  template <std::size_t I, typename... Ts>
  friend KVASIR_TUPLE_11_CONSTEXPR const tuple_element_t<I, tuple<Ts...>>&
  get(const tuple<Ts...>& __t) noexcept;

  template <std::size_t I, typename... Ts>
  friend KVASIR_TUPLE_11_CONSTEXPR const tuple_element_t<I, tuple<Ts...>>&&
  get(const tuple<Ts...>&& __t) noexcept;

  template <typename I, typename... Ts>
  friend KVASIR_TUPLE_11_CONSTEXPR I&
  get(tuple<Ts...>& __t) noexcept;

  template <typename I, typename... Ts>
  friend KVASIR_TUPLE_11_CONSTEXPR I&&
  get(tuple<Ts...>&& __t) noexcept;

  template <typename I, typename... Ts>
  friend KVASIR_TUPLE_11_CONSTEXPR const I&
  get(const tuple<Ts...>& __t) noexcept;

  template <typename I, typename... Ts>
  friend KVASIR_TUPLE_11_CONSTEXPR const I&&
  get(const tuple<Ts...>&& __t) noexcept;

  using __is_default_constructible = mpl::call<mpl::all<mpl::is_default_constructible<>>,Es...>; //used multiple times, should be memoized here
public:
  typename std::enable_if<mpl::call<mpl::is_swappable<>,__base_t>::value>::type swap(tuple& __t) noexcept(noexcept(std::declval<__base_t>().swap(__t))){
    __base_t::swap(__t);
  }
  template<typename _D = mpl::true_, typename = typename std::enable_if<(__is_default_constructible::value == _D::value)>::type>
  KVASIR_TUPLE_11_CONSTEXPR tuple() noexcept(mpl::call<mpl::all<mpl::is_nothrow_default_constructible<>>,Es...>::value):__base_t{} {}
  KVASIR_TUPLE_11_CONSTEXPR tuple(tuple const&) = default;
  KVASIR_TUPLE_11_CONSTEXPR tuple(tuple&&) = default;
  template<typename _AllocTag, typename _Alloc, typename _D = mpl::true_, 
    typename = typename std::enable_if<
        mpl::call<mpl::if_<mpl::front<mpl::same_as<std::allocator_arg_t>>,mpl::at1<mpl::same_as<mpl::true_>>,mpl::always<mpl::false_>>,_AllocTag,_D>::value
    >::type>
  tuple(_AllocTag, _Alloc __a){} //TODO  
  template<typename...Ts>
  KVASIR_TUPLE_11_CONSTEXPR tuple(Ts&&...__as):__base_t(detail::make_ref<Ts...>(std::forward<Ts>(__as)...)){}
};


template <std::size_t I, typename... Ts>
inline KVASIR_TUPLE_11_CONSTEXPR tuple_element_t<I, tuple<Ts...>>& get(tuple<Ts...>& __t) noexcept {
  using __e = tuple_element_t<I, tuple<Ts...>>;
  using __h = detail::element_holder<mpl::uint_<I>, __e>;
  return get(static_cast<__h&>(__t));
}

template <std::size_t I, typename... Ts>
inline KVASIR_TUPLE_11_CONSTEXPR const tuple_element_t<I, tuple<Ts...>>& get(tuple<Ts...>& __t) noexcept {
  using __e = tuple_element_t<I, tuple<Ts...>>;
  using __h = detail::element_holder<mpl::uint_<I>, __e>;
  return get(static_cast<const __h&>(__t));
}

template <std::size_t I, typename... Ts>
inline KVASIR_TUPLE_11_CONSTEXPR tuple_element_t<I, tuple<Ts...>>&& get(tuple<Ts...>&& __t) noexcept {
  using __e = tuple_element_t<I, tuple<Ts...>>;
  using __h = detail::element_holder<mpl::uint_<I>, __e>;
  return get(static_cast<__h&&>(__t));
}

template <std::size_t I, typename... Ts>
inline KVASIR_TUPLE_11_CONSTEXPR const tuple_element_t<I, tuple<Ts...>>&& get(const tuple<Ts...>&& __t) noexcept {
  using __e = tuple_element_t<I, tuple<Ts...>>;
  using __h = detail::element_holder<mpl::uint_<I>, __e>;
  return get(static_cast<const __h&&>(__t));
}


template <typename E, typename... Ts>
inline KVASIR_TUPLE_11_CONSTEXPR E& get(tuple<Ts...>& __t) noexcept {
  using __i = mpl::call<mpl::index_if<mpl::same_as<E>, mpl::front<>>, Ts...>;
  using __h = detail::element_holder<__i, E>;
  return get(static_cast<__h&>(__t));
}

template <typename E, typename... Ts>
inline KVASIR_TUPLE_11_CONSTEXPR const E& get(const tuple<Ts...>& __t) noexcept {
  using __i = mpl::call<mpl::index_if<mpl::same_as<E>, mpl::front<>>, Ts...>;
  using __h = detail::element_holder<__i, E>;
  return get(static_cast<const __h&>(__t));
}

template <typename E, typename... Ts>
inline KVASIR_TUPLE_11_CONSTEXPR E&& get(tuple<Ts...>&& __t) noexcept {
  using __i = mpl::call<mpl::index_if<mpl::same_as<E>, mpl::front<>>, Ts...>;
  using __h = detail::element_holder<__i, E>;
  return get(static_cast<__h&&>(__t));
}

template <typename E, typename... Ts>
inline KVASIR_TUPLE_11_CONSTEXPR const E&& get(const tuple<Ts...>&& __t) noexcept {
  using __i = mpl::call<mpl::index_if<mpl::same_as<E>, mpl::front<>>, Ts...>;
  using __h = detail::element_holder<__i, E>;
  return get(static_cast<const __h&&>(__t));
}

template<typename...Ts>
void swap(tuple<Ts...>& x, tuple<Ts...>& y) noexcept(noexcept(x.swap(y))){
  x.swap(y);
}

} // namespace kvasir
using t = kvasir::detail::make_base<int, long, kvasir::mpl::int_<2>, bool, int>;
