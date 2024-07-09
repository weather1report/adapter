#pragma once
#include <iterator>

template<class C, class F>
class transform_view {
private:
    static_assert(std::is_same<typename C::iterator::iterator_category, std::forward_iterator_tag>() ||
    std::is_same<typename C::iterator::iterator_category, std::bidirectional_iterator_tag>() ||
    std::is_same<typename C::iterator::iterator_category, std::random_access_iterator_tag>() ,"iterator must be forward");

    typedef typename C::value_type T;
    typename C::iterator b, e;
    F func_;
public:
    transform_view(C& container, F f): func_(f), b(container.begin()), e(container.end()) {}
    typedef T key_type;
    typedef  T value_type;

    class iterator {
    public:
        F func_;
        typename C::iterator curr_;
        typedef typename C::value_type value_type;
        typedef typename C::value_type& reference;
        typedef typename C::iterator::iterator_category iterator_category;
        iterator(const typename C::iterator it, F f): curr_(it), func_(f) {}

        bool operator==(const iterator& other) const {return curr_ == other.curr_;} 
        bool operator!=(const iterator& other) const {return curr_ != other.curr_;}

        iterator& operator++() {
            ++curr_;
            return *this;
        }

        auto operator*() const {
            return func_(*curr_);
        }
    };

    iterator begin() {return iterator(b, func_);}
    iterator end() {return iterator(e, func_);}    
};

template<class F>
struct transform_func {F func_;};

template<class F>
transform_func<F> transform(F f) {
    return transform_func<F>{f};
}

template<class C, class F>
transform_view<C,F> operator|(C& container, transform_func<F> f) {
    return transform_view<C,F>(container, f.func_);
}

template<class C, class F>
transform_view<C,F> operator|(C&& container, transform_func<F> f) {
    return transform_view<C,F>(container, f.func_);
}


template<class C, class F>
class filter_view {
private:
    static_assert(std::is_same<typename C::iterator::iterator_category, std::forward_iterator_tag>() ||
    std::is_same<typename C::iterator::iterator_category, std::bidirectional_iterator_tag>() ||
    std::is_same<typename C::iterator::iterator_category, std::random_access_iterator_tag>() ,"iterator must be forward");

    typedef typename C::value_type T;
    typename C::iterator b, e;
    F func_;
public:
    filter_view(C& container, F f): func_(f), b(container.begin()), e(container.end()) {
        while ((b != e) && !func_(*b)) ++b;
    }
    typedef T key_type;
    typedef  T value_type;

    class iterator {
    public:
        F func_;
        typename C::iterator curr_, e;
        typedef typename C::value_type value_type;
        typedef typename C::value_type& reference;
        typedef typename C::iterator::iterator_category iterator_category;
        iterator(const typename C::iterator it, F f, const typename C::iterator end): curr_(it), e(end), func_(f) {}

        bool operator==(const iterator& other) const {return curr_ == other.curr_;} 
        bool operator!=(const iterator& other) const {return curr_ != other.curr_;}

        iterator& operator++() {
            ++curr_;
            while ((curr_ != e) && !func_(*curr_)) ++curr_;
            return *this;
        }

        reference operator*() const {
            return *curr_;
        }
    };

    iterator begin() {return iterator(b, func_, e);}
    iterator end() {return iterator(e, func_, e);}    
};

template<class F>
struct filter_func {F func_;};

template<class F>
filter_func<F> filter(F f) {
    return filter_func<F>{f};
}

template<class C, class F>
filter_view<C,F> operator|(C& container, filter_func<F> f) {
    return filter_view<C,F>(container, f.func_);
}

template<class C, class F>
filter_view<C,F> operator|(C&& container, filter_func<F> f) {
    return filter_view<C,F>(container, f.func_);
}


template<class C>
class take_view {
private:
    static_assert(std::is_same<typename C::iterator::iterator_category, std::forward_iterator_tag>() ||
    std::is_same<typename C::iterator::iterator_category, std::bidirectional_iterator_tag>() ||
    std::is_same<typename C::iterator::iterator_category, std::random_access_iterator_tag>() ,"iterator must be forward");

    typedef typename C::value_type T;
    typename C::iterator b, e;
    size_t prefix;
public:
    take_view(C& container, size_t pre): prefix(pre), b(container.begin()), e(container.end()) {}
    typedef T key_type;
    typedef  T value_type;

    class iterator {
    public:
        size_t prefix;
        typename C::iterator curr_, e;
        typedef typename C::value_type value_type;
        typedef typename C::value_type& reference;
        typedef typename C::iterator::iterator_category iterator_category;
        iterator(const typename C::iterator it, size_t pre, const typename C::iterator end): curr_(it), e(end), prefix(pre) {}

        bool operator==(const iterator& other) const {return curr_ == other.curr_;} 
        bool operator!=(const iterator& other) const {return curr_ != other.curr_;}

        iterator& operator++() {
            ++curr_;
            if (--prefix <= 0) curr_ = e;
            return *this;
        }

        reference operator*() const {
            return *curr_;
        }
    };

    iterator begin() {return iterator(prefix > 0 ? b : e, prefix, e);}
    iterator end() {return iterator(e, prefix, e);}    
};

struct take_num {size_t prefix;};

take_num take(size_t pre) {return take_num{pre};}


template<class C>
take_view<C> operator|(C& container, take_num t) {
    return take_view<C>(container, t.prefix);
}

template<class C>
take_view<C> operator|(C&& container, take_num t) {
    return take_view<C>(container, t.prefix);
}


