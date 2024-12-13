#ifndef POLYGONAL_CURVE_H
#define POLYGONAL_CURVE_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_2.h>

#include <vector>

// CGAL Kernel (default Epick kernel for exact predicates)
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;  // 2D point type from CGAL

class PolygonalCurve {
 public:
  // Constructor to initialize the curve with a list of points
  PolygonalCurve(const std::vector<Point_2>& points);

  // Copy constructor
  PolygonalCurve(const PolygonalCurve& P_);

  // Method to add a point to the curve
  void addPoint(const Point_2& point);

  // Method to get the number of points in the curve
  std::size_t numPoints() const;

  // Method to access a point at a specific index
  const Point_2& getPoint(std::size_t index) const;

  // Method to compute the total length of the curve
  double curveLength() const;

  // Print the points of the curve (for debugging or output)
  void printCurve() const;

  // Shift the origin of the grid
  void shiftOrigin(const Point_2& newOrigin);

  // Scale the grid on which the points of the curve lie
  void scaleGrid(double scalingFactor);

  // Convert coordinates of points to floor integer values
  void floorCoordinates();

 private:
  std::vector<Point_2> m_points;  // Vector to store the sequence of points
};

#endif  // POLYGONAL_CURVE_H
