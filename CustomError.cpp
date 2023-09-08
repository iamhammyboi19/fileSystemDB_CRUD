#include "CustomError.h"
#include <iostream>

CustomError::CustomError(std::string message, int statusCode)
    : message{message}, statusCode{statusCode} {
  //
}

void CustomError::print(std::ostream &os) const {
  os << "{ ErrorCode: " << statusCode << ", ErrorMessage: " << message << " }";
}