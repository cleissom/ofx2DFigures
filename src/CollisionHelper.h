/*

    ofxTableGestures (formerly OF-TangibleFramework)
    Developed for Taller de Sistemes Interactius I
    Universitat Pompeu Fabra

    Copyright (c) 2011 Daniel Gallardo Grassot <daniel.gallardo@upf.edu>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef COLLISION_HELPER_H
#define COLLISION_HELPER_H

#include <math.h>
#include <vector>
#include "Matrix.h"
#include "ofMain.h"

namespace Figures
{
    /// default minimum and maximum values for some methods
    #define SmallValue 0.00001
    #define BigValue 99999.0
    typedef std::vector<ofPoint> vector_points;

    void SetColor (ofColor color);

    unsigned int parseHexColor(std::string & color);

    void Get_rgb_from_hex(std::string & color, int &red, int &green, int &blue, int &alpha);

    ofColor GetColorFromString(std::string color);


    class CollisionHelper
    {
        public:
            static Matrix ignore_transformation_matrix;
            static bool debug_graphics;
            /// Givven a segment A(ax,ay)--B(bx,by) returns if the point P(px,py) is at the left side of the segment.
            static float IsLeft(float ax, float ay, float bx, float by, float px, float py)
            {
                return ((bx - ax) * (py - ay) - (px - ax) * (by - ay));
            }

            /// Checks a collision of the point P (x,y) with the triangle (0,0) - A - B
            static bool CollideTriangle(ofPoint A, ofPoint B, float x, float y)
            {
                if (IsLeft(0, 0, A.x, A.y, x, y) > 0 ||
                    IsLeft(A.x, A.y, B.x, B.y, x, y) > 0 ||
                    IsLeft(B.x, B.y, 0, 0, x, y) > 0)
                    return false;
                return true;
            }

            /// Checks a collision of the point P (x,y) with the triangle C - A - B
            static bool CollideTriangle(ofPoint A, ofPoint B, ofPoint C, float x, float y)
            {
                if (IsLeft(A.x, A.y, B.x, B.y, x, y) < 0 ||
                    IsLeft(B.x, B.y, C.x, C.y, x, y) < 0 ||
                    IsLeft(C.x, C.y, A.x, A.y, x, y) < 0)
                    return false;
                return true;
            }

            /// Calculates the determinant of two givven vectors.
            static float determinant(ofPoint vec1, ofPoint vec2)
            {
                return vec1.x * vec2.y - vec1.y * vec2.x;
            }

            /// Returns the collision point between the two vectors a-b and c-d.
            static ofPoint edgeIntersection(ofPoint a, ofPoint b, ofPoint c, ofPoint d)
            {
                float det = determinant(b - a, c - d);
                float t = determinant(c - a, c - d) / det;
                float u = determinant(b - a, c - a) / det;
                if ((t < 0) || (u < 0) || (t > 1) || (u > 1)) return ofPoint(-1, -1);
                //return a * (1 - t) + t * b;
                ofPoint tmp_b = b * t;
                tmp_b += 1.0f - t;
                return a * tmp_b;
            }

            /// Returns the distance from the point P to the segment A-B
            static float Point_segment_distance(ofPoint P, ofPoint A, ofPoint B)
            {
                float abx = (B.x - A.x);
                float aby = (B.y - A.y);
                float length_square = abx * abx + aby * aby;
                float r = ((P.x - A.x) * abx + (P.y - A.y) * aby) / length_square;
                float s = ((A.y - P.y) * abx - (A.x - P.x) * aby) / length_square;
                if ((r >= 0) && r <= 1)
                {
                    return (float)fabs(s) * (float)sqrt(length_square);
                }
                else
                {
                    float dist1 = (P.x - A.x) * (P.x - A.x) + (P.x - A.y) * (P.x - A.y);
                    float dist2 = (P.x - B.x) * (P.x - B.x) + (P.x - B.y) * (P.x - B.y);
                    if (dist1 < dist2)
                        return (float)sqrt(dist1);
                    else
                        return (float)sqrt(dist2);
                }
            }
    };
}
#endif // COLLISION_HELPER_H
