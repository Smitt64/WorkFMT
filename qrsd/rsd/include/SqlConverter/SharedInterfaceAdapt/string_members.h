#ifndef STRING_MEMBERS_H
#define STRING_MEMBERS_H

//
//--------------------------------------------------------
#define string_iterator_adapt_FMEMBER_LIST(_class_name_, _f0_, _f1_, _f2_)\
  _f1_(_class_name_, clone, _class_name_##_intf*, _class_name_&, a0, new _class_name_##_impl(a0, _class_name_##_impl::impl_t(impl_)), const)\
  _f1_(_class_name_, operator=, _class_name_&, const _class_name_&, a0, impl_ = reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_ COMMA self_, )\
  \
  _f1_(_class_name_, operator+=, _class_name_&, difference_type, a0, impl_ += a0 COMMA self_, )\
  _f1_(_class_name_, operator-=, _class_name_&, difference_type, a0, impl_ -= a0 COMMA self_, )\
  _f0_(_class_name_, operator*, _class_name_##_intf::value_type&, *impl_, const)\
  _f0_(_class_name_, operator->, _class_name_##_intf::value_type*, &*impl_, const)\
  _f1_(_class_name_, operator[], _class_name_##_intf::value_type&, difference_type, a0, impl_[a0], const)\
  \
  _f0_(_class_name_, operator++, _class_name_&, ++impl_ COMMA self_, )\
  _f0_(_class_name_, operator--, _class_name_&, --impl_ COMMA self_, )\
  _f1_(_class_name_, operator++, _class_name_, int, a0, impl_++, )\
  _f1_(_class_name_, operator--, _class_name_, int, a0, impl_--, )\
  \
  _f1_(_class_name_, operator-, _class_name_##_intf::difference_type, const _class_name_&, a0, impl_ - reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator+, _class_name_, difference_type, a0, impl_ + a0, const)\
  _f1_(_class_name_, operator-, _class_name_, difference_type, a0, impl_ - a0, const)\
  \
  _f1_(_class_name_, operator==, bool, const _class_name_&, a0, impl_ == reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator!=, bool, const _class_name_&, a0, impl_ != reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator>, bool, const _class_name_&, a0, impl_ > reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator<, bool, const _class_name_&, a0, impl_ < reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator>=, bool, const _class_name_&, a0, impl_ >= reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator<=, bool, const _class_name_&, a0, impl_ <= reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\



#define string_const_iterator_adapt_FMEMBER_LIST(_class_name_, _f0_, _f1_, _f2_)\
  _f1_(_class_name_, clone, _class_name_##_intf*, _class_name_&, a0, new _class_name_##_impl(a0, _class_name_##_impl::impl_t(impl_)), const)\
  _f1_(_class_name_, operator=, _class_name_&, const _class_name_&, a0, impl_ = reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_ COMMA self_, )\
  \
  _f1_(_class_name_, operator+=, _class_name_&, difference_type, a0, impl_ += a0 COMMA self_, )\
  _f1_(_class_name_, operator-=, _class_name_&, difference_type, a0, impl_ -= a0 COMMA self_, )\
  _f0_(_class_name_, operator*, _class_name_##_intf::value_type&, *impl_, const)\
  _f0_(_class_name_, operator->, _class_name_##_intf::value_type*, &*impl_, const)\
  _f1_(_class_name_, operator[], _class_name_##_intf::value_type&, difference_type, a0, impl_[a0], const)\
  \
  _f0_(_class_name_, operator++, _class_name_&, ++impl_ COMMA self_, )\
  _f0_(_class_name_, operator--, _class_name_&, --impl_ COMMA self_, )\
  _f1_(_class_name_, operator++, _class_name_, int, a0, impl_++, )\
  _f1_(_class_name_, operator--, _class_name_, int, a0, impl_--, )\
  \
  _f1_(_class_name_, operator-, _class_name_##_intf::difference_type, const _class_name_&, a0, impl_ - reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator+, _class_name_, difference_type, a0, impl_ + a0, const)\
  _f1_(_class_name_, operator-, _class_name_, difference_type, a0, impl_ - a0, const)\
  \
  _f1_(_class_name_, operator==, bool, const _class_name_&, a0, impl_ == reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator!=, bool, const _class_name_&, a0, impl_ != reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator>, bool, const _class_name_&, a0, impl_ > reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator<, bool, const _class_name_&, a0, impl_ < reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator>=, bool, const _class_name_&, a0, impl_ >= reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator<=, bool, const _class_name_&, a0, impl_ <= reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\

//    -inline Iterator& operator+=(difference_type rhs) {_ptr += rhs; return *this;}
//    -inline Iterator& operator-=(difference_type rhs) {_ptr -= rhs; return *this;}
//    -inline Type& operator*() const {return *_ptr;}
//    -inline Type* operator->() const {return _ptr;}
//    -inline Type& operator[](difference_type rhs) const {return _ptr[rhs];}
//    
//    -inline Iterator& operator++() {++_ptr; return *this;}
//    -inline Iterator& operator--() {--_ptr; return *this;}
//    -inline Iterator operator++(int) const {Iterator tmp(*this); ++_ptr; return tmp;}
//    -inline Iterator operator--(int) const {Iterator tmp(*this); --_ptr; return tmp;}
//    /* inline Iterator operator+(const Iterator& rhs) {return Iterator(_ptr+rhs.ptr);} */
//    -inline difference_type operator-(const Iterator& rhs) const {return _ptr-rhs.ptr;}
//    -inline Iterator operator+(difference_type rhs) const {return Iterator(_ptr+rhs);}
//    -inline Iterator operator-(difference_type rhs) const {return Iterator(_ptr-rhs);}
//    friend inline Iterator operator+(difference_type lhs, const Iterator& rhs) {return Iterator(lhs+rhs._ptr);}
//    friend inline Iterator operator-(difference_type lhs, const Iterator& rhs) {return Iterator(lhs-rhs._ptr);}
//    
//    -inline bool operator==(const Iterator& rhs) const {return _ptr == rhs._ptr;}
//    -inline bool operator!=(const Iterator& rhs) const {return _ptr != rhs._ptr;}
//    -inline bool operator>(const Iterator& rhs) const {return _ptr > rhs._ptr;}
//    -inline bool operator<(const Iterator& rhs) const {return _ptr < rhs._ptr;}
//    -inline bool operator>=(const Iterator& rhs) const {return _ptr >= rhs._ptr;}
//    -inline bool operator<=(const Iterator& rhs) const {return _ptr <= rhs._ptr;}
//--------------------------------------------------------

//  _f1_(_class_name_, operator=, _class_name_&, const _class_name_&, a0, impl_ = reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_ COMMA self_, )\
//--------------------------------------------------------
//#define string_adapt_CLONE_LIST(_class_name_, _f0_, _f1_, _f2_)\
  _f1_(_class_name_, clone, _class_name_##_intf*, _class_name_&, a0, new self_t(a0, _class_name_::impl_t(impl_)), const)\


#define string_adapt_FMEMBER_LIST(_class_name_, _f0_, _f1_, _f2_)\
  _f1_(_class_name_, operator=, _class_name_&, const _class_name_&, a0, impl_ = reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_ COMMA self_, )\
  _f1_(_class_name_, operator=, _class_name_&, _class_name_&&, a0, impl_ = std::move(reinterpret_cast<_class_name_##_impl*>(a0.impl_)->impl_) COMMA self_, )\
  \
  _f0_(_class_name_, c_str, const _class_name_##_intf::value_type *, impl_.c_str(), const)\
  _f0_(_class_name_, size, size_t, impl_.size(), const)\
  _f0_(_class_name_, empty, bool, impl_.empty(), const)\
  _f0_(_class_name_, begin, _class_name_##_intf::iterator, _class_name_##_intf::iterator(std::move(impl_.begin())), )\
  _f0_(_class_name_, end, _class_name_##_intf::iterator, _class_name_##_intf::iterator(std::move(impl_.end())), )\
  _f0_(_class_name_, cbegin, _class_name_##_intf::const_iterator, _class_name_##_intf::const_iterator(std::move(impl_.cbegin())), const)\
  _f0_(_class_name_, cend, _class_name_##_intf::const_iterator, _class_name_##_intf::const_iterator(std::move(impl_.cend())), const)\
  _f0_(_class_name_, clear, void, impl_.clear(), )\
  _f2_(_class_name_, erase, _class_name_##_intf::iterator, _class_name_##_intf::iterator, a0, _class_name_##_intf::iterator, a1\
    , _class_name_##_intf::iterator(std::move(impl_.erase(reinterpret_cast<const _class_name_##_impl::iterator*>(a0.impl_)->impl_, reinterpret_cast<const _class_name_##_impl::iterator*>(a1.impl_)->impl_))), )\
  \
  _f2_(_class_name_, assign, _class_name_&, _class_name_##_intf::const_iterator, a0, _class_name_##_intf::const_iterator, a1\
    , impl_.assign(reinterpret_cast<const _class_name_##_impl::const_iterator*>(a0.impl_)->impl_, reinterpret_cast<const _class_name_##_impl::const_iterator*>(a1.impl_)->impl_) COMMA self_, )\
  \
  _f1_(_class_name_, assign, _class_name_&, const _class_name_&, a0 \
    , impl_.assign( reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_.cbegin(), reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_.cend() ) COMMA self_, )\
  \
  _f1_(_class_name_, operator+=, _class_name_&, const _class_name_##_intf::value_type *, a0, impl_ += a0 COMMA self_, )\
  _f1_(_class_name_, operator+=, _class_name_&, _class_name_##_intf::value_type, a0, impl_ += a0 COMMA self_, )\
  _f1_(_class_name_, operator+=, _class_name_&, const _class_name_&, a0, impl_ += reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_ COMMA self_, )\
  \
  _f1_(_class_name_, operator+, _class_name_, const _class_name_&, a0, impl_ + reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  \
  _f1_(_class_name_, operator==, bool, const _class_name_&, a0, impl_ == reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator==, bool, _class_name_##_intf::value_type const *, a0, impl_ == a0, const)\
  _f1_(_class_name_, operator!=, bool, _class_name_##_intf::value_type const *, a0, impl_ != a0, const)\
  /*_f1_(_class_name_, operator!=, bool, const _class_name_&, a0, impl_ != reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)*/\
  /*_f1_(_class_name_, operator>, bool, const _class_name_&, a0, impl_ > reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)*/\
  _f1_(_class_name_, operator<, bool, const _class_name_&, a0, impl_ < reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  //_f1_(_class_name_, operator>=, bool, const _class_name_&, a0, impl_ >= reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator<=, bool, const _class_name_&, a0, impl_ <= reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\


#define string_icase_adapt_FMEMBER_LIST(_class_name_, _f0_, _f1_, _f2_)\
  _f0_(_class_name_, str, const _class_name_##_intf::base_t, _class_name_##_intf::base_t( _class_name_##_intf::base_t::impl_t(impl_.str()) ), const)\
//_f0_(_class_name_, str, const _class_name_##_intf::base_t, _class_name_##_intf::base_t( boost::ref(impl_.str()) ), )\

//  _f2_(_class_name_, assign, _class_name_&, _class_name_##_intf::iterator, a0, _class_name_##_intf::iterator, a1\
    , impl_.assign(a0.impl_, a1.impl_) COMMA self_, )\

#endif