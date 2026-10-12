#include "pch.h"
#include "BitField.h"
#include "Set.h"
#include <sstream>
#include <stdexcept>

TEST(BitFieldConstructor, DefaultIsEmpty) {
    BitField bf;
    EXPECT_EQ(bf.size(), 0u);
    EXPECT_EQ(bf.words(), 0u);
}

TEST(BitFieldConstructor, SizeIsStored) {
    BitField bf(10);
    EXPECT_EQ(bf.size(), 10u);
    EXPECT_EQ(bf.words(), 1u);
}

TEST(BitFieldConstructor, WordsRoundedUp) {
    BitField bf(33);
    EXPECT_EQ(bf.words(), 2u);

    BitField bf2(64);
    EXPECT_EQ(bf2.words(), 2u);

    BitField bf3(65);
    EXPECT_EQ(bf3.words(), 3u);
}

TEST(BitFieldConstructor, CopyIsIndependent) {
    BitField a(8);
    a.set_bit(3);
    BitField b(a);
    b.reset_bit(3);
    EXPECT_TRUE(a.get_bit(3));
    EXPECT_FALSE(b.get_bit(3));
}

TEST(BitFieldConstructor, MoveLeavesSourceEmpty) {
    BitField a(8);
    a.set_bit(3);
    BitField b(std::move(a));
    EXPECT_EQ(b.size(), 8u);
    EXPECT_TRUE(b.get_bit(3));
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.words(), 0u);
}

TEST(BitFieldSetBit, SetsOnlyRequestedBit) {
    BitField bf(8);
    bf.set_bit(2);
    EXPECT_TRUE(bf.get_bit(2));
    EXPECT_FALSE(bf.get_bit(0));
    EXPECT_FALSE(bf.get_bit(1));
    EXPECT_FALSE(bf.get_bit(3));
}

TEST(BitFieldSetBit, Idempotent) {
    BitField bf(8);
    bf.set_bit(2);
    bf.set_bit(2);
    EXPECT_TRUE(bf.get_bit(2));
}

TEST(BitFieldSetBit, OutOfRangeThrows) {
    BitField bf(8);
    EXPECT_THROW(bf.set_bit(8), std::out_of_range);
    EXPECT_THROW(bf.set_bit(100), std::out_of_range);
}

TEST(BitFieldResetBit, ClearsBit) {
    BitField bf(8);
    bf.set_bit(4);
    bf.reset_bit(4);
    EXPECT_FALSE(bf.get_bit(4));
}

TEST(BitFieldResetBit, OutOfRangeThrows) {
    BitField bf(8);
    EXPECT_THROW(bf.reset_bit(8), std::out_of_range);
}

TEST(BitFieldGetBit, OutOfRangeThrows) {
    BitField bf(8);
    EXPECT_THROW(bf.get_bit(8), std::out_of_range);
}

TEST(BitFieldGetBit, FreshFieldIsZero) {
    BitField bf(16);
    for (size_t i = 0; i < 16; ++i)
        EXPECT_FALSE(bf.get_bit(i));
}

TEST(BitFieldAssignment, CopyAssignment) {
    BitField a(8);
    a.set_bit(1);
    BitField b(8);
    b = a;
    EXPECT_EQ(a, b);
    b.reset_bit(1);
    EXPECT_NE(a, b);
}

TEST(BitFieldAssignment, SelfAssignmentSafe) {
    BitField a(8);
    a.set_bit(1);
    a = a;
    EXPECT_TRUE(a.get_bit(1));
    EXPECT_EQ(a.size(), 8u);
}

TEST(BitFieldAssignment, MoveAssignment) {
    BitField a(8);
    a.set_bit(1);
    BitField b(4);
    b = std::move(a);
    EXPECT_EQ(b.size(), 8u);
    EXPECT_TRUE(b.get_bit(1));
    EXPECT_EQ(a.size(), 0u);
}

