#ifndef CURVE_EVAL
#define CURVE_EVAL

#include "gsim/gs.h"
#include "gsim/gs_array.h"
#include "gsim/gs_color.h"
#include "gsim/gs_vec.h"

GsArray<GsVec> evaluate_lagrange(int t, const GsArray<GsVec>& points);
GsArray<GsVec> evaluate_bezier(int t, const GsArray<GsVec>& points);

GsArray<GsVec> evaluate_bspline(int t, int k, const GsArray<GsVec>& points);
GsArray<GsVec> evaluate_crspline(int t, const GsArray<GsVec>& points);
GsArray<GsVec> evaluate_bospline(int t, const GsArray<GsVec>& points);

#endif