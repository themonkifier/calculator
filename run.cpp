#include "run.hpp"

int main(int argc, char const *argv[])
{
    std::string input;
    std::getline(std::cin, input);
    setup_units();

    PrintFormat pf = PrintFormat::Default;

    std::deque<Token> tokens = tokenize(input, pf);

    // for (auto it = tokens.begin(); it != tokens.end(); it++)
    // {
    //     if (it->index() == 0) std::cout << std::get<0>((const std::variant<Number, std::string>&) *it) << " ";
    //     else std::cout << std::get<1>((const std::variant<Number, std::string>&) *it) << " ";
    // }
    std::cout << std::endl;

    tokens = convert_to_rpn(tokens);
    

    if (pf == PrintFormat::Commas)
    {
        std::locale commas(std::locale(), new comma_numpunct());
        std::cout.imbue(commas);
    }
    else if (pf == PrintFormat::Spaces)
    {
        std::locale spaces(std::locale(), new space_numpunct());
        std::cout.imbue(spaces);
    }
    Number res = simplify_rpn(tokens);
    std::cout << "\n\n\n";
    std::cout << res.val << " " << res.unit << std::endl;

    // for (auto it = units.begin(); it != units.end(); it++) std::cout << *it << " ";
    // std::cout << std::endl;

    return 0;
}