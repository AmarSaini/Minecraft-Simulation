#include "curve_eval.h"
#include <cmath>

float factorial(int num) {

	float result = 1;

	for (int i = 1; i <= num; i++) {

		result *= i;

	}

	return result;

}

float calcBinomialCoefficient(int n, int k) {

	float result;

	result = factorial(n) / (factorial(n - k) * factorial(k));

	return result;

}

float NFunction(float i, float order, float myT) {

	// From lecture slides page 11

	// float, ui = i, u = order, uk = order-1 (previous uk)


	if (order == 1) {

		if (i <= myT && myT < i + 1) {

			return 1.0;

		}

		else {

			return 0.0;

		}

	}

	else {

		return (((myT - i) / (order - 1) * NFunction(i, order - 1, myT)) + ((i + order - myT) / (order - 1) * NFunction(i + 1, order - 1, myT)));

	}


}

GsArray<GsVec> evaluate_lagrange(int t, const GsArray<GsVec>& points) {

	// First find our starting X position, and the increment needed for t segments.

	GsArray<GsVec> myNewPoints;
	myNewPoints.size(t);

	if (points.size() == 0) {
		return myNewPoints;
	}

	float min = points[0].x;
	float max = points[0].x;

	for (int i = 0; i < points.size(); i++) {

		if (points[i].x > max) {
			max = points[i].x;
		}

		if (points[i].x < min) {
			min = points[i].x;
		}

	}



	// Increment = X Range (max-min) Divided by number of points (t-1), counting 0 as a point.

	float increment = (max - min) / (t - 1);


	// Algorithm: (SegX-oldX1) * (newX-oldX2) ... Divided by (oldXi-oldXj) * (oldXi - oldXj+1) where i!=j. All times oldYi

	float segX = min - increment;
	float segY = 0;
	

	for (int segNum = 0; segNum < t; segNum++) {

		segX += increment;
		segY = 0;

		// For every 'X'..... (Xi)

		for (int i = 0; i < points.size(); i++) {

			float fraction = 1;

			// For every 'X'..... (Xj) For comparions of denominator
			
			for (int j = 0; j < points.size(); j++) {

				if (i != j) {

					fraction *= (segX - points[j].x) / (points[i].x - points[j].x);

				}

			}

			// Sum over this fraction*f0 format
			segY += fraction * points[i].y;


		}

		myNewPoints[segNum] = GsVec(segX, segY, (float)0.0);


	}


	return myNewPoints;

}

GsArray<GsVec> evaluate_bezier(int t, const GsArray<GsVec>& points) {

	GsArray<GsVec> myNewPoints;
	myNewPoints.size(t);

	if (points.size() == 0) {
		return myNewPoints;
	}

	// For every segment, do the following:
	// Sum from i to n:   (n!/k!(n-k)!) * (1-t)^(n-i) * t^i * point

	// Each segment has a new t value from 0-1, incrementing by 1/(t-1), counting 0 as a point
	// n = number of points
	// k = our i
	// t = seg#/totalSegs
	// point = points[i]

	float increment = 1.0 / (t - 1);
	
	float ourT = 0 - increment; // Starts at 0 because it gets incremented in the loop

	for (int segNum = 0; segNum < t; segNum++) {

		ourT += increment;
		GsVec resultPoint(0.0, 0.0, 0.0);

		for (int i = 0; i < points.size(); i++) {

			float binCoe = calcBinomialCoefficient(points.size()-1, i);
			resultPoint += (binCoe * powf((1 - ourT), (points.size() - i - 1)) * powf(ourT, i)) * points[i];

		}

		myNewPoints[segNum] = resultPoint;

	}

	return myNewPoints;

}

