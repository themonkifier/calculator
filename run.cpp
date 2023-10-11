#include "run.hpp"

int main(int argc, char const *argv[])
{
    std::string input;
    std::getline(std::cin, input);
    setup_units();

    PrintFormat pf = PrintFormat::Default;

    std::deque<Token> tokens = tokenize(input, pf);

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

    return 0;
}