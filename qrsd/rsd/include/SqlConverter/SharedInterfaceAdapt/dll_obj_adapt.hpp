#ifndef DLL_OBJ_ADAPT_HPP
#define DLL_OBJ_ADAPT_HPP

#include <memory>

#if !defined __PRETTY_FUNCTION__
  #define __PRETTY_FUNCTION__   __FUNCSIG__  
#endif

//#define DBG_OUT

#ifdef DBG_OUT
#define DBG_ADAPT(_msg_)\
  std::wcout << _msg_ << L"\n";
#else
  #define DBG_ADAPT(_msg_)
#endif

template<class T>
class obj_adapt {
public:
  typedef void(*free_t)(void *);

  T * impl_;
  free_t free_;

  //obj_adapt() : impl_(nullptr), free_(nullptr) {}
  obj_adapt(T * obj, free_t free) : impl_(obj), free_(free) {}
  obj_adapt(obj_adapt && t, T * obj, free_t free) : impl_(obj), free_(free)  {
    move(std::move(t)); }
  obj_adapt & operator=(obj_adapt && t) {
    move(std::move(t));
    return *this; 
  }

  virtual ~obj_adapt(){ if(impl_ != nullptr) { free_(impl_); impl_ = nullptr; DBG_ADAPT( __PRETTY_FUNCTION__ ); } }

  //T* operator->() { return impl_; }
  //const T* operator->() const { return impl_; }
protected:
  obj_adapt(const obj_adapt & t, T * intf) : impl_(intf), free_(t.free_) { }

  obj_adapt(const obj_adapt & t) { throw; }
  obj_adapt & operator=(const obj_adapt & t) { throw; return *this; }
  obj_adapt(obj_adapt && t) { throw; }

  void move(obj_adapt && t) {
    T * tmp_impl = std::move(impl_);
    free_t tmp_free = std::move(free_);
    //if(impl_ != nullptr) free_(impl_);
    impl_ = std::move(t.impl_);
    free_ = std::move(t.free_);
    t.impl_ = std::move(tmp_impl);
    t.free_ = std::move(tmp_free);
  }
};



#define COMMA ,

#define GEN_VIRTUAL_FMEMBER_0(_class_name_, _name_, _ret_type_, _ret_val_, _const_)\
  virtual _ret_type_ _name_() _const_ = 0;\

#define GEN_VIRTUAL_FMEMBER_1(_class_name_, _name_, _ret_type_, _A0_, _a0_, _ret_val_, _const_)\
  virtual _ret_type_ _name_(_A0_ a0) _const_ = 0;\

#define GEN_VIRTUAL_FMEMBER_2(_class_name_, _name_, _ret_type_, _A0_, _a0_, _A1_, _a1_, _ret_val_, _const_)\
  virtual _ret_type_ _name_(_A0_ a0, _A1_ a1) _const_ = 0;\

#define GEN_VIRTUAL_FMEMBER_LIST(_class_name_, _list_)\
  _list_(_class_name_, GEN_VIRTUAL_FMEMBER_0, GEN_VIRTUAL_FMEMBER_1, GEN_VIRTUAL_FMEMBER_2)\

//-------------------------------------------
#define GEN_DELEGATE_FMEMBER_0(_class_name_, _name_, _ret_type_, _ret_val_, _const_)\
  inline _ret_type_ _name_() _const_ { return impl_->_name_(); }\

#define GEN_DELEGATE_FMEMBER_1(_class_name_, _name_, _ret_type_, _A0_, _a0_, _ret_val_, _const_)\
  inline _ret_type_ _name_(_A0_ a0) _const_ { return impl_->_name_(_a0_); }\

#define GEN_DELEGATE_FMEMBER_2(_class_name_, _name_, _ret_type_, _A0_, _a0_, _A1_, _a1_, _ret_val_, _const_)\
  inline _ret_type_ _name_(_A0_ a0, _A1_ a1) _const_ { return impl_->_name_(_a0_, _a1_); }\

#define GEN_DELEGATE_FMEMBER_LIST(_class_name_, _list_)\
  _list_(_class_name_, GEN_DELEGATE_FMEMBER_0, GEN_DELEGATE_FMEMBER_1, GEN_DELEGATE_FMEMBER_2)\

#define GEN_DELEGATE_UNWRAP(_class_name_)\
  impl_t & unwrap();\
  const impl_t & unwrap() const;\
  operator impl_t&();\
  operator impl_t const&() const;
  

#define GEN_DELEGATE_UNWRAP_IMPL(_class_name_)\
_class_name_::impl_t & _class_name_::unwrap() {\
  return reinterpret_cast<_class_name_##_impl*>(impl_)->unwrap();\
}\
const _class_name_::impl_t & _class_name_::unwrap() const {\
  return reinterpret_cast<_class_name_##_impl*>(impl_)->unwrap();\
}\
_class_name_::operator impl_t&() {\
  return unwrap();\
}\
_class_name_::operator impl_t const& () const {\
  return unwrap();\
}\

//-------------------------------------------
#define GEN_OVERRIDE_FMEMBER_0(_class_name_, _name_, _ret_type_, _ret_val_, _const_)\
  _ret_type_ _name_() _const_ override;\

#define GEN_OVERRIDE_FMEMBER_1(_class_name_, _name_, _ret_type_, _A0_, _a0_, _ret_val_, _const_)\
  _ret_type_ _name_(_A0_ a0) _const_ override;\

#define GEN_OVERRIDE_FMEMBER_2(_class_name_, _name_, _ret_type_, _A0_, _a0_, _A1_, _a1_, _ret_val_, _const_)\
  _ret_type_ _name_(_A0_ a0, _A1_ a1) _const_ override;\

#define GEN_OVERRIDE_FMEMBER_LIST(_class_name_, _list_)\
  _list_(_class_name_, GEN_OVERRIDE_FMEMBER_0, GEN_OVERRIDE_FMEMBER_1, GEN_OVERRIDE_FMEMBER_2)\

//-------------------------------------------
#define GEN_IMPL_FMEMBER_0(_class_name_, _name_, _ret_type_, _ret_val_, _const_)\
  _ret_type_ _class_name_##_impl::##_name_() _const_ {\
    DBG_ADAPT( __PRETTY_FUNCTION__ );\
    return _ret_val_;\
  }\

#define GEN_IMPL_FMEMBER_1(_class_name_, _name_, _ret_type_, _A0_, _a0_, _ret_val_, _const_)\
  _ret_type_ _class_name_##_impl::##_name_(_A0_ a0) _const_ {\
    DBG_ADAPT( __PRETTY_FUNCTION__ );\
    return _ret_val_;\
  }\

#define GEN_IMPL_FMEMBER_2(_class_name_, _name_, _ret_type_, _A0_, _a0_, _A1_, _a1_, _ret_val_, _const_)\
  _ret_type_ _class_name_##_impl::##_name_(_A0_ a0, _A1_ a1) _const_ {\
    DBG_ADAPT( __PRETTY_FUNCTION__ );\
    return _ret_val_;\
  }\

#define GEN_IMPL_FMEMBER_LIST(_class_name_, _list_)\
  _list_(_class_name_, GEN_IMPL_FMEMBER_0, GEN_IMPL_FMEMBER_1, GEN_IMPL_FMEMBER_2)\



#define GEN_IMPL_UNWRAP_IMPL(_class_name_)\
_class_name_##_impl::impl_t & _class_name_##_impl::unwrap() {\
  return impl_;\
}\
const _class_name_##_impl::impl_t & _class_name_##_impl::unwrap() const {\
  return impl_;\
}\

#endif