
#include <iostream>

#include <algorithm>
#include <ranges>

auto print_subrange = [](std::ranges::viewable_range auto&& r)
{
    std::cout << '[';
    for (int pos{}; auto elem : r)
        std::cout << (pos++ ? " " : "") << elem;
    std::cout << "] ";
};

int main(int argc, char *argv[])
{
    std::cout << "test" << std::endl;

    const auto v = {1, 2, 3, 4, 5, 6};

    for (const unsigned width : std::views::iota(1U, 2U + v.size()))
    {
        auto const chunks = v | std::views::chunk(width);
        std::cout << "chunk(" << width << "): ";
        std::ranges::for_each(chunks, print_subrange);
        std::cout << '\n';
    }

    return 0;
}
