#include "ssdfileline.h"
#include "ssdfileheader.h"
#include "error.h"
#include "filereader.h"

#include <ranges>
#include <algorithm> // std::ranges::fold_left

SSDFileHeader::SSDFileHeader(const QString& file_name)
{
    FileReader reader(file_name);

    std::ranges::transform_view header =
        reader.range()
            | std::views::take_while([](auto& s) { return s.starts_with("#"); })
            | std::views::transform([](auto s) {
                s.erase(0,1);
                while ( !s.empty() && s.at(0)==' ') s.erase(0,1);
                return s;
    });

    auto it = header.cbegin();
    auto read_string = [&it]() -> std::string {
        const std::string& str = *it;
        it++;
        return str;
    };

    org_app_ver = read_string();
    mes_type = read_string();
    date_and_time = read_string();

    other =
        std::ranges::fold_left( // C++23
            it, header.cend(),
            std::string(),
            [](std::string s1, std::string s2) { return s1 + "\n" + s2; }
        );
}


