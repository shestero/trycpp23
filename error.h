#ifndef ERROR_H
#define ERROR_H

#include <QString>
// #include <exception>

class Error // : public std::exception
{
public:
    Error(const QString& message): message(message) {}

    virtual const char* what() const throw()
    {
        return qPrintable(message);
    }

    const QString message;
};

#endif // ERROR_H
