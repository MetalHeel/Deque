// ----------------------
// projects/deque/Deque.h
// Copyright (C) 2010
// Glenn P. Downing
// ----------------------

#ifndef Deque_h
#define Deque_h

// --------
// includes
// --------

#include <algorithm> // equal, lexicographical_compare
#include <cassert>   // assert
#include <iterator>  // iterator, bidirectional_iterator_tag
#include <memory>    // allocator
#include <stdexcept> // out_of_range
#include <utility>   // !=, <=, >, >=

// -----
// using
// -----

using std::rel_ops::operator!=;
using std::rel_ops::operator<=;
using std::rel_ops::operator>;
using std::rel_ops::operator>=;

using namespace std;

// -------
// destroy
// -------

template <typename A, typename BI>
BI destroy (A& a, BI b, BI e) {
    while (b != e) {
        --e;
        a.destroy(&*e);}
    return b;}

// ------------------
// uninitialized_copy
// ------------------

template <typename A, typename II, typename BI>
BI uninitialized_copy (A& a, II b, II e, BI x) {
    BI p = x;
    try {
        while (b != e) {
            a.construct(&*x, *b);
            ++b;
            ++x;}}
    catch (...) {
        destroy(a, p, x);
        throw;}
    return x;}

// ------------------
// uninitialized_fill
// ------------------

template <typename A, typename BI, typename U>
BI uninitialized_fill (A& a, BI b, BI e, const U& v) {
    BI p = b;
    try {
        while (b != e) {
            a.construct(&*b, v);
            ++b;}}
    catch (...) {
        destroy(a, p, b);
        throw;}
    return e;}

// -----
// Deque
// -----

template < typename T, typename A = std::allocator<T> >
class Deque {
    public:
        // --------
        // typedefs
        // --------

        typedef A                                        allocator_type;
        typedef typename allocator_type::value_type      value_type;

        typedef typename allocator_type::size_type       size_type;
        typedef typename allocator_type::difference_type difference_type;

        typedef typename allocator_type::pointer         pointer;
        typedef typename allocator_type::const_pointer   const_pointer;

        typedef typename allocator_type::reference       reference;
        typedef typename allocator_type::const_reference const_reference;

    public:
        // -----------
        // operator ==
        // -----------

