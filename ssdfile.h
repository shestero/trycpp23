#ifndef SSDFILE_H
#define SSDFILE_H

#include "ssdfileheader.h"

#include <optional>
#include <ranges>
#include <vector>

#include <QString>

/**
 * @brief The SSDFile class
 * This is just file content description
 */
class SSDFile
{
public:
    SSDFile(const QString& file_name);

    const QString   file_name;

    auto range() {
        return std::ranges::subrange(data.cbegin(), data.cend());
    }
    size_t size() {
        return data.size();
    }

    SSDFileHeader   header;

    double tmin, tmax;
    double vmin, vmax;

private:
    std::vector<std::optional<std::pair<double, double>>> data;
    // empty optional means gape.

};

#endif // SSDFILE_H
