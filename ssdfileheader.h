#ifndef SSDFILEHEADER_H
#define SSDFILEHEADER_H

#include <fstream>
#include <QString>

class SSDFileHeader
{
public:
    SSDFileHeader(const QString& file_name);

    // header fields
    std::string org_app_ver;
    std::string mes_type;
    std::string date_and_time;
    std::string other;
};

#endif // SSDFILEHEADER_H
