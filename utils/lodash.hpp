#ifndef UNDERSCORE_UNDERSCORE_H_
#define UNDERSCORE_UNDERSCORE_H_
#define UNDERSCORE_BONUS
#define _VECTOR(...) <std::vector<__VA_ARGS__>>
#define _IDENTITY_LAMBDA [](const auto &_) { return _; }

#include <cstdlib>
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <vector>
#include <utility>
// #include <functional.h>

// using namespace sfinktah::functional;

////  not used at present, but a really handy example of dealing with multiple argument passing and rvalue and perfect forwarding
////  (from Scott Meyers, in his "Effective Modern C++")
// template <typename Function, typename... Args>
// inline auto reallyAsync(Function&& f, Args&&... params) {
//     // Maybe we could use it with std::apply - see http://en.cppreference.com/w/cpp/utility/apply
//     // and http://en.cppreference.com/w/cpp/utility/functional/invoke
//     return std::async(std::launch::async, std::forward<Function>(f),
//         std::forward<Args>(params)...);
// }
//

namespace _
{
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

    namespace helper
    {

        // For a number of Underscore functions, the elements of a container are
        // transformed in some way, and the results are placed in another container.
        // To be able to support different kinds of containers, a way of choosing the
        // proper method for addition to the result container must be called, but these
        // methods are not uniform across the standard library.

        // To get around this, the correct function to call must be determined at
        // compile time using metafunctions and SFINAE.

        // Because the body to determine whether or not a given member function is
        // relatively large, the HAS_MEMBER_FUNCTION macro is temporarily defined to
        // help reduce code size.
        // This is from http://stackoverflow.com/a/264088/1256
#define HAS_MEMBER_FUNCTION(func, name)                             \
    template <typename T, typename Sign>                            \
    struct name                                                     \
    {                                                               \
        typedef char yes[1];                                        \
        typedef char no[2];                                         \
        template <typename U, U>                                    \
        struct type_check;                                          \
        template <typename _1>                                      \
        static yes &chk(type_check<Sign, &_1::func> *);             \
        template <typename>                                         \
        static no &chk(...);                                        \
        static bool const value = sizeof(chk<T>(0)) == sizeof(yes); \
    }

        // Use the macro to define metafunctions for the various insertion methods that
        // Underscore supports. Primarily, these will be single parameter member
        // functions that are used across multiple types in the standard library.
        HAS_MEMBER_FUNCTION(push_back, HasPushBack);
        HAS_MEMBER_FUNCTION(insert, HasInsert);
        HAS_MEMBER_FUNCTION(key, HasKey);
        HAS_MEMBER_FUNCTION(is_object, HasIsObject);
        HAS_MEMBER_FUNCTION(to_cbor, HasToCbor); // json iterator

        // Remove the macro so that it doesn't pollute the global scope.
#undef HAS_MEMBER_FUNCTION

        // To simplify function declarations later, the insertion capabilities for a
        // given type are simply listed in a struct.
        template <typename Container>
        struct ContainerMethods
        {
            static bool const has_push_back =
                HasPushBack<
                    Container,
                    void (Container::*)(const typename Container::value_type &)>::value;
            static bool const has_insert =
                HasInsert<
                    Container,
                    std::pair<typename Container::iterator, bool> (Container::*)(const typename Container::value_type &)>::value;
            static bool const has_is_object =
                HasIsObject<
                    Container,
                    std::pair<typename Container::iterator, bool> (Container::*)(const typename Container::value_type &)>::value;
            static bool const has_to_cbor =
                HasToCbor<
                    Container,
                    void (Container::*)(const typename Container::value_type &)>::value;
        };

        // template <typename Iterator>
        // struct IteratorMethods
        //{
        //     static bool const has_key =
        //         HasKey<
        //             Iterator,
        //             void (Iterator::*)
        //                 (const typename Iterator::value_type&)
        //         >::value;
        // };

        template <typename Container>
        struct HasSupportedMethod
        {
            static bool const value = 0 || ContainerMethods<Container>::has_push_back || ContainerMethods<Container>::has_insert;
        };

        template <typename Container>
        struct HasJsonTraits
        {
            static bool const value = 0 || ContainerMethods<Container>::has_is_object || ContainerMethods<Container>::has_to_cbor;
        };

        // A simple implementation of enable_if allows alternative functions to be
        // selected at compile time.
        // This is from http://stackoverflow.com/a/264088/1256

        template <bool C, typename T = void>
        struct enable_if
        {
            typedef T type;
        };

        template <typename T>
        struct enable_if<false, T>
        {
        };

        // This `enable_if_t` syntactic sugar is really not worth the one-time use
        template <bool B, typename T = void>
        using enable_if_t = typename std::enable_if<B, T>::type;

        template <typename Container>
        enable_if_t<ContainerMethods<Container>::has_insert, void>
        insert(Container &container, typename Container::value_type const &value)
        {
            container.insert(value);
        }

        template <typename Container>
        typename enable_if<ContainerMethods<Container>::has_push_back, void>::type

        push_back(Container &container, typename Container::value_type const &value)
        {
            container.push_back(value);
        }

        template <typename Container>
        typename enable_if<!ContainerMethods<Container>::has_push_back, void>::type
        push_back(Container &container, typename Container::value_type const &value)
        {
            insert(container, value);
        }

        template <typename Container>
        typename enable_if<HasSupportedMethod<Container>::value, void>::type
        add_to_container(Container &container, typename Container::value_type const &value)
        {
            push_back(container, value);
        }

        template <typename Container>
        typename enable_if<HasSupportedMethod<Container>::value, void>::type
        add_to_container(Container &container, typename Container::key_type const &key, typename Container::value_type const &value)
        {
            container.insert(container, value);
        }

        template <typename T>
        struct is_void
        {
            static bool const value = false;
        };

        template <>
        struct is_void<void>
        {
            static bool const value = true;
        };

        template <typename T>
        constexpr T clamp(T value, T min, T max)
        {
            return (
                value > max ? max : value < min ? min
                                                : value);
        }

        // https://stackoverflow.com/questions/9044866/how-to-get-the-number-of-arguments-of-stdfunction/9044927#9044927
        template <typename T>
        struct count_arg;

        template <typename R, typename... Args>
        struct count_arg<std::function<R(Args...)>>
        {
            static const size_t value = sizeof...(Args);
        };

        // #include <type_traits>
        // #include <utility>
        // #include <map>

        // https://stackoverflow.com/questions/43992510/enable-if-to-check-if-value-type-of-iterator-is-a-pair/43993493#43993493
        template <typename>
        struct is_pair : std::false_type
        {
        };

        template <typename T, typename U>
        struct is_pair<std::pair<T, U>> : std::true_type
        {
        };

        // int main()
        //{
        //     std::map<int, int> foo{
        //         { 1, 2 },
        //         { 3, 4 },
        //     };

        //    do_stuff(foo.begin());
        //    return 0;
        //}
    } // namespace helper

    // Collections

    // each/for_each

