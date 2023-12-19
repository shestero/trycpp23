#ifndef FILEREADER_H
#define FILEREADER_H

#include <fstream>
#include <QString>

// simple read file as a stream (range) of lines:
class line : public std::string {};
std::istream &operator>>(std::istream &is, line &l);

class FileReader
{
public:
    FileReader(const QString& file_name);

    inline auto range() {
        std::istream_iterator<line> b(file);
        std::istream_iterator<line> e;
        return std::ranges::subrange(b, e);
    }
    // NB std::generator isn't supported in GCC yet

private:
    std::ifstream file;
};

#endif // FILEREADER_H
