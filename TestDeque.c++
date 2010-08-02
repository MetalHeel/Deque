// ----------------------------
// projects/deque/TestDeque.c++
// Copyright (C) 2010
// Glenn P. Downing
// ----------------------------

/*
To test the program:
% g++ -ansi -pedantic -lcppunit -ldl -Wall TestDeque.c++ -o TestDeque.app
% valgrind TestDeque.app >& TestDeque.out
*/

// --------
// includes
// --------

#include <algorithm> // copy, count, fill, reverse
#include <deque> // deque
#include <memory> // allocator

#include "cppunit/extensions/HelperMacros.h" // CPPUNIT_TEST, CPPUNIT_TEST_SUITE, CPPUNIT_TEST_SUITE_END
#include "cppunit/TestFixture.h" // TestFixture
#include "cppunit/TestSuite.h" // TestSuite
#include "cppunit/TextTestRunner.h" // TestRunner

#include "Deque.h"

// ---------
// TestDeque
// ---------

template <typename C>
struct TestDeque : CppUnit::TestFixture {
    // ----------------
    // test_constructor
    // ----------------

    void test_constructor () {
        const C x;
        const C y(10);
        const C z(50, 2);
        const C t = z;}

    void test_constructor2 () {
        const C x(8, 3);
        const C y(13, 7);
        const C z(666, 2);
        const C t = z;}

    // -------------
    // test_equality
    // -------------

    void test_equality () {
        const C x(10, 2);
        const C y(10, 2);
        assert(x == y);}

    void test_equality2 () {
        const C x(50, 2);
        const C y(50, 2);
        assert(x == y);
        assert(!(x != y));}

    void test_equality3 () {
              C x(45, 2);
        x.push_back(2);
        x.push_back(2);
        x.push_back(2);
        x.push_back(2);
        x.push_back(2);
        const C y(50, 2);
        assert(x == y);
        assert(!(x != y));}

    void test_equality4 () {
        const C y(666, 2);
        const C z(666, 2);
        assert(y == z);}

    // ---------------
    // test_comparison
    // ---------------

    void test_comparison () {
        const C x(10, 2);
        const C y(20, 3);
        assert(x < y);
        assert(!(x >= y));
        assert(y > x);
        assert(!(y <= x));}

    void test_comparison2 () {
        const C x(10, 3);
        const C y(20, 3);
        assert(x < y);
        assert(!(x >= y));
        assert(y > x);
        assert(!(y <= x));}

    void test_comparison3 () {
        const C x(20, 2);
        const C y(20, 3);
        assert(x < y);
        assert(!(x >= y));
        assert(y > x);
        assert(!(y <= x));}

    // ---------------
    // test_assignment
    // ---------------

    void test_assignment () {
              C x(10, 2);
        const C y(20, 3);
        x = y;
        assert(x == y);}

    void test_assignment2 () {
              C x(20, 2);
        const C y(10, 3);
        x = y;
        assert(x == y);}

    void test_assignment3 () {
              C x(10, 2);
              x.pop_back();
              x.pop_back();
        const C y(10, 3);
        x = y;
        assert(x == y);}

    // --------------
    // test_subscript
    // --------------

    void test_subscript () {
              C x(10, 2);
        const C y(10, 2);
        typename C::reference v = x[0];
        typename C::const_reference w = y[0];
        assert(v == w);}

    void test_subscript2 () {
              C x(10, 2);
        const C y(10, 2);
        typename C::reference v = x[3];
        typename C::const_reference w = y[8];
        assert(v == w);}

    void test_subscript3 () {
        C x(10);
        C y(10);
        x.push_back(1);
        x.push_back(1);
        x.push_back(1);
        x.push_back(1);
        x.push_back(1);
        y.push_back(1);
        y.push_back(1);
        y.push_back(1);
        y.push_back(1);
        y.push_back(1);
        typename C::reference v = x[2];
        typename C::const_reference w = y[2];
        assert(v == w);}

    void test_subscript4 () {
              C x(10, 2);
        x.insert(x.begin() + 4, 0);
        assert(x[4] == 0);}

    // -------
    // test_at
    // -------

    void test_at () {
              C x(10, 2);
        const C y(10, 2);
        typename C::reference v = x.at(0);
        typename C::const_reference w = y.at(0);
        assert(v == w);}

    // ---------
    // test_back
    // ---------

    void test_back () {
              C x(10, 2);
        const C y(10, 2);
        typename C::reference v = x.back();
        typename C::const_reference w = y.back();
        assert(v == w);}

    // ----------
    // test_begin
    // ----------

    void test_begin () {
              C x(10, 2);
        const C y(10, 2);
        typename C::iterator p = x.begin();
        typename C::const_iterator q = y.begin();
        assert(*p == *q);}

    // ----------
    // test_clear
    // ----------

    void test_clear () {
        C x(10, 2);
        x.clear();
        assert(x.empty());}

    // ----------
    // test_empty
    // ----------

    void test_empty () {
        const C x;
        const bool b = x.empty();
        assert(b);}

    // --------
    // test_end
    // --------

    void test_end () {
              C x(10, 2);
        const C y(10, 2);
        typename C::iterator p = x.end();
        typename C::const_iterator q = y.end();
        assert(*(p-1) == *(q-1));}

    // ----------
    // test_erase
    // ----------

    void test_erase () {
        C x(10, 2);
        typename C::iterator p = x.erase(x.begin());
        assert(p == x.begin());}

