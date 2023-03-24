//
// Created by 项小羽 on 2023/3/15.
//

#ifndef DBTRAIN_DEBUG_PRINT_HPP
#define DBTRAIN_DEBUG_PRINT_HPP


//#define Debug
#include <iostream>

template<typename T>
void Print(const T& t) {
#ifdef Debug
  std::cout << t << std::endl;
#endif
}

template<typename T, typename... args>
void Print(const T& t, const args&... param) {
#ifdef Debug
  std::cout << t ;
  Print(param...);
#endif
}



#endif  // DBTRAIN_DEBUG_PRINT_HPP
