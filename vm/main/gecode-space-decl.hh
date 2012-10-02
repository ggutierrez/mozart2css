#ifndef VM_HAS_CSS
#error This file is only included with the CSS support
#endif

// temporal!!
#include <iostream>

#include <vector>
#include <gecode/kernel.hh>
#include <gecode/int.hh>
#include <gecode/set.hh>

namespace mozart {
class GecodeSpace : public Gecode::Space {
private:
  std::vector<Gecode::IntVar> _intVars;
  std::vector<Gecode::SetVar> _setVars;
public:
    GecodeSpace(void)
      : Gecode::Space() {
      std::cout << "Constructed gecode space" << std::endl;
      Gecode::IntVar x(*this,0,1);
      Gecode::SetVar y(*this,Gecode::IntSet::empty,Gecode::IntSet::empty);
      _intVars.push_back(x);
      _setVars.push_back(y);
    }
    GecodeSpace(bool share, GecodeSpace& other) 
      : Gecode::Space(share,other),
	_intVars(other._intVars),
	_setVars(other._setVars)
    {
      for(auto i = _intVars.size(); i--;)
	_intVars[i].update(*this,share,other._intVars[i]);
      for(auto i = _setVars.size(); i--;)
	_setVars[i].update(*this,share,other._setVars[i]);
    }
    virtual ~GecodeSpace(void) {
      std::cout << "Destructed gecode space" << std::endl;
    }
    virtual Gecode::Space* copy(bool share) override {
      return new GecodeSpace(share,*this);
    }
};
}
