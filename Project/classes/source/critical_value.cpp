#include "critical_value.h"

#include <CGAL/squared_distance_2.h>

#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

// Constructor to initialize the polygonal curves P and Q
CriticalValue::CriticalValue(const PolygonalCurve& P, const PolygonalCurve& Q)
    : P(P), Q(Q) {
  computeAndSortAllTypes();
}

// Destructor
CriticalValue::~CriticalValue() {}

// Helper function to calculate the Euclidean distance between two points
double CriticalValue::distance(const Point_2& p1, const Point_2& p2) const {
  return sqrt(CGAL::squared_distance(p1, p2));
}

// Helper function to calculate the closest point on an edge
double CriticalValue::closestPointOnEdge(const Point_2& p, const Point_2& start,
                                         const Point_2& end) const {
  // Project point onto the line segment
  double dx = end.x() - start.x();
  double dy = end.y() - start.y();
  double len2 = dx * dx + dy * dy;
  if (len2 == 0.0) {
    return distance(p, start);  // Start and end are the same
  }

  double t = ((p.x() - start.x()) * dx + (p.y() - start.y()) * dy) / len2;
  if (t < 0.0)
    return distance(p, start);  // Closest to start
  else if (t > 1.0)
    return distance(p, end);  // Closest to end

  Point_2 proj(start.x() + t * dx, start.y() + t * dy);
  return distance(p, proj);  // Closest point on the edge
}

// Helper function to find the intersection point with the perpendicular
// bisector
Point_2 CriticalValue::findIntersectionWithPerpendicularBisector(
    const Point_2& p1, const Point_2& p2, const Point_2& start,
    const Point_2& end) const {
  // Project the start and end points of the edge onto the vector (p1 to
  // p2)
  double dx = p2.x() - p1.x();
  double dy = p2.y() - p1.y();
  double len2 = dx * dx + dy * dy;  // Squared length of the vector p1 to p2

  // If the selected points are the same, return no intersection (-1, -1)
  if (len2 == 0.0) return Point_2(-1, -1);

  // Projection for the start point of the edge
  double a = ((start.x() - p1.x()) * dx + (start.y() - p1.y()) * dy) / len2;

  // Projection for the end point of the edge
  double b = ((end.x() - p1.x()) * dx + (end.y() - p1.y()) * dy) / len2;

  // If the interval [a, b] does not contain 0.5, no intersection exists
  if ((a < 0.5 && b < 0.5) || (a > 0.5 && b > 0.5)) {
    return Point_2(-1, -1);  // No intersection
  }

  // If one of the projections is exactly 0.5, return the corresponding
  // point
  if (fabs(a - 0.5) < 1e-6 && fabs(b - 0.5) > 1e-6) {
    return start;  // First point of the edge is the intersection
  } else if (fabs(b - 0.5) < 1e-6 && fabs(a - 0.5) > 1e-6) {
    return end;  // Last point of the edge is the intersection
  }

  // If both a and b are 0.5, return the closer point to p1
  if (fabs(a - 0.5) < 1e-6 && fabs(b - 0.5) < 1e-6) {
    double distStart = CGAL::squared_distance(p1, start);
    double distEnd = CGAL::squared_distance(p1, end);
    return distStart < distEnd ? start : end;
  }

  // If [a, b] contains 0.5, calculate the internally dividing point
  // Ratio is (0.5 - a):(b - 0.5)
  double ratio1 = 0.5 - a;
  double ratio2 = b - 0.5;

  double intersectX =
      (ratio2 * start.x() + ratio1 * end.x()) / (ratio1 + ratio2);
  double intersectY =
      (ratio2 * start.y() + ratio1 * end.y()) / (ratio1 + ratio2);

  // Return the intersection point
  return Point_2(intersectX, intersectY);
}

