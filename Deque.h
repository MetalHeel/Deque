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
         * returns true if lhs is equal to rhs.
         */
        friend bool operator == (const Deque& lhs, const Deque& rhs) {
            return (lhs.size() == rhs.size()) && !lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) &&
                !lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());}

        // ----------
        // operator <
        // ----------

        /**
         * returns true if lhs is less than rhs.
         */
        friend bool operator < (const Deque& lhs, const Deque& rhs) {
            return lexicographical_compare(lhs._front, lhs._back, rhs._front, rhs._back);}

    private:
        // ----
        // data
        // ----

        allocator_type _a;
        typename A::template rebind<T*>::other _a2;
        int ROWS;
        int COLUMNS;
        T** container;
        unsigned int _size;
        unsigned int tsize;
        T* _front;
        T* _back;
        T** startRow;
        T** ofront;
        T** oback;

    private:
        // -----
        // valid
        // -----

      bool valid () const {
          int rowOffset = _size/10;
          if(_front == *startRow && _size%10 == 0 && _size > 0)
          {
              rowOffset -= 1;
          }
          return  (!_front && !_back && !ofront && !oback) || ((_front < *startRow + COLUMNS) && (_back <= *(startRow + rowOffset) + COLUMNS));}

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
                 * returns true is lhs is equal to rhs.
                 */
                friend bool operator == (const iterator& lhs, const iterator& rhs) {
                    return lhs.p == rhs.p;}

                // ----------
                // operator +
                // ----------

                /**
                 * increments lhs by rhs.
                 */
                friend iterator operator + (iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * decrements lhs by rhs.
                 */
                friend iterator operator - (iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                pointer p;
                T** row;
                int index;
                int size;
                pointer end;
                pointer front;

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
                 * constructs an iterator with value v, row r, index i, size my_size, and end and front.
                 */
                iterator (T* v, T** r, int i, int my_size, T* my_end, T* my_front) : p(v){
                    row = r;
                    index = i;
                    size = my_size;
                    end = my_end;
                    front = my_front;
                    assert(valid());}

                // Default copy, destructor, and copy assignment.
                // iterator (const iterator&);
                // ~iterator ();
                // iterator& operator = (const iterator&);

                pointer get_pointer()
                {
                    return p;
                }

                // ----------
                // operator *
                // ----------

                /**
                 * dereferences iterator.
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
                 * increments iterator by one (post-increment).
                 */
                iterator& operator ++ () {
                    if((p+1) == end)
                    {
                        p = end;
                    }
                    else if((index == 9 && p != end) ||
                             (index == 10 && p == end))
                    {
                        row = row + 1;
                        if(p == end)
                        {
                            index = 1;
                        }
                        else
                        {
                            index = 0;
                        }
                        p = *row;
                    }
                    else
                    {
                        ++p;
                        index++;
                    }
                    assert(valid());
                    return *this;}

                /**
                 * increments iterator by one (pre-increment).
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
                 * decrements iterator by one (post-decrement).
                 */
                iterator& operator -- () {
                    if(index == 0)
                    {
                        row = row - 1;
                        index = 9;
                        p = *row + 9;
                    }
                    else
                    {
                        --p;
                        index--;
                    }
                    assert(valid());
                    return *this;}

                /**
                 * decrements iterator by one (pre-decrement).
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
                 * increments this by d.
                 */
                    iterator& operator += (difference_type d) {
                      if((index + d > 9 && p != end) ||
                          (p == end && index + d > 10))
                      {
                        int temp = d - (10 - index);
                        if(!(d <= 10 && *row + d == end))
                          row++;
                        else
                          temp = d;
                        while(temp > 9)
                        {
                          if(*row + temp == end){
                            break;
                          }
                          temp -= 10;
                          row++;
                        }
                        index = temp;
                        p = *row + temp;
                      }
                      else
                      {
                        p += d;
                        index += d;
                      }
                      assert(valid());
                      return *this;}

                // -----------
                // operator -=
                // -----------

                /**
                 * decrements this by d.
                 */
                iterator& operator -= (difference_type d) {
                    if(index - d < 0)
                    {
                        int temp = d - (index + 1);
                        row--;
                        while(temp > 9)
                        {
                            temp -= 10;
                            row--;
                        }
                        index = 9 - temp;
                        p = *row + index;
                    }
                    else
                    {
                        p -= d;
                        index -= d;
                    }
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
                 * returns true is lhs is equal to rhs.
                 */
                friend bool operator == (const const_iterator& lhs, const const_iterator& rhs) {
                  return lhs.c_ptr == rhs.c_ptr;}

                // ----------
                // operator +
                // ----------

                /**
                 * increments lhs by rhs.
                 */
                friend const_iterator operator + (const_iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * decrements lhs by rhs.
                 */
                friend const_iterator operator - (const_iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                pointer c_ptr;
                T** row;
                int index;
                int size;
                pointer end;
                pointer front;

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
                 * constructs an iterator with value v, row r, index i, size my_size, and end and front.
                 */
                const_iterator (T* v, T** r, int i, int the_size, T* the_end, T* the_front) : c_ptr(v){
                    row = r;
                    index = i;
                    size = the_size;
                    end = the_end;
                    front = the_front;
                    assert(valid());}

                // Default copy, destructor, and copy assignment.
                // const_iterator (const const_iterator&);
                // ~const_iterator ();
                // const_iterator& operator = (const const_iterator&);


                pointer get_pointer()
                {
                    return c_ptr;
                }

                // ----------
                // operator *
                // ----------

                /**
                 * dereferences this.
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
                 * increments this by one (post-increment).
                 */
                const_iterator& operator ++ () {
                    if((c_ptr + 1) == end)
                    {
                        c_ptr = end;
                    }
                    else if((index == 9 && c_ptr != end) ||
                           (index == 10 && c_ptr == end))
                    {
                        row = row + 1;
                        if(c_ptr == end)
                        {
                          index = 1;
                        }
                        else
                        {
                          index = 0;
                        }
                        c_ptr = *row;
                    }
                    else
                    {
                        ++c_ptr;
                        index++;
                    }
                    assert(valid());
                    return *this;}

                /**
                 * increments this by one (pre-increment).
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
                 * decrements this by one (post-increment).
                 */
                const_iterator& operator -- () {
                    if(index == 0)
                    {
                        row = row - 1;
                        index = 9;
                        c_ptr = *row + 9;
                    }
                    else
                    {
                        --c_ptr;
                        index--;
                    }
                    assert(valid());
                    return *this;}

                /**
                 * decrements this by one (pre-increment).
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
                 * increments this by d.
                 */
                const_iterator& operator += (difference_type d) {
                    if((index + d > 9 && c_ptr != end) ||
                            (c_ptr == end && index + d > 10))
                    {
                        int temp = d - (10 - index);
                        if(!(d <= 10 && *row + d == end))
                            row++;
                        else
                            temp = d;
                        while(temp > 9)
                        {
                            if(*row + temp == end){
                                break;
                            }
                            temp -= 10;
                            row++;
                        }
                        index = temp;
                        c_ptr = *row + temp;
                    }
                    else
                    {
                        c_ptr += d;
                        index += d;
                    }
                    assert(valid());
                    return *this;}

                // -----------
                // operator -=
                // -----------

                /**
                 * decrements this by d.
                 */
                const_iterator& operator -= (difference_type d) {
                    if(index - d < 0)
                    {
                        int temp = d - (index + 1);
                        row--;
                        while(temp > 9)
                        {
                            temp -= 10;
                            row--;
                        }
                        index = 9 - temp;
                        c_ptr = *row + index;
                    }
                    else
                    {
                        c_ptr -= d;
                        index -= d;
                    }
                    assert(valid());
                    return *this;}};

    private:

        Deque (const Deque& that, size_type c){
            _a = that._a;
            _a2 = that._a2;
            _size = that._size;
            tsize = c;
            while(tsize%10 != 0)
                tsize++;
            ROWS = tsize/10;
            int thatRow = 0;
            int hml = _size;
            if(ROWS == 0)
                ROWS = 1;
            COLUMNS = that.COLUMNS;
            container = _a2.allocate(ROWS);
            for(int i = 0; i < ROWS; i++)
              *(container + i) = _a.allocate(COLUMNS);
            if(that._size <= 10)
            {
                uninitialized_copy(_a, that.begin(), that.end(), *container);
            }
            else
            {
                uninitialized_copy(_a, that.begin(), that.begin() + 10, *container);
                const_iterator it = that.begin() + 10;
                hml = _size - 10;
                thatRow = 1;
                while(hml > 10)
                {
                    uninitialized_copy(_a, it, it+10, *(container + thatRow));
                    it += 10;
                    hml -= 10;
                    thatRow++;
                }
                uninitialized_copy(_a, it, that.end(), *(container + thatRow));
            }
            _front = *container;
            _back = *(container + thatRow) + hml;
            startRow = container;
            ofront = container;
            oback = ofront + ROWS;
            assert(valid());
        }

    public:
        // ------------
        // constructors
        // ------------

        /**
         * default constructor.
         */
        explicit Deque (const allocator_type& a = allocator_type()) {
            _a = a;
            ROWS = 10;
            COLUMNS = 10;
            tsize = 0;
            container = _a2.allocate(ROWS);
            _size = 0;
            _front = 0;
            _back = _front;
            startRow = container;
            ofront = container;
            oback = ofront + ROWS;
            assert(valid());}

        /**
         * constructor with specifications for size, value, and allocator.
         */
        explicit Deque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type()) {
            _a = a;
            _size = s;
            tsize = s;
            while(tsize % 10 != 0)
                tsize++;
            ROWS = tsize / 10;
            COLUMNS = 10;
            container = _a2.allocate(ROWS);
            for(int i = 0; i < ROWS; i++)
                *(container + i) = _a.allocate(COLUMNS);
            int hml = _size;
            if(_size >= 10){
                int start = (tsize - _size) / 2;
                uninitialized_fill(_a, *container + start, *container + COLUMNS, v);
                hml = hml - (COLUMNS - start);
                int counter = 1;
                while(hml > 10)
                {
                    uninitialized_fill(_a, *(container + counter), *(container + counter) + COLUMNS, v);
                    hml -= 10;
                    counter++;
                }
                if(hml > 0){
                    uninitialized_fill(_a, *(container + counter), *(container + counter) + hml, v);
                }
                _front = *container + start;
                if(hml == 0)
                    hml = 10;
                _back = *(container + (ROWS - 1)) + hml;
            }
            else
            {
                uninitialized_fill(_a, *container, *container + hml, v);
                _front = *container;
                _back = *container + hml;
            }
            startRow = container;
            ofront = container;
            oback = ofront + ROWS;
            assert(valid());}

        /**
         * copy constructor.
         */
        Deque (const Deque& that) {
            _a = that._a;
            _a2 = that._a2;
            _size = that._size;
            tsize = _size;
            while(tsize % 10 != 0)
              tsize++;
            ROWS = tsize/10;
            if(ROWS == 0)
                ROWS = 1;
            COLUMNS = that.COLUMNS;
            container = _a2.allocate(ROWS);
            for(int i = 0; i < ROWS; i++)
              *(container + i) = _a.allocate(COLUMNS);
            int hml = _size;
            if(_size >= 10)
            {
                int start = (tsize - _size) / 2;
                uninitialized_copy(_a, that._front, *that.startRow + COLUMNS, *container + start);
                hml = hml - (COLUMNS - start);
                int counter = 0;
                while(hml > 10)
                {
                    counter++;
                    uninitialized_copy(_a, *(that.startRow + counter), *(that.startRow + counter) + COLUMNS, *(container + counter));
                    hml -= 10;
                }
                if(hml > 0){
                    counter++;
                    uninitialized_copy(_a, *(that.startRow + counter), that._back, *(container + counter));
                }
                _front = *container + start;
                _back = *(container + counter) + hml;
            }
            else
            {
                int start = (tsize - _size) / 2;
                uninitialized_copy(_a, that._front, that._back, *container + start);
                _front = *container + start;
                _back = *container + hml;
            }
            startRow = container;
            ofront = container;
            oback = ofront + ROWS;

            /*int offset = that._front - that.container;
            _front = container + offset;
            _back = uninitialized_copy(_a, that._front, that._back, _front);
            _size = that._size;*/
            assert(valid());}

        // ----------
        // destructor
        // ----------

        /**
         * destructor.
         */
            ~Deque () {
                if(_front)
                {
                    clear();
                    for(int i = 0; i < ROWS; i++)
                        _a.deallocate(*(container + i), COLUMNS);
                    _a2.deallocate(container, ROWS); 
                }
                /*assert(valid());*/}


        T* get_end(){
            return _back;
        }

        // ----------
        // operator =
        // ----------

        /**
         * assigns rhs to this.
         */
        Deque& operator = (const Deque& rhs) {
            if(this == &rhs){
                return *this;
            }
            else if (rhs.size() == size())
            {
                std::copy(rhs.begin(), rhs.end(), begin());
            }
            else if (rhs.size() < size())
            {
                std::copy(rhs.begin(), rhs.end(), begin());
                resize(rhs.size());
            }
            else if (rhs.size() <= tsize)
            {
                std::copy(rhs.begin(), rhs.end(), begin());
                iterator it = uninitialized_copy(_a, rhs.begin() + size(), rhs.end(), end());
                _size = rhs.size();
                _back = (it-1).get_pointer();
                _back++;
            }
            else
            {
                clear();
                if(rhs.size() > tsize)
                {
                    Deque<T> x(*this, rhs.size());
                    swap(x);
                }
                iterator it = uninitialized_copy(_a, rhs.begin(), rhs.end(), begin());
                _size = rhs.size();
                _back = (it-1).get_pointer();
                _back++;
            }

                /*---OLD CODE---
                _a = rhs._a;
                ROWS = rhs.ROWS;
                container = _a2.allocate(rhs.tsize);
                tsize = rhs.tsize;
                int offset = rhs._front - rhs.container;
                _front = container + offset;
                _back = uninitialized_copy(_a, rhs._front, rhs._back, _front);
                _size = rhs._size;
                -----------------*/
            assert(valid());
            return *this;}

        // -----------
        // operator []
        // -----------

        /**
         * indexes this based on index.
         */
        reference operator [] (size_type index) {
            return *(begin() + index);}

        /**
         * indexes this based on index.
         */
        const_reference operator [] (size_type index) const {
            return const_cast<Deque*>(this)->operator[](index);}

        // --
        // at
        // --

        /**
         * returns the item at index.
         */
        reference at (size_type index) {
            return *(begin() + index);}

        /**
         * returns the item at index.
         */
        const_reference at (size_type index) const {
            return const_cast<Deque*>(this)->at(index);}

        // ----
        // back
        // ----

        /**
         * returns the very last element.
         */
        reference back () {
            return *(end()-1);}

        /**
         * returns the very last element.
         */
        const_reference back () const {
            return const_cast<Deque*>(this)->back();}

        // -----
        // begin
        // -----

        /**
         * returns the very first element.
         */
        iterator begin () {
            return iterator(_front, startRow, _front - *startRow, _size, _back, _front);}

        /**
         * returns the very first element.
         */
        const_iterator begin () const {
            return const_iterator(_front, startRow, _front - *startRow, _size, _back, _front);}

        // -----
        // clear
        // -----

        /**
         * clears the deque (does not deallocate).
         */
        void clear () {
            destroy(_a, begin(), end());
            _front = *(container + (ROWS/2)) + (COLUMNS/2);
            _back = _front;
            startRow = container + (ROWS/2);
            _size = 0;
            assert(valid());}

        // -----
        // empty
        // -----

        /**
         * returns true if the deque is empty.
         */
        bool empty () const {
            return !size();}

        // ---
        // end
        // ---

        /**
         * returns an iterator pointing to one past the last element in the deque.
         */
        iterator end () {
            int rowOffset = 0;
            int index = 0;
            if((_front - *startRow) == 0 && (_size%10) == 0)
            {
                rowOffset = (_size / 10) - 1;
                index = 10;
            }
            else
            {
                rowOffset = _size / 10;
                index = _back - *(startRow + rowOffset);
            }
            return iterator(_back, startRow + rowOffset, index, _size, _back, _front);}

        /**
         * returns an iterator pointing to one past the last element in the deque.
         */
          const_iterator end () const {
            int rowOffset = 0;
            int index = 0;
            if((_front - *startRow) == 0 && (_size%10) == 0)
            {
                rowOffset = (_size / 10) - 1;
                index = 10;
            }
            else
            {
                rowOffset = _size / 10;
                index = _back - *(startRow + rowOffset);
            }
            return const_iterator(_back, startRow + rowOffset, index, _size, _back, _front);}

        // -----
        // erase
        // -----

        /**
         * erases the element pointed to by it.
         */
        iterator erase (iterator it) {
            if(it == begin())
            {
                _front = (begin() + 1).get_pointer();
                _size--;
                _a.destroy(&*it);
                it = begin();
            }



            /*----OLD CODE----
            _a.destroy(&*it);
            _size--;
            if(it == begin())
            {
                it++;
                _front = (begin() + 1).get_pointer();
            }
            else if(it == end() - 1)
            {
                it--;
                _back = (end() - 1).get_pointer();
            }
            else
            {
                iterator temp = it;
                while(!(temp == end() - 1))
                {
                    *temp = *(temp + 1);
                    temp++;
                }
                _a.destroy(&*temp);
                _back = (end()-1).get_pointer();
            }
            -----------------*/
            assert(valid());
            return it;}

        // -----
        // front
        // -----

        /**
         * returns the very first element of the deque.
         */
        reference front () {
            return *_front;}

        /**
         * returns the very first element of the deque.
         */
        const_reference front () const {
            return const_cast<Deque*>(this)->front();}

        // ------
        // insert
        // ------

        /**
         * inserts v at location pointed to by it.
         */
            iterator insert (iterator it, const_reference v) {
              int counter = 0;
            iterator temp = it;
            if(begin() == it)
            {
                push_front(v);
                temp = begin();
            }
            else if(end() == it)
            {
                push_back(v);
                temp = (end() - 1);
            }
            else
            {
                if(_size == tsize)
                {
                    while(begin() + counter != it){
                        counter++;
                    }
                    resize(tsize * 2);
                    it = begin() + counter;
                }
                if(!(end() == begin() + tsize)){
                    temp = end();
                    while(!(temp == it)){
                        *temp = *(temp - 1);
                        --temp;
                    }
                    *temp = v;
                    ++_size;
                    _back = (end() + 1).get_pointer();
                } else {
                  iterator temp = begin() - 1;
                    while(!(temp == it)){
                        *temp = *(temp + 1);
                        ++temp;
                    }
                    *temp = v;
                    ++_size;
                    _front = (begin() - 1).get_pointer();
                }
            }
            assert(valid());
            return temp;}

        // ---
        // pop
        // ---

        /**
         * removes the very last element.
         */
        void pop_back () {
            _a.destroy(_back-1);
            _back = (end()-1).get_pointer();
            --_size;
            assert(valid());}

        /**
         * removes the very first element.
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
         * inserts an element to the back of the deque.
         */
        void push_back (const_reference v) {
            resize(_size+1, v);
            assert(valid());}

        /**
         * inserts an element to the front of the deque.
         */
        void push_front (const_reference v) {
            if(_size == tsize)
            {
                resize(tsize * 2);
            }
            if(_front == *container)
            {
                iterator temp = end();
                while(!(temp == begin()))
                {
                    *temp = *(temp - 1);
                    temp--;
                }
                *temp = v;
                _back = (end() + 1).get_pointer();
            }
            else
            {
                *(begin() - 1) = v;
                _front = (begin() - 1).get_pointer();
            }
            _size++;
            assert(valid());}

        // ------
        // resize
        // ------

        /**
         * resizes the deque to size s and fills it with values v.
         */
        void resize (size_type s, const_reference v = value_type()) {
            if(s == _size)
                return;
            else if(s < _size)
            {
                iterator it = destroy(_a, begin() + s, end());
                _back = (it-1).get_pointer() + 1;
                _size = s;
            }
            else if(s <= tsize)
            {
                _size = s;
                int offset = (tsize - _size)/2;
                int rowOffset = 0;
                while(offset > 10)
                {
                    offset -= 10;
                    rowOffset++;
                }
                _front = *(container + rowOffset) + offset;
                iterator it = uninitialized_fill(_a, begin(), (begin() + _size), v);
                _back = (it-1).get_pointer() + 1;
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
         * returns the size of the deque.
         */
        size_type size () const {
            return _size;}

        // ----
        // swap
        // ----

        /**
         * swaps this for that.
         */
        void swap (Deque& that) {
          if (_a == that._a) {
            std::swap(_front, that._front);
            std::swap(_back, that._back);
            std::swap(container, that.container);
            std::swap(startRow, that.startRow);
            int tempROWS = ROWS;
            ROWS = that.ROWS;
            that.ROWS = tempROWS;
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
