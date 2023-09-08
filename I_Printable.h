#ifndef I_PRINTABLE
#define I_PRINTABLE

#include <iostream>

class I_Printable {
  friend std::ostream &operator<<(std::ostream &os, const I_Printable &obj);

private:
  /* data */
public:
  I_Printable() = default;
  virtual void print(std::ostream &os) const = 0;
  virtual ~I_Printable(){};
};

#endif