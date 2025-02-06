#ifndef vector2D_hpp
#define vector2D_hpp
#include <cmath>
class vector2D
{
public:
    float x;
    float y;

    vector2D();
    vector2D(float x, float y);

    vector2D &add(const vector2D &vec);
    static void normalizeVector(vector2D &vec);
    // vector2D &subtract(const vector2D &vec);
    // vector2D &multiply(const vector2D &vec);
    // vector2D &divide(const vector2D &vec);
};
#endif