    // http://en.cppreference.com/w/cpp/algorithm/for_each
    template <typename Container, typename Function>
    void each(Container container, Function function)
    {
        // function - function object, to be applied to the result of dereferencing every iterator in the range[first, last)
        // The signature of the function should be equivalent to the following :

        //    void fun(const Type &a);

        // The signature does not need to have `const &`.

        // The type Type must be such that an object of type InputIt can be dereferenced and then implicitly converted to Type.

        std::for_each(container.begin(), container.end(), function);
    }

    template <typename Container, typename Function>
    void each_with_distance(Container container, Function function)
    {
        // The signature of the function should be equivalent to the following :

        //    void fun(const Type &a, const size_t d);
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            function(*i, std::distance(container.begin(), i));
        }
    }

    template <typename Container, typename Function>
    void each_iter(Container container, Function function)
    {
        // The signature of the function should be equivalent to the following :

        //    void fun(const Type &a, const size_t d);
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            function(i);
        }
    }

    // template <class T, class = void>
    // struct is_iterator : std::false_type { };

    // template <class T>
    // struct is_iterator<T, std::void_t<
    //     typename std::iterator_traits<T>::iterator_category
    //     >> : std::true_type { };

    // template <typename T, typename Function>
    // typename std::enable_if<is_iterator<T>::value, void>::type each_key_value(T iterator, Function function)
    //{
    //     for (; iterator != container.end(); ++i) {
    //         auto key = i->first;
    //         auto value = i->second;
    //         function(value, key, container);
    //     }
    // }

    //  The full power of `each`.  Each invocation of iteratee is called
    //  with three arguments: (element, index, list). If list is an object,
    //  iteratee's arguments will be (value, key, list).  (MDN)
    template <typename Container, typename Function>
    void each_key_value(Container container, Function function)
    {
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            auto key = i->first;
            auto value = i->second;
            function(value, key, container);
        }
    }

    template <typename Iterator, typename Function>
    void each_key_value(Iterator i, Iterator end, Function function)
    {
        for (; i != end; ++i)
        {
            auto key = i->first;
            auto value = i->second;
            function(value, key);
        }
    }

    //  each - for nlohmann::json associative containers. iteratee has three arguments: (value, key, container).
    template <typename Container, typename Function>
    void each_json(Container &container, Function &function)
    {
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            auto key = i.key();
            auto value = i.value();
            function(value, key);
        }
    }

    // template <typename Container, typename... Args>
    // void each_json_magic(Container& container, std::function<void(Args...)> function)
    //{
    //     auto iteratee = [](auto value, auto key, auto& container) {};
    //     switch (helper::count_arg<function>::value) {
    //         case 3: iteratee = [&] { function(value, key, container); }; break;
    //         case 2: iteratee = [&] { function(value, key); }; break;
    //         case 1: iteratee = [&] { function(value); }; break;
    //     }

    //    for (auto i = container.begin(); i != container.end(); ++i) {
    //        auto key = i.key();
    //        auto value = i.value();
    //        iteratee(value, key, container);
    //    }
    //}

    template <typename Container, typename Function>
    void for_each(Container container, Function function)
    {
        each(container, function);
    }

    // map/collect
    template <typename ResultContainer, typename Container, typename Function>
    ResultContainer map(const Container &container, Function function)
    {
        ResultContainer result;

        for (auto &item : container)
            helper::add_to_container(result, function(item));
        // for (auto i = container.begin(); i != container.end(); ++i)
        //{
        //     helper::add_to_container(result, function(*i));
        // }
        return result;
    }

    // map/collect with raw iterators -- sfink
    template <typename ResultContainer, typename Container, typename Function>
    ResultContainer map_iter(Container container, Function function)
    {
        ResultContainer result;
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            helper::add_to_container(result, function(i));
        }
        return result;
    }

    // mapObject - Creates an array of values by running each element in collection thru iteratee.
    // The iteratee is invoked with two arguments: (value, key). -- sfink
    template <typename ResultContainer, typename Container, typename Function>
    ResultContainer mapObject(const Container &container, Function function)
    {
        ResultContainer result;
        auto keys = keys(container);
        for (const auto &key : keys)
            helper::add_to_container(result, function(container[key], key));

        return result;
    }

    // remove (lodash) - Removes all elements from array that predicate returns truthy for and returns an array of the removed elements.
    // Note: Unlike _.filter, this method mutates array. Use _.pull to pull elements from an array by value.
    // Note: for ease of use, this function does not return removed elements, use `removeAndReturn` instead
    template <typename Container, typename Function>
    void remove(Container &container, Function function)
    {
        for (auto i = container.begin(); i != container.end();)
            function(*i) ? i = container.erase(i) : ++i;
    }

    // remove (lodash) - Removes all elements from array that predicate returns truthy for and returns an array of the removed elements.
    // Note: Unlike _.filter, this method mutates array. Use _.pull to pull elements from an array by value.
    template <typename ResultContainer, typename Container, typename Function>
    ResultContainer removeAndReturn(Container &container, Function function)
    {
        ResultContainer result;
        for (auto i = container.begin(); i != container.end();)
        {
            if (function(*i))
                helper::add_to_container(result, *i),
                    i = container.erase(i);
            else
                ++i;
        }
        return result;
    }

    // pull (lodash) - Removes all given values from array using SameValueZero for equality comparisons.
    // Note: Unlike `without`, this method mutates array. Use _.remove to remove elements from an array by predicate.
    template <typename Container>
    void pull(Container &container, typename Container::value_type const &value)
    {
        for (auto i = container.begin(); i != container.end();)
            (*i == value) ? i = container.erase(i) : ++i;
    }

    // pullAll (lodash) - This method is like _.pull except that it accepts an array of values to remove.
    // Note: Unlike _.difference, this method mutates array.
    template <typename Container1, typename Container2>
    void pullAll(Container1 &container, Container2 const &values)
    {

        // Hmmm.... if it's similar to difference, maybe we could leverage the existing `difference` function...
        // However, that function looks complicated. Lets leverage `contains` instead. It's possibly less
        // efficient that using `difference` but simplicity wins today.
        for (auto i = container.begin(); i != container.end();)
            contains(values, *i) ? i = container.erase(i) : ++i;
    }

    // filter/select
    template <typename ResultContainer, typename Container, typename Predicate>
    ResultContainer filter(Container container, Predicate predicate)
    {
        ResultContainer result;
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            if (predicate(*i))
            {
                helper::add_to_container(result, *i);
            }
        }
        return result;
    }

    // without - Creates an array excluding all given values using SameValueZero for equality comparisons.
    // Note: Unlike `pull`, this method returns a new array.
    template <typename ResultContainer, typename Container>
    ResultContainer without(Container const &container, typename Container::value_type const &value)
    {
        // sorry, you'll have to work out your own checks for C++17
        return filter<ResultContainer>(container, [value](const auto &_)
                                       {
                                           return value != _;
                                           // return std::not_equal_to<typename Container::value_type>(_, value);
                                       });
#if 0
		// deprecated in c++11, removed in c++17
        return filter<ResultContainer>(
            container, std::bind2nd(std::not_equal_to<typename Container::value_type>(), value));
#endif
    }

    template <typename ResultContainer, typename Container, typename Function>
    ResultContainer collect(Container container, Function function)
    {
        return map<ResultContainer>(container, function);
    }

    // sfink - values
    template <typename ResultContainer, typename Container>
    ResultContainer values(Container container)
    {
        ResultContainer result;
        // zorg c++11 optimisation
        for (const auto &item : container)
            helper::add_to_container(result, item);
        // previously: (and still required for associative containers, e.g. map

        // for (auto i = container.begin(); i != container.end(); ++i)
        // {
        //    helper::add_to_container(result, *i);
        // }
        return result;
    }

    // template <class T, class Enable = void>
    // class value_type_from
    //{
    //     typedef T type;
    // };

    // template <class T>
    // class value_type_from<T, typename enable_if_has_type<typename T::value_type>::type>
    //{
    //     typedef typename T::value_type type;
    // };
    // typename Container::iterator

    template <typename ResultContainer, typename Container>
    ResultContainer valuesObject(Container container)
    {
        ResultContainer result;
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            auto value = i->second;
            helper::add_to_container(result, value);
        }
        return result;
    }

    // sfink - keys
    template <typename ResultContainer, typename Container>
    ResultContainer keys(Container container)
    {
        ResultContainer result;
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            auto k = i->first;
            helper::add_to_container(result, k);
        }
        // for (std::pair<std::string, std::string> i : container) {
        //     helper::add_to_container(result, i.first);
        // }
        return result;
    }

    template <typename ResultContainer, typename Container>
    ResultContainer pairs(Container container)
    {
        ResultContainer result;
        for (std::pair<std::string, std::string> i : container)
        {
            helper::add_to_container(result, i.second);
        }
        return result;
    }

    //  template <typename ResultContainer, typename Container, typename Function>
    //  typename helper::enable_if<!helper::is_void<ResultContainer>::value, ResultContainer>::type
    // invoke(Container container, Function function)
    //  {

    // sfink - keys_json
    // template <typename ResultContainer, typename Container, typename Iterator Container::const_iterator>
    template <typename ResultContainer, typename Container>
    typename helper::enable_if<helper::HasJsonTraits<Container>::value, ResultContainer>::type
    keys_json(Container container)
    {
        // json j;
        // j.
        ResultContainer result;
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            helper::add_to_container(result, i.key());
        }
        for (std::pair<std::string, std::string> i : container)
        {
            helper::add_to_container(result, i.first);
        }

        return result;
    }

    // sfink - keys2
    template <typename Container>
    auto keys2(const Container &container)
    {
        return keys<std::vector<typename Container::key_type>>(container);
        // typename Container::key_type result;
        // for (auto i = container.begin(); i != container.end(); ++i)
        //{
        //     auto k = i->first;
        //     helper::add_to_container(result, k);
        // }
        // return result;
    }

    // template<class Iterator, typename = typename std::enable_if<is_pair<typename Iterator::value_type>::value, Iterator>::type>
    // decltype(auto) do_stuff(Iterator&& iterator) {

    //    //access of iterator->second ok.
    //}

    template <typename T>
    class HasJsonPointer
    {
    private:
        typedef char yes[1];
        typedef char no[2];
        template <typename C>
        static yes &test(typename C::json_pointer *);
        template <typename C>
        static no &test(...);

    public:
        static bool const value = sizeof(test<T>(0)) == sizeof(yes);
    };

    template <typename ResultContainer, typename Container>
    typename helper::enable_if<!HasJsonPointer<typename Container::value_type>::value, void>::type
    keys_internal(ResultContainer &result, Container const &container)
    {
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            helper::add_to_container(result, *i);
        }
    }

    template <typename ResultContainer, typename Container>
    typename helper::enable_if<HasJsonPointer<typename Container::value_type>::value, void>::type
    keys_internal(ResultContainer &result, Container const &container)
    {
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            helper::add_to_container(result, *i);
        }
    }

    // MDN - The slice() method returns a shallow copy of a portion of an array into a
    // new array object selected from begin to end (end not included).
    // The original array will not be modified.
    template <typename ResultContainer, typename Container>
    ResultContainer slice(Container container, long long begin = 0, long long end = 0)
    {
        // begin Optional
        //     Zero - based index at which to begin extraction.
        //     A negative index can be used, indicating an offset from the end of the sequence.slice(-2) extracts the last two elements in the sequence.
        //     If begin is undefined, slice begins from index 0.

        // end Optional
        //     Zero - based index before which to end extraction.slice extracts up to but not including end.
        //     For example, slice(1, 4) extracts the second element through the fourth element(elements indexed 1, 2, and 3).
        //     A negative index can be used, indicating an offset from the end of the sequence.slice(2, -1) extracts the third element through the second - to - last element in the sequence.
        //     If end is omitted, slice extracts through the end of the sequence(arr.length).
        //     If end is greater than the length of the sequence, slice extracts through the end of the sequence(arr.length).

        const size_t len = container.size();
        if (end < 1)
            end = len + end;

        if (begin < 0)
            begin = len + begin + 1;

        begin = helper::clamp<long long>(begin, 0, len - 1);
        end = helper::clamp<long long>(end, 0, len);

        ResultContainer result;
        long long _index = 0;
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            auto index = _index++;
            if (index >= end)
                break;
            if (index >= begin)
                helper::add_to_container(result, *i);
        }
        return result;
    }

    // Output:
    //     Index: 0 = 1
    //     Index : 1 = 2
    //     Index : 2 = 3

    // reduce/inject/foldl
    /// <summary>Applies a function against an accumulator and each element in the container (from left to right) to reduce it to a single value.</summary>
    /// <param name="container">The container.</param>
    /// <param name="function">The callback, callback(<paramref name="initialValue" />, currentValue, currentIndex, container)</param>
    /// <param name="initialValue">Value to use as the first argument to the first call of the callback.</param>
    /// <returns>The value that results from the reduction.</returns>
    /// <example><code><![CDATA[
    /// using fspath = std::experimental::filesystem::path;
    /// template<typename Container>
    /// std::string pathCombine(Container paths) {
    ///     fspath path = _::reduce(Container, [](fspath _path, std::string segment) {
    ///         return _path /= filepath(segment);
    ///     }, fspath);
    ///     return path.string();
    /// }]]></code></example>
    /// TODO Implement initialValue as optional: "[Optional] Value to use as the first argument to the first call of the callback. If no initial value is supplied, the first element in the array will be used. Calling reduce on an empty array without an initial value is an error."
    /// TODO Implement full range of functionality as described in https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/Reduce?v=b
    template <typename Container, typename Function, typename Memo>
    Memo reduce(const Container &container, Function function, Memo initialValue)
    {
        each(container, [&](const auto &i)
             { initialValue = function(initialValue, i); });
        // for (auto i = container.begin(); i != container.end(); ++i)
        //{
        //     initialValue = function(initialValue, *i);
        // }
        return initialValue;
    }

    /// <summary>`reduce` for sequence containers with 4 argument callback</summary>
    /// <param name="container">The container.</param>
    /// <param name="function">callback(<paramref name="initialValue" />, currentValue, currentIndex, <paramref name="container" />)</param>
    /// <param name="initialValue">Value to use as the first argument to the first call of the callback.</param>
    /// <returns>The value that results from the reduction.</returns>
    /// <example><code>
    /// std::vector<int> v{ 1, 2, 3 };
    /// count << _::reduceArray(v, [](auto accumulator, auto currentValue, auto currentIndex, auto container) {
    ///     return accumulator + "Index: "s + std::to_string(currentIndex) + " = "s + std::to_string(currentValue) + '\n';
    /// }, std::string{})
    /// </example></code>
    /// TODO Implement initialValue as optional: "[Optional] Value to use as the first argument to the first call of the callback. If no initial value is supplied, the first element in the array will be used. Calling reduce on an empty array without an initial value is an error."
    template <typename Container, typename Function, typename Memo>
    Memo reduceArray(const Container &container, Function function, Memo initialValue)
    {
        each_with_distance(container, [&](const typename Container::value_type &value, const size_t index)
                           { initialValue = function(initialValue, value, index, container); });
        return initialValue;
    }

    /// <summary>`reduce` for associative containers with 4 argument callback</summary>
    /// <see cref="reduce" />
    /// <seealso cref="reduceArray" />
    /// <param name="container">The container.</param>
    /// <param name="function">callback(<paramref name="initialValue" />, currentValue, currentKey, <paramref name="container" />)</param>
    /// <param name="initialValue">Value to use as the first argument to the first call of the callback.</param>
    /// <returns>The value that results from the reduction.</returns>
    /// TODO Implement initialValue as optional: "[Optional] Value to use as the first argument to the first call of the callback. If no initial value is supplied, the first element in the array will be used. Calling reduce on an empty array without an initial value is an error."
    template <typename Container, typename Function, typename Memo>
    Memo reduceObject(const Container &container, Function function, Memo initialValue)
    {
        // ResultContainer result;
        auto keys = _::keys2(container);
        for (const auto &key : keys)
        {
            // const auto& value = container.at(key);
            auto value = container.at(key);
            initialValue = function(initialValue, value, key, container);
        }
        return initialValue;
    }

    template <typename Container, typename Function, typename Memo>
    Memo inject(const Container &container, Function function, Memo initialValue)
    {
        return reduce(container, function, initialValue);
    }

    template <typename Container, typename Function, typename Memo>
    Memo foldl(const Container &container, Function function, Memo initialValue)
    {
        return reduce(container, function, initialValue);
    }

    // reduce_right/foldr
    template <typename Container, typename Function, typename Memo>
    Memo reduce_right(const Container &container, Function function, Memo initialValue)
    {
        for (typename Container::const_reverse_iterator i = container.rbegin(); i != container.rend();
             ++i)
        {
            initialValue = function(initialValue, *i);
        }
        return initialValue;
    }

    /// <summary>A copy of std::find_if</summary>
    /// <param name="first">Iterator first.</param>
    /// <param name="last">Iterator last.</param>
    /// <param name="predicate">predicate</param>
    /// <returns></returns>
    template <class InputIterator, class UnaryPredicate>
    InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate predicate)
    {
        while (first != last)
        {
            if (predicate(*first))
                return first;
            ++first;
        }
        return last;
    }

    template <typename Container, typename Function, typename Memo>
    Memo foldr(const Container &container, Function function, Memo initialValue)
    {
        return reduce_right(container, function, initialValue);
    }

    // find/detect
    /// <summary>Iterates over elements of collection, returning the first element predicate returns truthy for. The predicate is invoked with one argument: (value).</summary>
    /// <param name="container">The container.</param>
    /// <param name="predicate">The predicate.</param>
    /// <returns></returns>
    /// <remarks>This doesn't translate well into C++, as it should (by JavaScript underscore standards) return an actual element, or <c>undefined</c>.  While we could simulate <c>undefined</c> with C++17 <c>std::optional</c> usage would not be more convenient that returning an iterator.
    template <typename Container, typename Predicate>
    typename Container::iterator find(Container &container, Predicate predicate)
    {
        return _::find_if(container.begin(), container.end(), predicate);
    }

    /// <summary>Iterates over elements of an associate collection, returning the first element predicate returns truthy for. The predicate is invoked with three arguments: (value, index|key, collection).</summary>
    /// <param name="container">The container.</param>
    /// <param name="predicate">The predicate (value, key, collection)</param>
    /// <returns>The key of the first object found, or {}</returns>
    template <typename Container, typename Function, typename Memo>
    Memo findObject(const Container &container, Function predicate)
    {
        // ResultContainer result;
        auto keys = _::keys2(container);
        for (const auto &key : keys)
        {
            // const auto& value = container.at(key);
            auto value = container.at(key);
            auto found = function(value, key, container);
            if (found)
                return key;
        }
        return {};
    }

    template <typename ResultContainer, typename Container, typename Predicate>
    ResultContainer select(Container container, Predicate predicate)
    {
        return filter<ResultContainer>(container, predicate);
    }

    // reject
    template <typename ResultContainer, typename Container, typename Predicate>
    ResultContainer reject(Container container, Predicate predicate)
    {
        ResultContainer result;
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            if (!predicate(*i))
            {
                helper::add_to_container(result, *i);
            }
        }
        return result;
    }

    // all/every
    template <typename Container, typename Predicate>
    bool all(Container container, Predicate predicate)
    {
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            if (!predicate(*i))
            {
                return false;
            }
        }
        return true;
    }

    template <typename Container, typename Predicate>
    bool every(Container container, Predicate predicate)
    {
        return all(container, predicate);
    }

    // any/some
    template <typename Container, typename Predicate>
    bool any(Container container, Predicate predicate)
    {
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            if (predicate(*i))
            {
                return true;
            }
        }
        return false;
    }

    template <typename Container, typename Predicate>
    bool some(Container container, Predicate predicate)
    {
        return any(container, predicate);
    }

    // contains (alias includes)
    template <typename Container>
    bool contains(Container container, typename Container::value_type value)
    {
        return std::find(container.begin(), container.end(), value) != container.end();
    }

    template <typename Container>
    bool includes(Container container, typename Container::value_type value)
    {
        return contains(container, value);
    }

    // invoke
    template <typename ResultContainer, typename Container, typename Function>
    typename helper::enable_if<!helper::is_void<ResultContainer>::value, ResultContainer>::type
    invoke(Container container, Function function)
    {
        ResultContainer result;
        for (typename Container::iterator i = container.begin(); i != container.end(); ++i)
        {
            helper::add_to_container(result, (*i.*function)());
        }
        return result;
    }

    template <typename ResultContainer, typename Container, typename Function>
    typename helper::enable_if<helper::is_void<ResultContainer>::value, void>::type
    invoke(Container container, Function function)
    {
        for (typename Container::iterator i = container.begin(); i != container.end(); ++i)
        {
            (*i.*function)();
        }
    }

    // pluck
    // Called like `_::pluck<vector<int>>(container, &value_type::member)`
    template <typename ResultContainer, typename Container, typename Member>
    ResultContainer pluck(Container const &container, Member member)
    {
        ResultContainer result;
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            helper::add_to_container(result, *i.*member);
        }
        return result;
    }

    /**
     * The base implementation of `_.sum` and `_.sumBy` without support for
     * iteratee shorthands.
     *
     * @private
     * @param {Array} array The array to iterate over.
     * @param {Function} iteratee The function invoked per iteration.
     * @returns {number} Returns the sum.
     */
    template <typename Container, typename Function>
    typename Container::value_type baseSum(Container array, Function iteratee)
    {
        typename Container::value_type result = {};
        for (const auto &i : array)
        {
            auto current = iteratee(i);
            result += current;
        }
        return result;
    }

    template <typename T>
    constexpr auto identity(const T &_)
    {
        return _;
    }

    /**
     * Computes the sum of the values in `array`.
     *
     * @static
     * @memberOf _
     * @since 3.4.0
     * @category Math
     * @param {Array} array The array to iterate over.
     * @returns {number} Returns the sum.
     * @example
     *
     * _.sum([4, 2, 8, 6]);
     * // => 20
     */
    template <typename Container>
    typename Container::value_type sum(Container container)
    {
        // return container.size() ? baseSum<Container>(container, identity) : 0;
        return container.size() ? baseSum<Container>(container, _IDENTITY_LAMBDA) : 0;
    }

    // max
    template <typename Container>
    typename Container::iterator max(Container container)
    {
        if (container.begin() == container.end())
        {
            return container.end();
        }

        typename Container::iterator max = container.begin();
        for (typename Container::iterator i = ++container.begin(); i != container.end(); ++i)
        {
            if (*max < *i)
            {
                max = i;
            }
        }
        return max;
    }

    template <typename Compared, typename Container, typename Function>
    typename Container::iterator max(Container container, Function function)
    {
        if (container.begin() == container.end())
        {
            return container.end();
        }

        struct
        {
            typename Container::iterator position;
            Compared computed;
        } max = {container.begin(), function(*container.begin())};

        for (typename Container::iterator i = ++container.begin(); i != container.end(); ++i)
        {
            Compared computed = function(*i);
            if (max.computed < computed)
            {
                max.position = i;
                max.computed = computed;
            }
        }
        return max.position;
    }

    // min
    template <typename Container>
    typename Container::iterator min(Container container)
    {
        if (container.begin() == container.end())
        {
            return container.end();
        }

        typename Container::iterator min = container.begin();
        for (typename Container::iterator i = ++container.begin(); i != container.end(); ++i)
        {
            if (*i < *min)
            {
                min = i;
            }
        }
        return min;
    }

    template <typename Compared, typename Container, typename Function>
    typename Container::iterator min(Container container, Function function)
    {
        if (container.begin() == container.end())
        {
            return container.end();
        }

        struct
        {
            typename Container::iterator position;
            Compared computed;
        } min = {container.begin(), function(*container.begin())};

        for (typename Container::iterator i = ++container.begin(); i != container.end(); ++i)
        {
            Compared computed = function(*i);
            if (computed < min.computed)
            {
                min.position = i;
                min.computed = computed;
            }
        }
        return min.position;
    }

    /// <summary>Returns an array of the elements in container1 and container2 that match, terminating at the first mismatch</summary>
    /// <param name="container1">container1.</param>
    /// <param name="container2">container2.</param>
    /// <returns></returns>
    /// <remarks>not an underscore or lodash function</remarks>
    template <typename ResultContainer, typename Container1, typename Container2>
    ResultContainer match_consecutive(Container1 const &container1, Container2 const &container2)
    {
        ResultContainer result;

        typename Container1::const_iterator left = container1.begin();
        typename Container2::const_iterator right = container2.begin();
        while (left != container1.end() &&
               right != container2.end())
        {
            if (*left != *right)
                break;
            helper::add_to_container(result, *left);

            *left++, *right++;
        }

        return result;
    }

    // compare
    /// <summary>Compares the contents of two arrays</summary>
    /// <param name="container1">container1.</param>
    /// <param name="container2">container2.</param>
    /// <returns>-1, 0 or 1 as per <c>strcmp</c></returns>
    /// <remarks>not an underscore or lodash function</remarks>
    template <typename Container1, typename Container2>
    int compare(const Container1 &container1, const Container2 &container2)
    {
        typename Container1::const_iterator left = container1.begin();
        typename Container2::const_iterator right = container2.begin();
        while (left != container1.end() &&
               right != container2.end())
        {
            if (*left != *right)
                return *left < *right ? -1 : 1;

            *left++, *right++;
        }

        // shorter container "win" (is less than)
        return
            // right is longer, ergo left is less
            (right != container2.end()) ? -1 :
                                        // left is longer, ergo right is less
                (left != container1.end()) ? +1
                                           :
                                           // both of equal length, ergo equal
                0;
    }

    // sort_by
    template <typename Container, typename Function>
    Container sort_by(Container container, Function function)
    {
        std::vector<typename Container::value_type> to_sort(container.begin(), container.end());
        std::sort(to_sort.begin(), to_sort.end(), function);
        return Container(to_sort.begin(), to_sort.end());
    }

    // group_by
    template <typename Key, typename Container, typename Function>
    std::multimap<Key, typename Container::value_type> group_by(Container container, Function function)
    {
        std::multimap<Key, typename Container::value_type> result;
        for (typename Container::iterator i = container.begin(); i != container.end(); ++i)
        {
            result.insert(std::pair<Key, typename Container::value_type>(function(*i), *i));
        }
        return result;
    }

    // sorted_index
    template <typename Container>
    typename Container::iterator sorted_index(
        Container container, typename Container::value_type const &value)
    {
        return std::upper_bound(container.begin(), container.end(), value);
    }

    namespace helper
    {
        template <typename Argument, typename Function>
        class TransformCompare
#if _HAS_CXX17 == 0
            : std::binary_function<Argument, Argument, bool>
#endif
        {
        public:
            TransformCompare(Function const &function)
                : function_(function)
            {
            }

            bool operator()(Argument const &left, Argument const &right) const
            {
                return function_(left) < function_(right);
            }

        private:
            Function function_;
        };
    } // namespace helper

    template <typename Container, typename Function>
    typename Container::iterator sorted_index(
        Container container, typename Container::value_type const &value, Function function)
    {
        return std::upper_bound(container.begin(), container.end(), value,
                                helper::TransformCompare<typename Container::value_type, Function>(function));
    }

    // shuffle
    // This assumes srand has already been called.
    template <typename ResultContainer, typename Container>
    ResultContainer shuffle(Container container)
    {
        std::vector<typename Container::value_type> deck(container.begin(), container.end());
        for (int i = deck.size() - 1; i > 1; --i)
        {
            int j = std::rand() % (i + 1);
            std::swap(deck[i], deck[j]);
        }
        return ResultContainer(deck.begin(), deck.end());
    }

    // to_array
    template <typename Container>
    typename Container::value_type *to_array(Container container)
    {
        typename Container::value_type *array = new typename Container::value_type[container.size()];
        struct
        {
            int numeric;
            typename Container::iterator iterator;
        } i;
        for (i.numeric = 0, i.iterator = container.begin(); i.iterator != container.end();
             ++i.numeric, ++i.iterator)
        {
            array[i.numeric] = *i.iterator;
        }

        return array;
    }

    // size
    template <typename Container>
    int size(Container container)
    {
        return container.size();
    }

    // Arrays

    // first/head
    template <typename Container>
    typename Container::iterator first(Container &container)
    {
        return container.begin();
    }

    template <typename ResultContainer, typename Container>
    ResultContainer first(Container &container, int count)
    {
        typename Container::iterator end = container.begin();
        std::advance(end, count);
        return ResultContainer(container.begin(), end);
    }

    template <typename Container>
    typename Container::iterator head(Container &container)
    {
        return first(container);
    }

    template <typename ResultContainer, typename Container>
    ResultContainer head(Container &container, int count)
    {
        return first<ResultContainer>(container, count);
    }

    /// <summary>Similar to <paramref="first" /> but returns an array of between 0 and 1 elements</summary>
    /// <param name="container">The container.</param>
    /// <returns></returns>
    template <typename ResultContainer, typename Container>
    ResultContainer first_jquery(Container &container)
    {

        ResultContainer result;
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            helper::add_to_container(result, *i);
            break;
        }
        return result;
    }

    // initial
    template <typename ResultContainer, typename Container>
    ResultContainer initial(Container &container)
    {
        typename Container::iterator end = container.begin();
        std::advance(end, container.size() - 1);
        return ResultContainer(container.begin(), end);
    }

    template <typename ResultContainer, typename Container>
    ResultContainer initial(Container &container, int n)
    {
        typename Container::iterator end = container.begin();
        std::advance(end, container.size() - n);
        return ResultContainer(container.begin(), end);
    }

    // last
    template <typename Container>
    typename Container::iterator last(Container &container)
    {
        typename Container::iterator last = container.begin();
        std::advance(last, container.size() - 1);
        return last;
    }

    template <typename ResultContainer, typename Container>
    ResultContainer last(Container &container, int n)
    {
        typename Container::iterator begin = container.begin();
        std::advance(begin, container.size() - n);
        return ResultContainer(begin, container.end());
    }

    // rest/tail
    template <typename ResultContainer, typename Container>
    ResultContainer rest(Container &container)
    {
        return ResultContainer(++container.begin(), container.end());
    }

    template <typename ResultContainer, typename Container>
    ResultContainer rest(Container &container, int index)
    {
        typename Container::iterator begin = container.begin();
        std::advance(begin, index);
        return ResultContainer(begin, container.end());
    }

    template <typename ResultContainer, typename Container>
    ResultContainer tail(Container &container)
    {
        return rest<ResultContainer>(container);
    }

    template <typename ResultContainer, typename Container>
    ResultContainer tail(Container &container, int index)
    {
        return rest<ResultContainer>(container, index);
    }

    // concat
    /// <summary>Creates a new array concatenating array <paramref="container1" /> with <paramref="container2" /></summary>
    /// <param name="container1">container1</param>
    /// <param name="container2">container2</param>
    /// <returns></returns>
    template <typename ResultContainer, typename Container1, typename Container2>
    ResultContainer concat(const Container1 &container1, const Container2 &container2)
    {
        ResultContainer result;

        // This may be a terrible idea, if reserve or size is not defined.
        // result.reserve(container1.size() + container2.size());
        each(container1, [&result](auto value)
             { helper::add_to_container(result, value); });
        each(container2, [&result](auto value)
             { helper::add_to_container(result, value); });

        // vector1.insert(vector1.end(), vector2.begin(), vector2.end());
        // for (auto i = container.begin(); i != container.end(); ++i)
        //{
        //     if (static_cast<bool>(*i))
        //     {
        //         helper::add_to_container(result, *i);
        //     }
        // }
        return result;
    }

    /// <summary>Append the contents of <paramref="source" /> to <paramref="destination" /></summary>
    /// <param name="destination">The destination array</param>
    /// <param name="source">The source array</param>
    /// <returns>void</returns>
    template <typename Container1, typename Container2>
    void concat_inplace(Container1 &destination, Container2 const &source)
    {
        // This may be a terrible idea, if reserve or size is not defined.
        // result.reserve(container1.size() + source.size());
        // each(container1, [&result](auto value) { helper::add_to_container(result, value);  });
        each(source, [&destination](const auto &value)
             { helper::add_to_container(destination, value); });

        // vector1.insert(vector1.end(), vector2.begin(), vector2.end());
        // for (auto i = container.begin(); i != container.end(); ++i)
        //{
        //     if (static_cast<bool>(*i))
        //     {
        //         helper::add_to_container(result, *i);
        //     }
        // }
        // return result;
    }

    // compact
    template <typename ResultContainer, typename Container>
    ResultContainer compact(Container const &container)
    {
        ResultContainer result;
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            if (static_cast<bool>(*i))
            {
                helper::add_to_container(result, *i);
            }
        }
        return result;
    }

    // flatten
    namespace helper
    {
        template <typename ResultContainer, typename Container>
        ResultContainer flatten_one_layer(Container const &container)
        {
            ResultContainer result;
            for (auto i = container.begin(); i != container.end(); ++i)
            {
                for (typename Container::value_type::const_iterator j = i->begin(); j != i->end(); ++j)
                {
                    add_to_container(result, *j);
                }
            }
            return result;
        }

        template <typename T>
        class HasConstIterator
        {
        private:
            typedef char yes[1];
            typedef char no[2];
            template <typename C>
            static yes &test(typename C::const_iterator *);
            template <typename C>
            static no &test(...);

        public:
            static bool const value = sizeof(test<T>(0)) == sizeof(yes);
        };

        template <typename ResultContainer, typename Container>
        typename enable_if<!HasConstIterator<typename Container::value_type>::value, void>::type
        flatten_loop(ResultContainer &result, Container const &container)
        {
            for (auto i = container.begin(); i != container.end(); ++i)
            {
                add_to_container(result, *i);
            }
        }

        template <typename ResultContainer, typename Container>
        typename enable_if<HasConstIterator<typename Container::value_type>::value, void>::type
        flatten_loop(ResultContainer &result, Container const &container)
        {
            for (auto i = container.begin(); i != container.end(); ++i)
            {
                flatten_loop(result, *i);
            }
        }

    } // namespace helper

    template <typename ResultContainer, typename Container>
    ResultContainer flatten(Container const &container)
    {
        ResultContainer result;
        helper::flatten_loop(result, container);
        return result;
    }

    template <typename ResultContainer, bool shallow, typename Container>
    typename helper::enable_if<shallow == true, ResultContainer>::type
    flatten(Container const &container)
    {
        return helper::flatten_one_layer<ResultContainer>(container);
    }

    template <typename ResultContainer, bool shallow, typename Container>
    typename helper::enable_if<shallow == false, ResultContainer>::type
    flatten(Container const &container)
    {
        return flatten<ResultContainer>(container);
    }

    // uniq/unique
    template <typename ResultContainer, typename Key, typename Container, typename Function>
    ResultContainer uniq(Container const &container, bool is_sorted, Function function)
    {
        ResultContainer result;
        std::vector<Key> keys = map<std::vector<Key>>(container, function);
        if (container.size() < 3)
        {
            is_sorted = true;
        }

        std::vector<Key> memo;

        for (
            std::pair<typename std::vector<Key>::const_iterator, typename Container::const_iterator> i = std::make_pair(keys.begin(), container.begin()); i.first != keys.end(); ++i.first, ++i.second)
        {
            if (is_sorted ? !memo.size() || *last(memo) != *i.first : !includes(memo, *i.first))
            {
                memo.push_back(*i.first);
                helper::add_to_container(result, *i.second);
            }
        }
        return result;
    }

    /// <summary>This method is like _.uniq except that it accepts iteratee which is invoked for each element in array to generate the criterion by which uniqueness is computed. The order of result values is determined by the order they occur in the array. The iteratee is invoked with one argument</summary>
    /// <param name="container">The container.</param>
    /// <param name="function">iteratee(<paramref name="Key" /> identity)</param>
    /// <returns></returns>
    template <typename ResultContainer, typename Key, typename Container, typename Function>
    ResultContainer uniqBy(Container const &container, Function function)
    {
        return uniq<ResultContainer, Key>(container, false, function);
    }

    template <typename ResultContainer, typename Key, typename Container, typename Function>
    ResultContainer uniq(Container const &container, Function function)
    {
        return uniq<ResultContainer, Key>(container, false, function);
    }

    template <typename ResultContainer, typename Container>
    ResultContainer uniq(Container const &container, bool is_sorted)
    {
        ResultContainer result;
        if (container.size() < 3)
        {
            is_sorted = true;
        }

        std::vector<typename Container::value_type> memo;
        for (auto i = container.begin(); i != container.end(); ++i)
        {
            if (is_sorted ? !memo.size() || *last(memo) != *i : !includes(memo, *i))
            {
                memo.push_back(*i);
                helper::add_to_container(result, *i);
            }
        }
        return result;
    }

    template <typename ResultContainer, typename Container>
    ResultContainer uniq(Container const &container)
    {
        return uniq<ResultContainer>(container, false);
    }

    template <typename ResultContainer, typename Key, typename Container, typename Function>
    ResultContainer unique(Container const &container, bool is_sorted, Function function)
    {
        return uniq<ResultContainer, Key>(container, is_sorted, function);
    }

    template <typename ResultContainer, typename Key, typename Container, typename Function>
    ResultContainer unique(Container const &container, Function function)
    {
        return uniq<ResultContainer, Key>(container, false, function);
    }

    template <typename ResultContainer, typename Container>
    ResultContainer unique(Container const &container, bool is_sorted)
    {
        return uniq<ResultContainer>(container, is_sorted);
    }

    template <typename ResultContainer, typename Container>
    ResultContainer unique(Container const &container)
    {
        return uniq<ResultContainer>(container, false);
    }

    template <typename ResultContainer, typename Container, typename Function>
    ResultContainer unique(Container const &container, Function function)
    {
        return uniq<ResultContainer>(container, false);
    }

    // union_of
    template <typename ResultContainer, typename Container1, typename Container2>
    ResultContainer union_of(Container1 const &container1, Container2 const &container2)
    {
        std::vector<typename ResultContainer::value_type> left(container1.begin(), container1.end());
        std::vector<typename ResultContainer::value_type> right(container2.begin(), container2.end());
        std::sort(left.begin(), left.end());
        std::sort(right.begin(), right.end());

        std::vector<typename ResultContainer::value_type> union_result;
        std::set_union(
            left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(union_result));
        return ResultContainer(union_result.begin(), union_result.end());
    }

    // intersection
    template <typename ResultContainer, typename Container1, typename Container2>
    ResultContainer intersection(Container1 const &container1, Container2 const &container2)
    {
        std::vector<typename ResultContainer::value_type> left(container1.begin(), container1.end());
        std::vector<typename ResultContainer::value_type> right(container2.begin(), container2.end());
        std::sort(left.begin(), left.end());
        std::sort(right.begin(), right.end());

        std::vector<typename ResultContainer::value_type> union_result;
        std::set_intersection(
            left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(union_result));
        return ResultContainer(union_result.begin(), union_result.end());
    }

    // difference
    template <typename ResultContainer, typename Container1, typename Container2>
    ResultContainer difference(Container1 const &container1, Container2 const &container2)
    {
        std::vector<typename ResultContainer::value_type> left(container1.begin(), container1.end());
        std::vector<typename ResultContainer::value_type> right(container2.begin(), container2.end());
        std::sort(left.begin(), left.end());
        std::sort(right.begin(), right.end());

        std::vector<typename ResultContainer::value_type> union_result;
        std::set_difference(
            left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(union_result));
        return ResultContainer(union_result.begin(), union_result.end());
    }

    // difference2 - because `difference` doesn't work for all types
    template <typename ResultContainer, typename Container1, typename Container2>
    ResultContainer difference2(Container1 const &container1, Container2 const &container2)
    {
        return filter<ResultContainer>(container1, [&](const auto &value)
                                       { return !contains(container2, value); });
    }

    // zip
    /// <summary>Merges together the values of each of the arrays with the values at the corresponding position. Useful when you have separate data sources that are coordinated through matching array indexes.
    /// </summary>
    /// <param name="container1">The container1.</param>
    /// <param name="container2">The container2.</param>
    /// <returns>A sequential container of size <c>min(keys.size(), values.size())</c></returns>
    /// <example><code><![CDATA[// JavaScript example from underscore.org
    /// _.zip(['moe', 'larry', 'curly'], [30, 40, 50], [true, false, false]);
    /// // => [["moe", 30, true], ["larry", 40, false], ["curly", 50, false]]    /// }]]></code></example>
    /// <remarks>Limited to 2 arrays</remarks>
    /// <remarks>lodash version should actually take an array of arrays as a single argument</remarks>
    template <typename ResultContainer, typename Container1, typename Container2>
    ResultContainer zip(const Container1 &container1, const Container2 &container2)
    {
        ResultContainer result;
        typename Container1::const_iterator left = container1.begin();
        typename Container2::const_iterator right = container2.begin();
        while (left != container1.end() && right != container2.end())
        {
            helper::add_to_container(result, typename ResultContainer::value_type(*left++, *right++));
        }
        return result;
    }

    /// <summary>Converts arrays into objects. Pass a list of keys, and a list of values. If duplicate keys exist, the last value wins.</summary>
    /// <param name="keys">The keys.</param>
    /// <param name="values">The values.</param>
    /// <returns>An associative container of size <c>min(keys.size(), values.size())</c></returns>
    /// <example><code><![CDATA[// JavaScript example from https://lodash.com/docs/4.17.4#zipObject
    /// _.zipObject(['a', 'b'], [1, 2]);
    /// // => { 'a': 1, 'b': 2 }	/// <remarks>Limited to 2 arrays</remarks>
    /// <remarks>a.k.a. underscore's <c>object()</c> function, when passing a list of keys, and a list of values</remarks>
    /// <remarks></remarks>
    template <typename ResultContainer, typename Container1, typename Container2>
    ResultContainer zipObject(const Container1 &keys, const Container2 &values)
    {
        ResultContainer result;
        typename Container1::const_iterator left = keys.begin();
        typename Container2::const_iterator right = values.begin();
        while (left != keys.end() && right != values.end())
        {
            helper::add_to_container(result, typename ResultContainer::value_type(*left++, *right++));
        }
        return result;
    }

    // indexOf
    template <typename Container>
    int indexOf(Container &container, typename Container::value_type value)
    {
        auto value_position = std::find(container.begin(), container.end(), value);
        return value_position == container.end() ? -1
                                                 : std::distance(container.begin(), value_position);
    }

    // `indexOf` that accepts `Container::value_type = std::pair<K, V>`
    template <typename Container, typename Value>
    int indexOfMap(Container &container, Value value)
    {
        // https://stackoverflow.com/questions/12742472/how-to-get-matching-key-using-the-value-in-a-map-c

        auto value_position = std::find_if(std::begin(container), std::end(container), [&](const auto &pair)
                                           { return pair.second == value; });

        return value_position == container.end() ? -1
                                                 : std::distance(container.begin(), value_position);
    }

    // `contains` that accepts `Container::value_type = std::pair<K, V>`
    template <typename Container, typename Value>
    bool containsMap(Container container, Value value)
    {
        return indexOfMap(container, value) != -1;
    }

    template <typename Container>
    int indexOf(Container &container, typename Container::value_type value, bool is_sorted)
    {
        if (!is_sorted)
        {
            return indexOf(container, value);
        }
        typename Container::iterator value_lower_bound = std::lower_bound(container.begin(), container.end(), value);
        return value_lower_bound == container.end() || *value_lower_bound != value
                   ? -1
                   : std::distance(container.begin(), value_lower_bound);
    }

    // last_index_of
    template <typename Container>
    int last_index_of(Container const &container, typename Container::value_type value)
    {
        typename Container::const_iterator result = std::find(container.begin(), container.end(), value);
        typename Container::const_iterator i = result;
        while (i != container.end())
        {
            i = std::find(++i, container.end(), value);
            if (i != container.end())
            {
                result = i;
            }
        }
        return result == container.end() ? -1 : std::distance(container.begin(), result);
    }

    // range
    template <typename ResultContainer>
    ResultContainer range(int start, int stop, int step)
    {
        int length = std::max((stop - start) / step, 0);
        int index = 0;
        ResultContainer result;

        while (index < length)
        {
            helper::add_to_container(result, start);
            start += step;
            ++index;
        }

        return result;
    }

    template <typename ResultContainer>
    ResultContainer range(int start, int stop)
    {
        return range<ResultContainer>(start, stop, 1);
    }

    template <typename ResultContainer>
    ResultContainer range(int stop)
    {
        return range<ResultContainer>(0, stop, 1);
    }

    template <typename T>
    auto dereference(T _)
    {
        return *_;
    }

    /// <summary>Invokes the iteratee n times, returning an array of the results of each invocation. The iteratee is invoked with one argument; (index).</summary>
    /// <param name="n">The number of times to invoke <paramref="iteratee" /></param>
    /// <param name="iteratee">The iteratee, iteratee(size_t n)</param>
    /// <returns>Array of the returned values</returns>
    template <typename ResultContainer, typename Function>
    ResultContainer times(size_t n, Function iteratee)
    {
        auto accum = range<std::vector<size_t>>(n);
        return map<ResultContainer>(accum, iteratee);
    };

