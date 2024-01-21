#pragma once

#ifndef LD_MATH_PI
# define LD_MATH_PI     3.141592653589793f
#endif

#ifndef LD_MATH_SQRT
# include <cmath>
# define LD_MATH_SQRT(EXPR)   std::sqrt(EXPR)
#endif

#ifndef LD_MATH_SIN
# include <cmath>
# define LD_MATH_SIN(EXPR)   std::sin(EXPR)
#endif

#ifndef LD_MATH_COS
# include <cmath>
# define LD_MATH_COS(EXPR)   std::cos(EXPR)
#endif

#ifndef LD_MATH_TAN
# include <cmath>
# define LD_MATH_TAN(EXPR)   std::tan(EXPR)
#endif

#ifndef LD_MATH_ASIN
# include <cmath>
# define LD_MATH_ASIN(EXPR)   std::asin(EXPR)
#endif

#ifndef LD_MATH_ACOS
# include <cmath>
# define LD_MATH_ACOS(EXPR)   std::acos(EXPR)
#endif

#ifndef LD_MATH_ATAN
# include <cmath>
# define LD_MATH_ATAN(EXPR)   std::atan(EXPR)
#endif