#pragma once
#include "../Library/Variable.h"
#include "CoefficientParser.h"
#include <map>

template<typename CoefficientType, typename VariableOrder = VariableOrders::AsciiOrder, typename DegreeType = int64_t,
         typename VariableNumberType = int32_t>
class MonomialParser {
    using Var = Variable<VariableNumberType, VariableOrder>;

public:
    void parse(std::string& s, CoefficientType* coefficient, std::map<Var, DegreeType>* var_store) {
        if (s.empty()) { return; }
        *coefficient = CoefficientParser<CoefficientType>().parse(s);
        parse_variables(s, var_store);
    }

private:
    void parse_variables(const std::string& s, std::map<Var, DegreeType>* var_store) {
        size_t l = 0;
        while (l < s.size()) {
            size_t r = l;
            char var_name = s[l];
            VariableNumberType num = -1;
            if (r + 1 < s.size() && s[r + 1] == '_') {
                num = 0;
                r = num_reader::read_num(s, num, r + 2) - 1;
            }
            DegreeType var_degree = 1;
            if (r + 1 < s.size() && s[r + 1] == '^') {
                var_degree = 0;
                r = num_reader::read_num(s, var_degree, r + 2) - 1;
            }
            Var var{var_name, num};
            if (!(*var_store).count(var)) { (*var_store)[var] = 0; }
            (*var_store)[var] += var_degree;
            l = r + 1;
        }
    }
};
