#ifndef POLYGONAL_CURVE_H
#define POLYGONAL_CURVE_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_2.h>

#include <vector>

using namespace std;

// CGAL Kernel (default Epick kernel for exact predicates)
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;  // 2D point type from CGAL

class PolygonalCurve {
 public:
  // Constructor to initialize the curve with a list of points
  PolygonalCurve(const vector<Point_2>& points);

  // Method to add a point to the curve
  void addPoint(const Point_2& point);

  // Method to get the number of points in the curve
  size_t numPoints() const;

  // Method to access a point at a specific index
  const Point_2& getPoint(size_t index) const;

  // Method to compute the total length of the curve
  double curveLength() const;

  // Print the points of the curve (for debugging or output)
  void printCurve() const;

 private:
  vector<Point_2> m_points;  // Vector to store the sequence of points
};

#endif  // POLYGONAL_CURVE_H
