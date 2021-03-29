#include <caesar/type_traits.hpp>

#include <doctest/doctest.h>
#include <list>

namespace cs = caesar;

struct TypeTraitsTest {
    struct Foo {};
    struct MyInt {};
    struct MyFloat {};
};

// Specialize `std::numeric_limits` for `MyInt` & `MyFloat`.
template<>
class std::numeric_limits<TypeTraitsTest::MyInt>
    : public std::numeric_limits<int> {};

template<>
class std::numeric_limits<TypeTraitsTest::MyFloat>
    : public std::numeric_limits<float> {};

TEST_CASE_FIXTURE(TypeTraitsTest, "type_traits.is_arithmetic")
{
    CHECK(cs::is_arithmetic_v<bool>);
    CHECK(cs::is_arithmetic_v<unsigned>);
    CHECK(cs::is_arithmetic_v<long long>);
    CHECK(cs::is_arithmetic_v<float>);
    CHECK(cs::is_arithmetic_v<const double>);
    CHECK(cs::is_arithmetic_v<MyInt>);
    CHECK(cs::is_arithmetic_v<const MyFloat>);

    CHECK_FALSE(cs::is_arithmetic_v<Foo>);
    CHECK_FALSE(cs::is_arithmetic_v<int&>);
    CHECK_FALSE(cs::is_arithmetic_v<std::list<double>>);
}