template<class C>
class drop_view {
private:
    static_assert(std::is_same<typename C::iterator::iterator_category, std::forward_iterator_tag>() ||
    std::is_same<typename C::iterator::iterator_category, std::bidirectional_iterator_tag>() ||
    std::is_same<typename C::iterator::iterator_category, std::random_access_iterator_tag>() ,"iterator must be forward");

    typedef typename C::value_type T;
    typename C::iterator b, e;
    size_t prefix;
public:
    drop_view(C& container, size_t pre): prefix(pre), b(container.begin()), e(container.end()) {
        while (b != e && pre--) ++b;
    }
    typedef T key_type;
    typedef  T value_type;

    class iterator {
    public:
        size_t prefix;
        typename C::iterator curr_, e;
        typedef typename C::value_type value_type;
        typedef typename C::value_type& reference;
        typedef typename C::iterator::iterator_category iterator_category;
        iterator(const typename C::iterator it, size_t pre, const typename C::iterator end): curr_(it), e(end), prefix(pre) {}

        bool operator==(const iterator& other) const {return curr_ == other.curr_;} 
        bool operator!=(const iterator& other) const {return curr_ != other.curr_;}

        iterator& operator++() {
            ++curr_;
            return *this;
        }

        reference operator*() const {
            return *curr_;
        }
    };

    iterator begin() {return iterator(b, prefix, e);}
    iterator end() {return iterator(e, prefix, e);}    
};

struct drop_num {size_t prefix;};

drop_num drop(size_t pre) {return drop_num{pre};}

template<class C>
drop_view<C> operator|(C& container, drop_num t) {
    return drop_view<C>(container, t.prefix);
}

template<class C>
drop_view<C> operator|(C&& container, drop_num t) {
    return drop_view<C>(container, t.prefix);
}


template<class C>
class reverse_view {
private:
    static_assert(std::is_same<typename C::iterator::iterator_category, std::forward_iterator_tag>() ||
    std::is_same<typename C::iterator::iterator_category, std::bidirectional_iterator_tag>() ||
    std::is_same<typename C::iterator::iterator_category, std::random_access_iterator_tag>() ,"iterator must be forward");

    typedef typename C::value_type T;
    typedef typename std::reverse_iterator<typename C::iterator> It;
    It b, e;
public:
    reverse_view(C& container): b(container.end()), e(container.begin()) {}
    typedef T key_type;
    typedef  T value_type;

    class iterator {
    public:
        It curr_;
        typedef typename C::value_type value_type;
        typedef typename C::value_type& reference;
        typedef typename C::iterator::iterator_category iterator_category;
        iterator(It it): curr_(it) {}

        bool operator==(const iterator& other) const {return curr_ == other.curr_;} 
        bool operator!=(const iterator& other) const {return curr_ != other.curr_;}

        iterator& operator++() {
            ++curr_;
            return *this;
        }

        reference operator*() const {
            return *curr_;
        }
    };

    iterator begin() {return iterator(b);}
    iterator end() {return iterator(e);}    
};

struct reverse_f {};

reverse_f reverse() {return reverse_f();}


template<class C>
reverse_view<C> operator|(C& container, reverse_f) {
    return reverse_view<C>(container);
}


template<class C>
class keys_view {
private:
    static_assert(std::is_same<typename C::iterator::iterator_category, std::bidirectional_iterator_tag>(),"iterator must be bidirectional");
    typedef typename C::iterator It;
    It b, e;
public:
    keys_view(C& container): b(container.begin()), e(container.end()) {}
    typedef typename C::key_type key_type;
    typedef typename C::value_type value_type;

    class iterator {
    public:
        It curr_;
        typedef typename C::value_type value_type;
        typedef typename C::value_type& reference;
        typedef typename C::iterator::iterator_category iterator_category;
        iterator(It it): curr_(it) {}

        bool operator==(const iterator& other) const {return curr_ == other.curr_;} 
        bool operator!=(const iterator& other) const {return curr_ != other.curr_;}

        iterator& operator++() {
            ++curr_;
            return *this;
        }

        key_type operator*() const {
            return (*curr_).first;
        }
    };

    iterator begin() {return iterator(b);}
    iterator end() {return iterator(e);}    
};

struct keys_f {};

keys_f keys() {return keys_f();}

template<class C>
keys_view<C> operator|(C& container, keys_f) {
    return keys_view<C>(container);
}

template<class C>
keys_view<C> operator|(C&& container, keys_f) {
    return keys_view<C>(container);
}


template<class C>
class values_view {
private:
    static_assert(std::is_same<typename C::iterator::iterator_category, std::bidirectional_iterator_tag>(),"iterator must be bidirectional");
    typedef typename C::iterator It;
    It b, e;
public:
    values_view(C& container): b(container.begin()), e(container.end()) {}
    typedef typename C::key_type key_type;
    typedef typename C::value_type value_type;

    class iterator {
    public:
        It curr_;
        typedef typename C::value_type value_type;
        typedef typename C::mapped_type mapped_type;
        typedef typename C::value_type& reference;
        typedef typename C::iterator::iterator_category iterator_category;
        iterator(It it): curr_(it) {}

        bool operator==(const iterator& other) const {return curr_ == other.curr_;} 
        bool operator!=(const iterator& other) const {return curr_ != other.curr_;}

        iterator& operator++() {
            ++curr_;
            return *this;
        }

        mapped_type operator*() const {
            return (*curr_).second;
        }
    };

    iterator begin() {return iterator(b);}
    iterator end() {return iterator(e);}    
};

struct values_f {};

values_f values() {return values_f();}

template<class C>
values_view<C> operator|(C& container, values_f) {
    return values_view<C>(container);
}

template<class C>
values_view<C> operator|(C&& container, values_f) {
    return values_view<C>(container);
}