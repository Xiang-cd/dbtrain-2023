//
// Created by 项小羽 on 2023/3/15.
//

#ifndef DBTRAIN_DEBUG_PRINT_HPP
#define DBTRAIN_DEBUG_PRINT_HPP


//#define Debug
//#define DebugLAB3
//#define DebugLAB4
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



template<typename T>
void LAB3Print(const T& t) {
#ifdef DebugLAB3
  std::cout << t << std::endl;
#endif
}


template<typename T, typename... args>
void LAB3Print(const T& t, const args&... param) {
#ifdef DebugLAB3
  std::cout << t ;
  LAB3Print(param...);
#endif
}


template<typename T>
void LAB4Print(const T& t) {
#ifdef DebugLAB4
  std::cout << t << std::endl;
#endif
}


template<typename T, typename... args>
void LAB4Print(const T& t, const args&... param) {
#ifdef DebugLAB4
  std::cout << t ;
  LAB4Print(param...);
#endif
}


#endif  // DBTRAIN_DEBUG_PRINT_HPP