TEST(BitFieldEquality, EmptyAreEqual) {
    BitField a, b;
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(BitFieldEquality, DifferentSizeNotEqual) {
    BitField a(8), b(16);
    EXPECT_FALSE(a == b);
}

TEST(BitFieldEquality, SameContentEqual) {
    BitField a(8), b(8);
    a.set_bit(3);
    b.set_bit(3);
    EXPECT_TRUE(a == b);
}

TEST(BitFieldOr, BasicUnion) {
    BitField a(8), b(8);
    a.set_bit(0);
    b.set_bit(1);
    BitField c = a | b;
    EXPECT_TRUE(c.get_bit(0));
    EXPECT_TRUE(c.get_bit(1));
}

TEST(BitFieldOr, SizeMismatchThrows) {
    BitField a(8), b(16);
    EXPECT_THROW(a | b, std::invalid_argument);
}

TEST(BitFieldAnd, BasicIntersection) {
    BitField a(8), b(8);
    a.set_bit(0);
    a.set_bit(1);
    b.set_bit(1);
    BitField c = a & b;
    EXPECT_FALSE(c.get_bit(0));
    EXPECT_TRUE(c.get_bit(1));
}

TEST(BitFieldAnd, SizeMismatchThrows) {
    BitField a(8), b(16);
    EXPECT_THROW(a & b, std::invalid_argument);
}

TEST(BitFieldXor, BasicXor) {
    BitField a(8), b(8);
    a.set_bit(0);
    a.set_bit(1);
    b.set_bit(1);
    b.set_bit(2);
    BitField c = a ^ b;
    EXPECT_TRUE(c.get_bit(0));
    EXPECT_FALSE(c.get_bit(1));
    EXPECT_TRUE(c.get_bit(2));
}

TEST(BitFieldXor, SizeMismatchThrows) {
    BitField a(8), b(16);
    EXPECT_THROW(a ^ b, std::invalid_argument);
}

TEST(BitFieldNot, InvertsBits) {
    BitField a(8);
    BitField b = ~a;
    for (size_t i = 0; i < 8; ++i)
        EXPECT_TRUE(b.get_bit(i));
}

TEST(BitFieldCompoundOr, Works) {
    BitField a(8), b(8);
    a.set_bit(0);
    b.set_bit(1);
    a |= b;
    EXPECT_TRUE(a.get_bit(0));
    EXPECT_TRUE(a.get_bit(1));
}

TEST(BitFieldCompoundAnd, Works) {
    BitField a(8), b(8);
    a.set_bit(0);
    a.set_bit(1);
    b.set_bit(1);
    a &= b;
    EXPECT_FALSE(a.get_bit(0));
    EXPECT_TRUE(a.get_bit(1));
}

TEST(BitFieldCompoundXor, Works) {
    BitField a(8), b(8);
    a.set_bit(0);
    a.set_bit(1);
    b.set_bit(1);
    a ^= b;
    EXPECT_TRUE(a.get_bit(0));
    EXPECT_FALSE(a.get_bit(1));
}

TEST(BitFieldOutput, PrintsZeros) {
    BitField bf(4);
    std::ostringstream os;
    os << bf;
    EXPECT_EQ(os.str(), "0000");
}

TEST(BitFieldOutput, PrintsBitsLeftToRight) {
    BitField bf(4);
    bf.set_bit(0);
    bf.set_bit(2);
    std::ostringstream os;
    os << bf;
    EXPECT_EQ(os.str(), "1010");
}

TEST(BitFieldInput, ReadsStringIntoEmptyField) {
    std::istringstream is("1010");
    BitField bf;
    is >> bf;
    EXPECT_EQ(bf.size(), 4u);
    EXPECT_TRUE(bf.get_bit(0));
    EXPECT_FALSE(bf.get_bit(1));
    EXPECT_TRUE(bf.get_bit(2));
    EXPECT_FALSE(bf.get_bit(3));
}

TEST(BitFieldInput, ReadsIntoPresizedField) {
    std::istringstream is("1010");
    BitField bf(4);
    is >> bf;
    EXPECT_TRUE(bf.get_bit(0));
    EXPECT_FALSE(bf.get_bit(1));
    EXPECT_TRUE(bf.get_bit(2));
    EXPECT_FALSE(bf.get_bit(3));
}

TEST(BitFieldRoundTrip, OutputThenInput) {
    BitField a(6);
    a.set_bit(0);
    a.set_bit(5);
    std::stringstream ss;
    ss << a;
    BitField b(6);
    ss >> b;
    EXPECT_EQ(a, b);
}

TEST(SetConstructor, DefaultIsEmpty) {
    Set s;
    EXPECT_EQ(s.max_power(), 0u);
}

TEST(SetConstructor, SizedIsEmpty) {
    Set s(10);
    EXPECT_EQ(s.max_power(), 10u);
    for (int i = 1; i <= 10; ++i)
        EXPECT_FALSE(s.is_member(i));
}

TEST(SetConstructor, FromBitField) {
    BitField bf(8);
    bf.set_bit(2);
    Set s(bf);
    EXPECT_EQ(s.max_power(), 8u);
    EXPECT_TRUE(s.is_member(3));
    EXPECT_FALSE(s.is_member(1));
}

TEST(SetConstructor, CopyIsIndependent) {
    Set a(8);
    a.insert(3);
    Set b(a);
    b.erase(3);
    EXPECT_TRUE(a.is_member(3));
    EXPECT_FALSE(b.is_member(3));
}

TEST(SetConstructor, MoveLeavesSourceEmpty) {
    Set a(8);
    a.insert(3);
    Set b(std::move(a));
    EXPECT_EQ(b.max_power(), 8u);
    EXPECT_TRUE(b.is_member(3));
    EXPECT_EQ(a.max_power(), 0u);
}

TEST(SetInsert, AddsElement) {
    Set s(10);
    s.insert(5);
    EXPECT_TRUE(s.is_member(5));
}

TEST(SetInsert, Idempotent) {
    Set s(10);
    s.insert(5);
    s.insert(5);
    EXPECT_TRUE(s.is_member(5));
}

TEST(SetInsert, OutOfRangeThrows) {
    Set s(10);
    EXPECT_THROW(s.insert(0), std::out_of_range);
    EXPECT_THROW(s.insert(-1), std::out_of_range);
    EXPECT_THROW(s.insert(11), std::out_of_range);
}

TEST(SetErase, RemovesElement) {
    Set s(10);
    s.insert(5);
    s.erase(5);
    EXPECT_FALSE(s.is_member(5));
}

TEST(SetErase, ErasingAbsentIsNoOp) {
    Set s(10);
    s.erase(5);
    EXPECT_FALSE(s.is_member(5));
}

TEST(SetErase, OutOfRangeThrows) {
    Set s(10);
    EXPECT_THROW(s.erase(0), std::out_of_range);
    EXPECT_THROW(s.erase(11), std::out_of_range);
}

TEST(SetIsMember, OutOfRangeReturnsFalse) {
    Set s(10);
    EXPECT_FALSE(s.is_member(0));
    EXPECT_FALSE(s.is_member(-1));
    EXPECT_FALSE(s.is_member(11));
}

TEST(SetElemPlus, AddElement) {
    Set s(10);
    Set t = s + 5;
    EXPECT_TRUE(t.is_member(5));
    EXPECT_FALSE(s.is_member(5));
}

TEST(SetElemMinus, RemoveElement) {
    Set s(10);
    s.insert(5);
    Set t = s - 5;
    EXPECT_FALSE(t.is_member(5));
    EXPECT_TRUE(s.is_member(5));
}

TEST(SetElemPlusAssign, Works) {
    Set s(10);
    s += 5;
    EXPECT_TRUE(s.is_member(5));
}

TEST(SetElemMinusAssign, Works) {
    Set s(10);
    s.insert(5);
    s -= 5;
    EXPECT_FALSE(s.is_member(5));
}

TEST(SetUnion, Basic) {
    Set a(8), b(8);
    a.insert(1);
    b.insert(2);
    Set c = a + b;
    EXPECT_TRUE(c.is_member(1));
    EXPECT_TRUE(c.is_member(2));
}

TEST(SetUnion, SizeMismatchThrows) {
    Set a(8), b(16);
    EXPECT_THROW(a + b, std::invalid_argument);
}

TEST(SetIntersection, Basic) {
    Set a(8), b(8);
    a.insert(1);
    a.insert(2);
    b.insert(2);
    b.insert(3);
    Set c = a * b;
    EXPECT_FALSE(c.is_member(1));
    EXPECT_TRUE(c.is_member(2));
    EXPECT_FALSE(c.is_member(3));
}

TEST(SetIntersection, SizeMismatchThrows) {
    Set a(8), b(16);
    EXPECT_THROW(a * b, std::invalid_argument);
}

TEST(SetDifference, Basic) {
    Set a(8), b(8);
    a.insert(1);
    a.insert(2);
    b.insert(2);
    Set c = a - b;
    EXPECT_TRUE(c.is_member(1));
    EXPECT_FALSE(c.is_member(2));
}

TEST(SetDifference, SizeMismatchThrows) {
    Set a(8), b(16);
    EXPECT_THROW(a - b, std::invalid_argument);
}

TEST(SetCompoundUnion, Works) {
    Set a(8), b(8);
    a.insert(1);
    b.insert(2);
    a += b;
    EXPECT_TRUE(a.is_member(1));
    EXPECT_TRUE(a.is_member(2));
}

TEST(SetCompoundUnion, SizeMismatchThrows) {
    Set a(8), b(16);
    EXPECT_THROW(a += b, std::invalid_argument);
}

TEST(SetCompoundIntersection, Works) {
    Set a(8), b(8);
    a.insert(1);
    a.insert(2);
    b.insert(2);
    a *= b;
    EXPECT_FALSE(a.is_member(1));
    EXPECT_TRUE(a.is_member(2));
}

TEST(SetCompoundDifference, Works) {
    Set a(8), b(8);
    a.insert(1);
    a.insert(2);
    b.insert(2);
    a -= b;
    EXPECT_TRUE(a.is_member(1));
    EXPECT_FALSE(a.is_member(2));
}

TEST(SetUnaryMinus, ComplementOnAlignedSize) {
    Set a(32);
    a.insert(1);
    a.insert(32);
    Set b = -a;
    EXPECT_FALSE(b.is_member(1));
    EXPECT_FALSE(b.is_member(32));
    EXPECT_TRUE(b.is_member(2));
    EXPECT_TRUE(b.is_member(31));
}

TEST(SetAssignment, CopyAssignment) {
    Set a(8);
    a.insert(3);
    Set b(8);
    b = a;
    EXPECT_TRUE(b.is_member(3));
    b.erase(3);
    EXPECT_TRUE(a.is_member(3));
}

TEST(SetAssignment, SelfAssignmentSafe) {
    Set a(8);
    a.insert(3);
    a = a;
    EXPECT_TRUE(a.is_member(3));
    EXPECT_EQ(a.max_power(), 8u);
}

TEST(SetAssignment, MoveAssignment) {
    Set a(8);
    a.insert(3);
    Set b(4);
    b = std::move(a);
    EXPECT_EQ(b.max_power(), 8u);
    EXPECT_TRUE(b.is_member(3));
    EXPECT_EQ(a.max_power(), 0u);
}

TEST(SetOutput, Empty) {
    Set s(8);
    std::ostringstream os;
    os << s;
    EXPECT_EQ(os.str(), "{  }");
}

TEST(SetRoundTrip, OutputThenInput) {
    Set a(10);
    a.insert(1);
    a.insert(4);
    a.insert(9);
    std::stringstream ss;
    ss << a;
    Set b(10);
    ss >> b;
    EXPECT_TRUE(b.is_member(1));
    EXPECT_TRUE(b.is_member(4));
    EXPECT_TRUE(b.is_member(9));
    EXPECT_FALSE(b.is_member(2));
}

//TEST(Sieve, UpTo30) {
//    int n = 30;
//    Set primes(n);
//    Set composite(n);
//    for (int i = 2; i * i <= n; ++i)
//        if (!composite.is_member(i))
//            for (int j = i * i; j <= n; j += i)
//                composite += j;
//    for (int i = 2; i <= n; ++i)
//        if (!composite.is_member(i))
//            primes += i;
//
//    EXPECT_TRUE(primes.is_member(2));
//    EXPECT_TRUE(primes.is_member(3));
//    EXPECT_TRUE(primes.is_member(5));
//    EXPECT_TRUE(primes.is_member(29));
//    EXPECT_FALSE(primes.is_member(4));
//    EXPECT_FALSE(primes.is_member(30));
//}