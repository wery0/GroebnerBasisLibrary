# GroebnerBasisLibrary

Implemented primitives for working with ideals of polynomial rings of several variables and algorithms for finding various Groebner
bases. Primitives are:

1). **Fields**: **Fraction** (for working with rational numbers), **Mint** (modulo numbers).

2). **Variable**: represents a variable of the form `[A − Za − z](_(\d))+`.

3). **Monomial**: product of variables and coefficient. Represents a monomial of the form `(Coefficient)?(Variable(^(\d)+)?)+
`.

4). **Orders**: **VariableOrders** and **MonomialOrders**. Use them to change order of variables inside monomial and monomials inside polynomial.

5). **Parsers**: **CoefficientParser** and **MonomialParser** for parsing `std::string` to primitives.

6). **Polynomial**: sum of monomials.

7). **Ideal**: represents ideal of polynomial ring of several variables `K[x_1, ..., x_n]`.

Implemented Buchberger's algorithm with S-polynomials optimization for finding arbitrary Groebner basis, as well as functions for turning an arbitrary Groebner basis into a minimal Groebner basis and a minimal Groebner basis into a reduced Groebner basis.

Everything is tested by unit tests.

