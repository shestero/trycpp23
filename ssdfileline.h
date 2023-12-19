#ifndef SSDFILELINE_H
#define SSDFILELINE_H

#include <iostream>
#include <string>

class SSDFileLine : public std::string {};

inline std::istream &operator>>(std::istream &is, SSDFileLine &l) {
    std::getline(is, l);
    l.erase(l.find_last_not_of(" \t\n\r")+1);
    return is;
}

#endif // SSDFILELINE_H
