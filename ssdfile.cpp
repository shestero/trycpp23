#include "ssdfile.h"

#include "error.h"
#include "filereader.h"

#include <fstream>
#include <ranges>
#include <sstream>

SSDFile::SSDFile(const QString& file_name)
    : file_name(file_name)
    , header(file_name)
{
    FileReader reader(file_name);
    std::optional<double> tlast; // used only to check t order
    size_t size = 0;
    double t=0, v=0;
    char* end;
    std::ranges::transform_view range =
        reader.range()
             | std::views::drop_while([](auto& s) { return s.starts_with("#"); })
             //| std::views::filter([](auto& s) { return !s.empty(); }) // or throw error
             | std::views::transform([&](auto& s) ->
                                     std::optional<std::pair<double, double>> {
        if (s.empty())
            return std::nullopt;
        // try speed up
        t = std::strtod(s.c_str(), &end);
        if (t>0) v = std::strtod(end, nullptr);
        if (t<=0 || v==0 || errno == ERANGE) { // perhaps wrong values
            errno = 0;
            std::stringstream(s) >> t >> v; // slow
        }
        if (tlast.has_value() && tlast.value()>t) { // >=
            throw Error(QString("Time order violation: %1 then %2").arg(tlast.value()).arg(t));
        }
        tlast = t;
        size++;
        return std::optional(std::pair(t, v));
    });

    data.clear();
    data.reserve(500000); // speed up
    std::ranges::copy(range.begin(), range.end(), std::back_inserter(data));
    if (size==0) {
        throw Error("No data in file!");
    }

    // find min and max of t and v
    auto flatten = this->range()
            | std::views::filter([](auto const &op) { return op.has_value(); });
    auto ranget = flatten
            | std::views::transform([](auto const &op) { return op->first; });
    auto rangev = flatten
            | std::views::transform([](auto const &op) { return op->second; });
    auto [tmin, tmax] = std::minmax_element(ranget.begin(), ranget.end());
    auto [vmin, vmax] = std::minmax_element(rangev.begin(), rangev.end());
    this->tmin = *tmin;
    this->tmax = *tmax;
    this->vmin = *vmin;
    this->vmax = *vmax;
}
