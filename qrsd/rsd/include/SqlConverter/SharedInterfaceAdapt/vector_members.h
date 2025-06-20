#ifndef VECTOR_MEMBERS_H
#define VECTOR_MEMBERS_H

#include <boost/move/make_unique.hpp>

#define vector_ustring_iterator_adapt_FMEMBER_LIST(_class_name_, _f0_, _f1_, _f2_)\
  _f1_(_class_name_, clone, _class_name_##_intf*, _class_name_&, a0, new _class_name_##_impl(a0, _class_name_::impl_t(impl_), _class_name_::impl_adapt_t(impl_adapt_)), const)\
  _f1_(_class_name_, operator=, _class_name_&, const _class_name_&, a0, impl_ = reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_\
     COMMA impl_adapt_ = reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_adapt_ COMMA self_, )\
  \
  _f1_(_class_name_, operator+=, _class_name_&, difference_type, a0, impl_ += a0 COMMA impl_adapt_ += a0 COMMA self_, )\
  _f1_(_class_name_, operator-=, _class_name_&, difference_type, a0, impl_ -= a0 COMMA impl_adapt_ -= a0 COMMA self_, )\
  _f0_(_class_name_, operator*, _class_name_##_intf::value_type&, sync_impl() COMMA (*impl_adapt_).get(), const)\
  _f0_(_class_name_, operator->, boost::movelib::unique_ptr<_class_name_##_intf::value_type>, boost::movelib::make_unique<_class_name_##_intf::value_type>(boost::ref(*impl_)), const)\
  /*_f1_(_class_name_, operator[], _class_name_##_intf::value_type&, difference_type, a0, _class_name_##_intf::value_type(impl_[a0]), const)*/\
  \
  _f0_(_class_name_, operator++, _class_name_&, ++impl_ COMMA ++impl_adapt_ COMMA self_, )\
  _f0_(_class_name_, operator--, _class_name_&, --impl_ COMMA --impl_adapt_ COMMA self_, )\
  _f1_(_class_name_, operator++, _class_name_, int, a0, _class_name_(impl_++, impl_adapt_++), )\
  _f1_(_class_name_, operator--, _class_name_, int, a0, _class_name_(impl_--, impl_adapt_--), )\
  \
  _f1_(_class_name_, operator-, _class_name_##_intf::difference_type, const _class_name_&, a0, impl_ - reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator+, _class_name_, difference_type, a0, _class_name_(impl_ + a0, impl_adapt_ + a0), const)\
  _f1_(_class_name_, operator-, _class_name_, difference_type, a0, _class_name_(impl_ - a0, impl_adapt_ - a0), const)\
  \
  _f1_(_class_name_, operator==, bool, const _class_name_&, a0, impl_ == reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator!=, bool, const _class_name_&, a0, impl_ != reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator>, bool, const _class_name_&, a0, impl_ > reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator<, bool, const _class_name_&, a0, impl_ < reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator>=, bool, const _class_name_&, a0, impl_ >= reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\
  _f1_(_class_name_, operator<=, bool, const _class_name_&, a0, impl_ <= reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_, const)\


#define vector_ustring_adapt_FMEMBER_LIST(_class_name_, _f0_, _f1_, _f2_)\
  _f1_(_class_name_, clone, _class_name_##_intf*, _class_name_&, a0, new _class_name_##_impl(a0, _class_name_::impl_t(impl_)), const)\
  _f1_(_class_name_, operator=, _class_name_&, const _class_name_&, a0, impl_ = reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_ COMMA self_, )\
  _f1_(_class_name_, operator=, _class_name_&, _class_name_&&, a0, impl_ = std::move(reinterpret_cast<const _class_name_##_impl*>(a0.impl_)->impl_) COMMA self_, )\
  _f1_(_class_name_, operator[], _class_name_##_intf::value_type&, size_type, a0, sync_impl(a0) COMMA impl_adapt_[a0].get(), )\
  \
  _f0_(_class_name_, size, size_t, impl_.size(), const)\
  _f0_(_class_name_, empty, bool, impl_.empty(), const)\
  _f0_(_class_name_, begin, _class_name_##_intf::iterator, impl_adapt_.resize(impl_.size()) COMMA _class_name_##_intf::iterator(std::move(impl_.begin()), std::move(impl_adapt_.begin())), )\
  _f0_(_class_name_, end, _class_name_##_intf::iterator, impl_adapt_.resize(impl_.size()) COMMA _class_name_##_intf::iterator(std::move(impl_.end()), std::move(impl_adapt_.end())), )\
  _f0_(_class_name_, clear, void, impl_.clear(), )\
  _f1_(_class_name_, push_back, void, _class_name_##_intf::value_type&&, std::move(a0)\
    , impl_.push_back( std::move(reinterpret_cast<const _class_name_##_impl::value_type*>(a0.impl_)->unwrap()) ), )\
  _f1_(_class_name_, push_back, void, const _class_name_##_intf::value_type&, a0\
    , impl_.push_back( reinterpret_cast<const _class_name_##_impl::value_type*>(a0.impl_)->unwrap() ), )\

#endif