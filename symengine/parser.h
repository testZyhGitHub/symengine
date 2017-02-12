#ifndef SYMENGINE_PARSER_H
#define SYMENGINE_PARSER_H

#include "parserbase.h"
#include "scanner.h"
#include <fstream>

namespace SymEngine
{

#undef Parser
class Parser: public ParserBase
{
    Scanner d_scanner;
        
    public:

        RCP<const Basic> res;

        inline Parser(std::string input)
        {
            // TODO convert strings to some stream
            std::string s = "out_string.txt";

            std::ofstream out(s);
            out << input;
            out.close();

            d_scanner.switchStreams(s, "");
            d_scanner.dval = &d_val__;
        }

        inline Parser()
        {
            d_scanner.dval = &d_val__;
        }

        std::map<const std::string, const RCP<const Basic>> constants = {

            {"e", E},
            {"E", E},
            {"EulerGamma", EulerGamma},
            {"Catalan", Catalan},
            {"GoldenRatio", GoldenRatio},
            {"pi", pi},
            {"I", I}
        };

        // reference :
        // http://stackoverflow.com/questions/30393285/stdfunction-fails-to-distinguish-overloaded-functions
        typedef RCP<const Basic> (*single_arg_func)(const RCP<const Basic> &);
        typedef RCP<const Basic> (*double_arg_func)(const RCP<const Basic> &,
                                                    const RCP<const Basic> &);

        // cast overlaoded functions below to single_arg, double_arg before they can
        // be used in the map
        single_arg_func single_casted_log = log;
        single_arg_func single_casted_zeta = zeta;

        double_arg_func double_casted_log = log;
        double_arg_func double_casted_zeta = zeta;

        // maps string to corresponding single argument function
        std::map<std::string,
                 std::function<RCP<const Basic>(const RCP<const Basic> &)>>
            single_arg_functions = {

                {"", [](const RCP<const Basic> &x) {return x; }},

                {"sin", sin},
                {"cos", cos},
                {"tan", tan},
                {"cot", cot},
                {"csc", csc},
                {"sec", sec},

                {"asin", asin},
                {"acos", acos},
                {"atan", atan},
                {"asec", asec},
                {"acsc", acsc},
                {"acot", acot},

                {"sinh", sinh},
                {"cosh", cosh},
                {"tanh", tanh},
                {"coth", coth},
                {"sech", sech},
                {"csch", csch},

                {"asinh", asinh},
                {"acosh", acosh},
                {"atanh", atanh},
                {"asech", asech},
                {"acoth", acoth},
                {"acsch", acsch},

                {"gamma", gamma},
                {"sqrt", sqrt},
                {"abs", abs},
                {"exp", exp},
                {"erf", erf},
                {"erfc", erfc},
                {"loggamma", loggamma},
                {"lambertw", lambertw},
                {"dirichlet_eta", dirichlet_eta},
                {"ln", single_casted_log},
                {"log", single_casted_log},
                {"zeta", single_casted_zeta}};

        // maps string to corresponding double argument function
        std::map<std::string,
                 std::function<RCP<const Basic>(const RCP<const Basic> &,
                                                const RCP<const Basic> &)>>
            double_arg_functions = {

                {"pow", pow},
                {"beta", beta},
                {"log", double_casted_log},
                {"zeta", double_casted_zeta},
                {"lowergamma", lowergamma},
                {"uppergamma", uppergamma},
                {"polygamma", polygamma},
                {"kronecker_delta", kronecker_delta}};

        // maps string to corresponding multi argument function
        std::map<std::string, std::function<RCP<const Basic>(vec_basic &)>>
            multi_arg_functions = {

                {"max", max}, {"min", min}, {"levi_civita", levi_civita}};

        int parse();

    private:
        void error(char const *msg) {}  // called on (syntax) errors
        int lex();                      // returns the next token from the
                                        // lexical scanner. 
        void print();                   // use, e.g., d_token, d_loc

        void executeAction(int ruleNr);
        void errorRecovery();
        int lookup(bool recovery);
        void nextToken();
        void print__();
        void exceptionHandler__(std::exception const &exc)
        {
          std::cout << exc.what() << '\n';
        }
};

inline RCP<const Basic> parse(const std::string &s)
{
    Parser pp(s);

    if (pp.parse() == 0)
        return pp.res;
    
    throw ParseError("Parsing Unsuccessful");
}

RCP<const Basic> parse_old(const std::string &s);
}

#endif
