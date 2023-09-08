#ifndef CUSTOM_ERROR_H_
#define CUSTOM_ERROR_H_

#include "I_Printable.h"
#include <iostream>
#include <string>

class CustomError : public I_Printable {
private:
  std::string message;
  int statusCode;

public:
  CustomError(std::string message, int statusCode);
  virtual void print(std::ostream &os) const override;
  virtual ~CustomError() {}
};

#endif