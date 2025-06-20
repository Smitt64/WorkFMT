#ifndef SPIRITINFOWALKER_HPP
#define SPIRITINFOWALKER_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <sstream>
#include <string>

template<class T>
struct printer
{
    typedef T string;
    std::stringstream stm;
    void element(string const& tag, string const& value, int depth)
    {
        for (int i = 0; i < (depth*4); ++i) // indent to depth
            stm << ' ';

        stm << "tag: "<< tag;
        if (value != "")
            stm << ", value: " << value;
        std::cout << std::endl;
    }
};

typedef printer<boost::spirit::utf8_string> printer_utf8; 

inline std::string print_info(boost::spirit::info const& what)
{
    using boost::spirit::basic_info_walker;

    printer_utf8 pr;
    basic_info_walker<printer_utf8> walker(pr, what.tag, 0);
    boost::apply_visitor(walker, what.value);
    return pr.stm.str();
}

#endif