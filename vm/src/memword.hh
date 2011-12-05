// Copyright © 2011, Université catholique de Louvain
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// *  Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// *  Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// A MemWord (memory word) is a type which is defined as having the size of a
// char* and behaving as the union of a number of other types. If they are
// small enough, they are actually stored in the MemWord. If not, they are
// stored as a pointer to external memory.

// The MWUnion type implements the MemWord by having a template pack of types
// to be made into a union.

// The MWUAccessor provides the getters/setters for the MWUnion.

// The MWTest is an helper class to deal with types which are bigger than a
// char*


// We need the VM class to allocate memory for "big" types.
class VirtualMachine;
typedef VirtualMachine& VM;

// MWTest is a metafunction that returns T if i!=0 and a type guaranteed to be
// smaller or equal to char* if i==0
template<class T, int i>
struct MWTest{
  typedef T Type;
};
template<class T>
class MWTest<T,0>{
  typedef char Type;
};

template<class ...args>
union MWUnion;

// In an MWUAccessor, U is a specialization of MWUnion, T is the requested type
// and R is the type of the first member of the union.
// In the default case, the requested type is different from the first one and
// we recurse in the rest.
template<class U, class T, class R>
class MWUAccessor{
  static T& get(U *u){return (u->next).template get<T>();}
  static void set(VM vm, U *u, T v){u->next.template set<T>(vm,v);}
};

// The bottom case of the MWUnion contains just a char* as it can be cast to
// any other pointer without fear of aliasing problems.
template<>
union MWUnion<>{
  char* it;
  template<class T>
  T& get(){return MWUAccessor<MWUnion,T,T>::get(this);}
  template<class T>
  void set(VM vm,T v){MWUAccessor<MWUnion,T,T>::set(vm,this,v);}
};

// The easy case of accessing what we have in the first member of the union.
template<class U, class T>
class MWUAccessor<U,T,T>{
  static T& get(U *u){return u->it;}
  static void set(VM vm, U *u, T v){u->it=v;}
};

// Accessing something that wasn't there, if a pointer type, we just get away
// with a cast.
template<class T>
class MWUAccessor<MWUnion<>,T*,T*>{
  static T*& get(MWUnion<>* u){return static_cast<T*>(u->it);}
  static void set(VM vm, MWUnion<> *u, T *v){u->it=static_cast<char*>(v);}
};
// It isn't there and isn't a pointer so we store a pointer to it. We need new
// memory on setting too.
template<class T>
class MWUAccessor<MWUnion<>,T,T>{
  static T& get(MWUnion<>* u){return *static_cast<T*>(u->it);}
  static void set(VM vm, MWUnion<> *u, T *v){
    u->it=static_cast<char*>(new(vm)T(v));
  }
};
// The union itself, recursive on the parameter pack. If the first type is too
// big, we reduce it.
template<class T, class ...args>
union MWUnion<T,args...>{
  typedef typename MWTest<T,sizeof(T)<=sizeof(char*)>::Type Tred;
  Tred it;
  MWUnion<args...> next;
  template<class Q>
  Q& get(){return MWUAccessor<MWUnion,Q,Tred>::get(this);}
  template<class Q>
  void set(VM vm,Q v){MWUAccessor<MWUnion,Q,Tred>::set(vm,this,v);}
};

// Finally, here comes the list of potentially small types that we want to
// optimize in a memory word.
typedef MWUnion<int,float> MemWord;