#ifdef UNDERSCORE_BONUS
    // at
    template <typename Container>
    auto &at(Container &container, typename Container::key_type key)
    {
        return container.at(key);
    }

    template <typename Container>
    auto tryAndGet(Container container, const typename Container::key_type key, typename Container::value_type &value)
    {
        if (contains(container, key))
        {
            value = at(container, key);
            return true;
        }
        return false;
    }

    template <typename Container>
    auto &getOrCall(Container container, const typename Container::key_type key, typename Container::value_type (*function)(typename Container::key_type))
    {
        if (!contains(container, key))
            helper::add_to_container(container, key, function(key));
        return at(container, key);
    }

    template <typename Container>
    auto &getOrDefault(Container container, const typename Container::key_type key, const typename Container::value_type &value)
    {
        if (!contains(container, key))
            helper::add_to_container(container, key, value);
        return at(container, key);
    }

    template <typename Container>
    bool tryAndPop(Container &the_queue, typename Container::value_type &popped_value)
    {
        if (the_queue.empty())
        {
            return false;
        }

        popped_value = the_queue.front();
        the_queue.pop_front();
        return true;
    }

    /// <summary>Perumtate the specified containers.</summary>
    /// <param name="container1">container 1.</param>
    /// <param name="container2">container 2.</param>
    /// <param name="iteratee">The iteratee, iteratee(a, b)</param>
    /// <returns>Unique permutations of the two containers passed through iteratee</returns>
    template <typename ResultContainer, typename Container1, typename Container2, typename Function>
    ResultContainer permutate(const Container1 &container1, const Container2 &container2, Function iteratee)
    {
        ResultContainer result;
        for (auto i = 0; i < container1.size(); i++)
        {
            for (auto j = 0; j < container2.size(); j++)
            {
                helper::add_to_container(result, iteratee(container1[i], container2[j]));
            }
        }
        return result;
    }
