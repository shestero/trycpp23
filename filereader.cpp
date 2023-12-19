#include "error.h"
#include "filereader.h"

std::istream &operator>>(std::istream &is, line &l) {
    std::getline(is, l);
    l.erase(l.find_last_not_of(" \t\n\r")+1);
    return is;
}

FileReader::FileReader(const QString& file_name)
    : file(file_name.toStdString())
{
    if (!file.is_open()) {
        throw Error(QString("Cannot open the file: %1").arg(file_name));
    }
}
