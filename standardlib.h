#ifndef __STD_LIB__
#define __STD_LIB__

#include <iostream>

class standardlib
{
  private:
  public:
    standardlib();
    void mid() { std::cout << "akash\n"; };
    void ex() { exit(1); };
    ~standardlib();
};

standardlib::standardlib()
{
    std::cout << "started\n";
}

standardlib::~standardlib()
{
    std::cout << "ended\n";
}

#endif