#endif

    // Functions

    // bind
    // bindAll
    // memoize
    // delay
    // defer
    // throttle
    // debounce
    // once
    // after
    // wrap
    // compose

    // Objects

    // keys
    // values
    // functions
    // extend
    // defaults
    // clone
    // tap
    // has
    // isEqual
    // isEmpty
    // isElement
    // isArray
    // isArguments
    // isFunction
    // isString
    // isNumber
    // isBoolean
    // isDate
    // isRegExp
    // isNaN
    // isNull
    // isUndefined

    // Utility

    // noConflict
    // identity
    // times
    // mixin
    // uniqueId
    // escape
    // template

    // Chaining

    template <typename Container>
    class Wrapper;

    // chain
    template <typename Container>
    Wrapper<Container> chain(Container container)
    {
        return Wrapper<Container>(container);
    }

    // value
    template <typename Container>
    typename Container::value_type value(Wrapper<Container> &wrapper)
    {
        return wrapper.value();
    }

    template <typename Container>
    class Wrapper
    {
    public:
        // what's this for?
        typedef Container value_type;
        Wrapper(Container container)
            : container_(container)
        {
        }

        Container value()
        {
            return container_;
        }

        template <typename Function>
        Wrapper &each(Function function)
        {
            _::each(container_, function);
            return *this;
        }

        template <typename Function, typename ResultContainer = std::vector<typename Container::value_type>>
        Wrapper<ResultContainer> filter(Function function)
        {
            return chain(_::filter<ResultContainer>(container_, function));
        }

        template <typename ResultContainer, typename Function>
        Wrapper<ResultContainer> map(Function function)
        {
            return chain(_::map<ResultContainer>(container_, function));
        }

        template <typename Function, typename Memo>
        Wrapper<Memo> reduce(Function function, Memo memo)
        {
            return chain(_::reduce(container_, function, memo));
        }

    private:
        Container container_;
    };

} // namespace _

// namespace _ = _;

// template <typename Key, typename Iterator>
// struct KeyIterator
//{
//     KeyIterator(
//         Iterator i)
//         :_i(i)
//     {
//     }
//
//     KeyIterator operator++()
//     {
//         ++_i;
//         return *this;
//     }
//
//     bool operator==(
//         KeyIterator ki)
//     {
//         return _i = ki._i;
//     }
//
//     typename Iterator::value_type operator*()
//     {
//         return _i->first;
//     }
// };

// see also: https://stackoverflow.com/questions/43992510/enable-if-to-check-if-value-type-of-iterator-is-a-pair
#endif // UNDERSCORE_UNDERSCORE_H_