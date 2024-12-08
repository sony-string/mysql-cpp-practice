#include <iostream>

void clear_cin_error() {
    std::cin.clear();            
    while (std::cin.get() == '\n'); 
}