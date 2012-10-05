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
  Gecode::IntVar& intVar(size_t index) {
    if (index >= _intVars.size())
      std::cerr << "Accessing intVar at invalid index" << std::endl;
    return _intVars[index];
  }
  size_t newIntVar(int min, int max) {
    _intVars.push_back(Gecode::IntVar(*this,min,max));
    return _intVars.size() - 1;
  }
  Gecode::SetVar& setVar(size_t index) {
    if (index >= _setVars.size())
      std::cerr << "Accessing setVar at invalid index" << std::endl;
    return _setVars[index];
  }
  void dumpSpaceInformation(void) const {
    std::cout << "Space information" 
	      << std::endl
	      << "\tInteger variables: " << _intVars.size() << std::endl
	      << "\tset variables: " << _setVars.size() << std::endl;
  }
};
}