    void test_erase2 () {
        C x(10, 2);
        C y(9, 2);
        typename C::iterator p = x.erase(x.begin());
        assert(p == x.begin());
        assert(x == y);}

    // ----------
    // test_front
    // ----------

    void test_front () {
              C x(10, 2);
        const C y(10, 2);
        typename C::reference v = x.front();
        typename C::const_reference w = y.front();
        assert(v == w);}

    // ---------------
    // test_push_front
    // ---------------

    void test_push_front () {
        C x(10, 2);
        x.push_front(3);}

    // -----------
    // test_insert
    // -----------

    void test_insert () {
        C x(10, 2);
        typename C::iterator p = x.insert(x.begin(), 3);
        assert(p == x.begin());}
        
    void test_insert2 () {
        C x(10, 2);
        typename C::iterator p = x.insert(x.begin()+1, 3);
        assert(p == x.begin()+1);}

    // -------------
    // test_pop_back
    // -------------

    void test_pop_back () {
        C x(10, 2);
        x.pop_back();}

    // --------------
    // test_push_back
    // --------------

    void test_push_back () {
        C x(10, 2);
        x.push_back(3);}

    // -----------
    // test_resize
    // -----------

    void test_resize () {
        C x(10, 2);
        x.resize(20);
        x.resize(30, 3);}

    // ---------
    // test_size
    // ---------

    void test_size () {
        const C x;
        assert(x.size() == 0);}

    // ---------
    // test_swap
    // ---------

    void test_swap () {
        C x(10, 2);
        C y(20, 3);
        x.swap(y);}

    // -------------
    // test_iterator
    // -------------

    void test_iterator () {
        C x(10, 2);
        typename C::iterator b = x.begin();
        assert(b == x.begin());
        typename C::reference v = *b;
        ++b;
        b += 2;
        --b;
        b -= 2;
        typename C::reference w = *b;
        assert(v == w);}

    // -------------------
    // test_const_iterator
    // -------------------

    void test_const_iterator () {
        const C x(10, 2);
        typename C::const_iterator b = x.begin();
        assert(b == x.begin());
        typename C::const_reference v = *b;
        ++b;
        b += 2;
        --b;
        b -= 2;
        typename C::const_reference w = *b;
        assert(v == w);}

    void test_const_iterator2 () {
        const C x(10, 2);
        typename C::const_iterator b = x.begin();
        typename C::const_iterator e = x.end();
        assert(b + x.size() == e);}

    // ---------------
    // test_algorithms
    // ---------------

    void test_algorithms () {
              C x(10, 2);
        const C y(10, 2);
        assert(std::count(y.begin(), y.end(), 3) == 0);
        std::copy(y.begin(), y.end(), x.begin());
        std::fill(x.begin(), x.end(), 2);
        std::reverse(x.begin(), x.end());}

    // -----
    // suite
    // -----

    CPPUNIT_TEST_SUITE(TestDeque);
    CPPUNIT_TEST(test_constructor);
    CPPUNIT_TEST(test_constructor2);
    CPPUNIT_TEST(test_equality);
    CPPUNIT_TEST(test_equality2);
    CPPUNIT_TEST(test_equality3);
    CPPUNIT_TEST(test_equality4);
    CPPUNIT_TEST(test_comparison);
    CPPUNIT_TEST(test_comparison2);
    CPPUNIT_TEST(test_comparison3);
    CPPUNIT_TEST(test_assignment);
    CPPUNIT_TEST(test_assignment2);
    CPPUNIT_TEST(test_assignment3);
    CPPUNIT_TEST(test_subscript);
    CPPUNIT_TEST(test_subscript2);
    CPPUNIT_TEST(test_subscript3);
    CPPUNIT_TEST(test_at);
    CPPUNIT_TEST(test_back);
    CPPUNIT_TEST(test_begin);
    CPPUNIT_TEST(test_clear);
    CPPUNIT_TEST(test_empty);
    CPPUNIT_TEST(test_end);
    CPPUNIT_TEST(test_erase);
    CPPUNIT_TEST(test_erase2);
    CPPUNIT_TEST(test_front);
    CPPUNIT_TEST(test_push_front);
    CPPUNIT_TEST(test_insert);
    CPPUNIT_TEST(test_pop_back);
    CPPUNIT_TEST(test_push_back);
    CPPUNIT_TEST(test_resize);
    CPPUNIT_TEST(test_size);
    CPPUNIT_TEST(test_swap);
    CPPUNIT_TEST(test_iterator);
    CPPUNIT_TEST(test_const_iterator);
    CPPUNIT_TEST(test_const_iterator2);
    CPPUNIT_TEST(test_algorithms);
    CPPUNIT_TEST_SUITE_END();};

// ----
// main
// ----

int main () {
    using namespace std;
    ios_base::sync_with_stdio(false); // turn off synchronization with C I/O
    cout << "TestDeque.c++" << endl;

    CppUnit::TextTestRunner tr;
    tr.addTest(TestDeque< std::deque<int> >::suite());
    tr.addTest(TestDeque< std::deque<int, std::allocator<int> > >::suite());
    tr.addTest(TestDeque< Deque<int> >::suite());
    tr.addTest(TestDeque< Deque<int, std::allocator<int> > >::suite());
    tr.run();

    cout << "Done." << endl;
    return 0;}