// Function to compute Type A distances
void CriticalValue::computeTypeA() {
  // Distance between the first point of P and the first point of Q
  typeAValues.push_back(distance(P.getPoint(0), Q.getPoint(0)));

  // Distance between the last point of P and the last point of Q
  typeAValues.push_back(
      distance(P.getPoint(P.numPoints() - 1), Q.getPoint(Q.numPoints() - 1)));
}

// Function to compute Type B distances
void CriticalValue::computeTypeB() {
  int p = P.numPoints();
  int q = Q.numPoints();

  // For each point in P, find the closest point on each edge of Q
  for (int i = 0; i < p; ++i) {
    for (int j = 0; j < q - 1; ++j) {
      typeBValues.push_back(
          closestPointOnEdge(P.getPoint(i), Q.getPoint(j), Q.getPoint(j + 1)));
    }
  }

  // For each point in Q, find the closest point on each edge of P
  for (int i = 0; i < q; ++i) {
    for (int j = 0; j < p - 1; ++j) {
      typeBValues.push_back(
          closestPointOnEdge(Q.getPoint(i), P.getPoint(j), P.getPoint(j + 1)));
    }
  }
}

// Function to compute Type C distances
void CriticalValue::computeTypeC() {
  int p = P.numPoints();
  int q = Q.numPoints();

  // Compute distances for pairs of points on P and edges of Q
  for (int i = 0; i < p - 1; ++i) {
    for (int j = i + 1; j < p; ++j) {
      for (int k = 0; k < q - 1; ++k) {
        Point_2 intersection = findIntersectionWithPerpendicularBisector(
            P.getPoint(i), P.getPoint(j), Q.getPoint(k), Q.getPoint(k + 1));

        // If there is no intersection, continue to the next iteration
        if (intersection == Point_2(-1, -1)) {
          continue;
        }

        // Compute the distance to the intersection and add to typeCValues
        double distToIntersection = distance(P.getPoint(i), intersection);
        typeCValues.push_back(distToIntersection);
      }
    }
  }

  // Compute distances for pairs of points on Q and edges of P
  for (int i = 0; i < q - 1; ++i) {
    for (int j = i + 1; j < q; ++j) {
      for (int k = 0; k < p - 1; ++k) {
        Point_2 intersection = findIntersectionWithPerpendicularBisector(
            Q.getPoint(i), Q.getPoint(j), P.getPoint(k), P.getPoint(k + 1));

        // If there is no intersection, continue to the next iteration
        if (intersection == Point_2(-1, -1)) {
          continue;
        }

        // Compute the distance to the intersection and add to typeCValues
        double distToIntersection = distance(Q.getPoint(i), intersection);
        typeCValues.push_back(distToIntersection);
      }
    }
  }
}

// Function to compute all types of values, integrate them, and sort them
void CriticalValue::computeAndSortAllTypes() {
  // Compute Type A, B, and C values
  computeTypeA();
  computeTypeB();
  computeTypeC();

  // Combine all values into critical_values
  critical_values.insert(critical_values.end(), typeAValues.begin(),
                         typeAValues.end());
  critical_values.insert(critical_values.end(), typeBValues.begin(),
                         typeBValues.end());
  critical_values.insert(critical_values.end(), typeCValues.begin(),
                         typeCValues.end());

  // Sort all values in ascending order
  sort(critical_values.begin(), critical_values.end());

  // Remove duplicates
  auto last = unique(critical_values.begin(), critical_values.end());

  // Resize the vector to remove the duplicate elements
  critical_values.erase(last, critical_values.end());
}

// Getter for Type A values
const vector<double>& CriticalValue::getTypeAValues() const {
  return typeAValues;
}

// Getter for Type B values
const vector<double>& CriticalValue::getTypeBValues() const {
  return typeBValues;
}

// Getter for Type C values
const vector<double>& CriticalValue::getTypeCValues() const {
  return typeCValues;
}

// Getter for critical values (all combined and sorted)
const vector<double>& CriticalValue::getCriticalValues() const {
  return critical_values;
}