        /**
         * <your documentation>
         */
        friend bool operator == (const Deque& lhs, const Deque& rhs) {
            if(!lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) &&
              !lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end())){
                  return true;
            }
            return false;}

        // ----------
        // operator <
        // ----------

        /**
         * <your documentation>
         */
        friend bool operator < (const Deque& lhs, const Deque& rhs) {
            return lexicographical_compare(lhs._front, lhs._back, rhs._front, rhs._back);}

    private:
        // ----
        // data
        // ----

        allocator_type _a;
        int ROWS;
        //const int COLUMNS;
        T* container;
        unsigned int _size;
        unsigned int tsize;
        T* _front;
        T* _back;

    private:
        // -----
        // valid
        // -----

        bool valid () const {
            return (!_front && !_back && !(container + tsize)) || (_front <= _back) && (_back <= (container + tsize));}

    public:
        // --------
        // iterator
        // --------

        class iterator {
            public:
                // --------
                // typedefs
                // --------

                typedef std::bidirectional_iterator_tag iterator_category;
                typedef typename Deque::value_type      value_type;
                typedef typename Deque::difference_type difference_type;
                typedef typename Deque::pointer         pointer;
                typedef typename Deque::reference       reference;

            public:
                // -----------
                // operator ==
                // -----------

                /**
                 * <your documentation>
                 */
                friend bool operator == (const iterator& lhs, const iterator& rhs) {
                    return lhs.p == rhs.p;}

                // ----------
                // operator +
                // ----------

                /**
                 * <your documentation>
                 */
                friend iterator operator + (iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * <your documentation>
                 */
                friend iterator operator - (iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                pointer p;

            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    return !p || (sizeof(*p) == sizeof(T));}

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * <your documentation>
                 */
                iterator (T* v) : p(v){
                    assert(valid());}

                // Default copy, destructor, and copy assignment.
                // iterator (const iterator&);
                // ~iterator ();
                // iterator& operator = (const iterator&);

                // ----------
                // operator *
                // ----------

                /**
                 * <your documentation>
                 */
                reference operator * () const {
                    return *p;}

                // -----------
                // operator ->
                // -----------

                /**
                 * <your documentation>
                 */
                pointer operator -> () const {
                    return &**this;}

                // -----------
                // operator ++
                // -----------

                /**
                 * <your documentation>
                 */
                iterator& operator ++ () {
                    ++p;
                    assert(valid());
                    return *this;}

                /**
                 * <your documentation>
                 */
                iterator operator ++ (int) {
                    iterator x = *this;
                    ++(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator --
                // -----------

                /**
                 * <your documentation>
                 */
                iterator& operator -- () {
                    --p;
                    assert(valid());
                    return *this;}

                /**
                 * <your documentation>
                 */
                iterator operator -- (int) {
                    iterator x = *this;
                    --(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator +=
                // -----------

                /**
                 * <your documentation>
                 */
                    iterator& operator += (difference_type d) {
                    while(d > 0){
                        ++p;
                        --d;
                    }
                    assert(valid());
                    return *this;}

                // -----------
                // operator -=
                // -----------

                /**
                 * <your documentation>
                 */
                iterator& operator -= (difference_type d) {
                    p -= d;
                    assert(valid());
                    return *this;}};

    public:
        // --------------
        // const_iterator
        // --------------

        class const_iterator {
            public:
                // --------
                // typedefs
                // --------

                typedef std::bidirectional_iterator_tag iterator_category;
                typedef typename Deque::value_type      value_type;
                typedef typename Deque::difference_type difference_type;
                typedef typename Deque::const_pointer   pointer;
                typedef typename Deque::const_reference reference;

            public:
                // -----------
                // operator ==
                // -----------

                /**
                 * <your documentation>
                 */
                friend bool operator == (const const_iterator& lhs, const const_iterator& rhs) {
                  return lhs.c_ptr == rhs.c_ptr;}

                // ----------
                // operator +
                // ----------

                /**
                 * <your documentation>
                 */
                friend const_iterator operator + (const_iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * <your documentation>
                 */
                friend const_iterator operator - (const_iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                pointer c_ptr;

            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    return !c_ptr || (sizeof(*c_ptr) == sizeof(T));}

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator (T* v) : c_ptr(v){
                    assert(valid());}

                // Default copy, destructor, and copy assignment.
                // const_iterator (const const_iterator&);
                // ~const_iterator ();
                // const_iterator& operator = (const const_iterator&);

                // ----------
                // operator *
                // ----------

                /**
                 * <your documentation>
                 */
                reference operator * () const {
                    return *c_ptr;}

                // -----------
                // operator ->
                // -----------

                /**
                 * <your documentation>
                 */
                pointer operator -> () const {
                    return &**this;}

                // -----------
                // operator ++
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator ++ () {
                    ++c_ptr;
                    assert(valid());
                    return *this;}

                /**
                 * <your documentation>
                 */
                const_iterator operator ++ (int) {
                    const_iterator x = *this;
                    ++(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator --
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator -- () {
                    --c_ptr;
                    assert(valid());
                    return *this;}

                /**
                 * <your documentation>
                 */
                const_iterator operator -- (int) {
                    const_iterator x = *this;
                    --(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator +=
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator += (difference_type d) {
                    while(d > 0){
                        ++c_ptr;
                        --d;
                    }
                    assert(valid());
                    return *this;}

                // -----------
                // operator -=
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator -= (difference_type d) {
                     c_ptr -= d;
                    assert(valid());
                    return *this;}};

    public:
        // ------------
        // constructors
        // ------------

        /**
         * <your documentation>
         */
        explicit Deque (const allocator_type& a = allocator_type()) {
            _a = a;
            ROWS = 10;
            tsize = ROWS;
            container = _a.allocate(ROWS);
            _size = 0;
            _front = container + (ROWS / 2);
            _back = _front;
            assert(valid());}

        /**
         * <your documentation>
         */
        explicit Deque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type()) {
            _a = a;
            _size = s;
            ROWS = 10;
            tsize = ROWS * (((_size / ROWS) + 1) * 2);
            if(s > 5)
                container = _a.allocate(tsize);
            else
                container = _a.allocate(ROWS);

            _front = container + (tsize / 2);
            _back = _front + _size;

            uninitialized_fill(_a, _front, _back, v);
/*
            for(int i = 0; i < 5; i++) {
                if(s == 0)
                    break;
                *our_back = v;
                our_back++;
                s--;
            }

            int tmp = 1;
            while(s > 10) {
                container[(ROWS / 2) + tmp] = a.allocate(COLUMNS);
                fill(container[(ROWS / 2) + tmp], container[(ROWS / 2) + tmp] + COLUMNS, v);
                s -= 10;
                tmp++;
            }

            our_back = container[(ROWS / 2) + tmp][0];

            while(s > 0) {
                *our_back = v;
                our_back++;
                s--;
            }
*/
            assert(valid());}

        /**
         * <your documentation>
         */
        Deque (const Deque& that) {
            _a = that._a;
            ROWS = that.ROWS;
            container = _a.allocate(that.tsize);
            tsize = that.tsize;
            int offset = that._front - that.container;
            _front = container + offset;
            _back = uninitialized_copy(_a, that._front, that._back, _front);
            _size = that._size;
            assert(valid());}

        // ----------
        // destructor
        // ----------

        /**
         * <your documentation>
         */
        ~Deque () {
            destroy(_a, _front, _back);
            _a.deallocate(container, tsize);
            assert(valid());}

        // ----------
        // operator =
        // ----------

        /**
         * <your documentation>
         */
        Deque& operator = (const Deque& rhs) {
            _a = rhs._a;
            ROWS = rhs.ROWS;
            container = _a.allocate(rhs.tsize);
            tsize = rhs.tsize;
            int offset = rhs._front - rhs.container;
            _front = container + offset;
            _back = uninitialized_copy(_a, rhs._front, rhs._back, _front);
            _size = rhs._size;
            assert(valid());
            return *this;}

        // -----------
        // operator []
        // -----------

        /**
         * <your documentation>
         */
        reference operator [] (size_type index) {
            return *(_front + index);}

        /**
         * <your documentation>
         */
        const_reference operator [] (size_type index) const {
            return const_cast<Deque*>(this)->operator[](index);}

        // --
        // at
        // --

        /**
         * <your documentation>
         */
        reference at (size_type index) {
            return *(_front + index);}

        /**
         * <your documentation>
         */
        const_reference at (size_type index) const {
            return const_cast<Deque*>(this)->at(index);}

        // ----
        // back
        // ----

        /**
         * <your documentation>
         */
        reference back () {
            return *(_back-1);}

        /**
         * <your documentation>
         */
        const_reference back () const {
            return const_cast<Deque*>(this)->back();}

        // -----
        // begin
        // -----

        /**
         * <your documentation>
         */
        iterator begin () {
            return iterator(_front);}

        /**
         * <your documentation>
         */
        const_iterator begin () const {
            return const_iterator(_front);}

        // -----
        // clear
        // -----

        /**
         * <your documentation>
         */
        void clear () {
            destroy(_a, _front, _back);
            _front = container + (tsize / 2);
            _back = _front;
            _size = 0;
            assert(valid());}

        // -----
        // empty
        // -----

        /**
         * <your documentation>
         */
        bool empty () const {
            return !size();}

        // ---
        // end
        // ---

        /**
         * <your documentation>
         */
        iterator end () {
            return iterator(_back);}

        /**
         * <your documentation>
         */
        const_iterator end () const {
            return const_iterator(_back);}

        // -----
        // erase
        // -----

        /**
         * <your documentation>
         */
        iterator erase (iterator it) {
            _a.destroy(&*it);
            if(it == _front)
            {
                ++it;
                ++_front;
            }
            else if(it == _back - 1)
            {
                --it;
                --_back;
            }
            else
            {
                iterator temp = it;
                while(!(temp == _back - 1))
                {
                    *temp = *(temp + 1);
                    temp++;
                }
                _a.destroy(&*temp);
                _back--;
            }
            _size--;
            assert(valid());
            return it;}

        // -----
        // front
        // -----

        /**
         * <your documentation>
         */
        reference front () {
            return *_front;}

        /**
         * <your documentation>
         */
        const_reference front () const {
            return const_cast<Deque*>(this)->front();}

        // ------
        // insert
        // ------

        /**
         * <your documentation>
         */
        iterator insert (iterator it, const_reference v) {
            iterator temp = it;
            if(_size == tsize)
            {
                resize(tsize * 2);
            }
            if(_front == it)
            {
                push_front(v);
                temp = it - 1;
            }
            else if(_back == it)
            {
                push_back(v);
                temp = it;
            } 
            else 
            {
                if(!(_back == (container + tsize))){
                    temp = _back;
                    while(!(temp == it)){
                        *temp = *(temp - 1);
                        --temp;
                    }
                    *temp = v;
                    ++_size;
                    ++_back;
                } else {
                    iterator temp = _front - 1;
                    while(!(temp == it)){
                        *temp = *(temp + 1);
                        ++temp;
                    }
                    *temp = v;
                    ++_size;
                    --_front;
                }
            }
            assert(valid());
            return iterator(&*temp);}

        // ---
        // pop
        // ---

        /**
         * <your documentation>
         */
        void pop_back () {
            _a.destroy(_back-1);
            --_back;
            --_size;
            assert(valid());}

        /**
         * <your documentation>
         */
        void pop_front () {
            _a.destroy(_front);
            ++_front;
            --_size;
            assert(valid());}

        // ----
        // push
        // ----

        /**
         * <your documentation>
         */
        void push_back (const_reference v) {
            if(_size == tsize)
            {
              resize(tsize * 2);
            }
            if(_back == (container + tsize))
            {
              iterator temp = _front - 1;
              while(!(temp == _back))
              {
                *temp = *(temp + 1);
                temp++;
              }
              *temp = v;
              _front--;
            }
            else
            {
              *(_back) = v;
              _back++;
            }
            _size++;
            assert(valid());}

        /**
         * <your documentation>
         */
        void push_front (const_reference v) {
            if(_size == tsize)
            {
                resize(tsize * 2);
            }
            if(_front == container)
            {
                iterator temp = _back;
                while(!(temp == _front))
                {
                    *temp = *(temp - 1);
                    temp--;
                }
                *temp = v;
                _back++;
            }
            else
            {
                *(_front - 1) = v;
                _front--;
            }
            _size++;
            assert(valid());}

        // ------
        // resize
        // ------

        /**
         * <your documentation>
         */
        void resize (size_type s, const_reference v = value_type()) {
            if(s == _size)
                return;
            else if(s < _size)
            {
                _back = destroy(_a, _front + s, _back);
                _size = s;
            }
            else if(s <= tsize)
            {
                _front = (container + ((tsize - _size)/2));
                _back = _front + s;
                _back = uninitialized_fill(_a, _front, _back, v);
                _size = s;
            }
            else
            {
                if(s > (2 * _size))
                {
                    Deque nDeque (s, v);
                    swap(nDeque);
                }
                else
                {
                    Deque nDeque (2 * _size, v);
                    swap(nDeque);
                }
            }
            assert(valid());}

        // ----
        // size
        // ----

        /**
         * <your documentation>
         */
        size_type size () const {
            return _size;}

        // ----
        // swap
        // ----

        /**
         * <your documentation>
         */
        void swap (Deque& that) {
          if (_a == that._a) {
            std::swap(_front, that._front);
            std::swap(_back, that._back);
            std::swap(container, that.container);
            int temp = tsize;
            tsize = that.tsize;
            that.tsize = temp;
            temp = _size;
            _size = that._size;
            that._size = temp;}
          else {
            Deque x(*this);
            *this = that;
            that = x;}
            assert(valid());}};

#endif // Deque_h