GsArray<GsVec> evaluate_bspline(int t, int k, const GsArray<GsVec>& points) {

	GsArray<GsVec> myNewPoints;

	// t is in [2, n+1]


	// For each segment, do:
	// For each point in points, do:
	// Point * N(i)


	// N(i) = ((myT - myPointIndex) / (order-1))) * N(before) + (myPointIndex + order - myT)/(order-1) * N(after)

	float increment = 1.0 / t;

	for (float myT = 2.0; myT <= points.size(); myT += increment) {

		GsVec newPoint(0.0, 0.0, 0.0);

		for (int i = 0; i < points.size(); i++) {

			newPoint += points[i] * NFunction(i, 3, myT);

		}

		myNewPoints.push(newPoint);

	}

	return myNewPoints;

}

GsArray<GsVec> evaluate_crspline(int t, const GsArray<GsVec>& points) {

	GsArray<GsVec> myNewPoints;

	// t is in [0, n-2]

	float increment = 1.0 / t;

	// I = ( P(i+1) - P(i-1) ) / 2

	// 4 Control points:
	// The point itself
	// The point + I/3.0
	// The next point - I/3.0
	// The next point

	// P

	// P is a bezier cord of seg# - floor(seg#) of the control points


	for (float myT = 1.0; myT <= points.size()-1.9; myT += increment) {

		GsVec P(0.0, 0.0, 0.0);
		GsVec I(0.0, 0.0, 0.0);

		GsArray<GsVec> CP;

		int i = (int)myT;

		CP.push(points[i]);

		I = (points[i + 1] - points[i - 1]) / 2.0;
		
		CP.push(points[i] + I/3.0);

		// For the next point
		i++;

		I = (points[i + 1] - points[i - 1]) / 2.0;

		CP.push(points[i] - I / 3.0);

		CP.push(points[i]);

		i--;

		float dif = myT - i;

		for (int j = 0; j < CP.size(); j++) {

			float binCoe = calcBinomialCoefficient(CP.size() - 1, j);
			P += (binCoe * powf((1 - dif), (CP.size() - j - 1)) * powf(dif, j)) * CP[j];

		}

		myNewPoints.push(P);

	}

	return myNewPoints;

}

GsArray<GsVec> evaluate_bospline(int t, const GsArray<GsVec>& points) {

	GsArray<GsVec> myNewPoints;

	// t is in [0, n-2]

	float increment = 1.0 / t;


	// 4 Control points:
	// The point itself
	// The point + 1/3 * dPlus * v
	// The next point - 1/3 * dMinus * v
	// The next point

	for (float myT = 1.0; myT <= points.size()-1.9; myT += increment) {

		float dPlus;
		float dMinus;
		GsVec P;
		GsVec V;
		GsVec vPlus;
		GsVec vMinus;
		GsArray<GsVec> CP;

		int i = int(myT);

		CP.push(points[i]);

		dPlus = dist(points[i + 1], points[i]);
		vPlus = (points[i + 1] - points[i]) / dPlus;

		dMinus = dist(points[i], points[i - 1]);
		vMinus = (points[i] - points[i - 1]) / dMinus;

		V = dPlus*vMinus + dMinus*vPlus / (dPlus+dMinus);
		CP.push(points[i] + (1 / 3.0) * dPlus * V);

		i++;

		dPlus = dist(points[i + 1], points[i]);
		vPlus = (points[i + 1] - points[i]) / dPlus;

		dMinus = dist(points[i], points[i - 1]);
		vMinus = (points[i] - points[i - 1]) / dMinus;

		V = dPlus*vMinus + dMinus*vPlus / (dPlus + dMinus);
		CP.push(points[i] - (1 / 3.0) * dMinus * V);


		CP.push(points[i]);


		i--;

		float dif = myT - i;

		for (int j = 0; j < CP.size(); j++) {

			float binCoe = calcBinomialCoefficient(CP.size() - 1, j);
			P += (binCoe * powf((1 - dif), (CP.size() - j - 1)) * powf(dif, j)) * CP[j];

		}

		myNewPoints.push(P);


	}

	return myNewPoints;

}

