#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>

#include <fstream>
#include <limits>

#include "critical_value.h"
#include "decision_problem.h"
#include "fdistance.h"
#include "free_space.h"
#include "polygonal_curve.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;

typedef CGAL::Point_set_3<Point> Point_set;

void print_point_set(const Point_set& point_set) {
  std::cerr << "Content of point set:" << std::endl;
  for (Point_set::const_iterator it = point_set.begin(); it != point_set.end();
       ++it)
    std::cerr << "* Point " << *it << ": "
              << point_set.point(*it)  // or point_set[it]
              << " with normal " << point_set.normal(*it) << std::endl;
}

void testFDistance(const std::vector<Point_2>& pointsP,
                   const std::vector<Point_2>& pointsQ) {
  // Create polygonal curves P and Q using the provided points
  PolygonalCurve P(pointsP);
  PolygonalCurve Q(pointsQ);

  // Create the FDistance object to compute the Fréchet distance
  FDistance fDistance(P, Q);

  // Output the computed F-distance
  double result = fDistance.getFDistance();
  if (result != -1.0) {
    std::cout << "Computed Fréchet Distance: " << result << std::endl;
  } else {
    std::cout << "No valid Fréchet distance found (f-distance = -1)."
              << std::endl;
  }
}

int main(int, char**) {
  // Define multiple sets of points for testing

  // Test Case 1: Simple linear curves
  std::vector<Point_2> pointsP1 = {Point_2(0.0, 0.0), Point_2(3.0, 3.0)};
  std::vector<Point_2> pointsQ1 = {Point_2(1.0, 0.0), Point_2(3.0, 2.0)};

  // Test Case 2: Curves with more complex paths
  std::vector<Point_2> pointsP2 = {Point_2(0.0, 0.0), Point_2(2.0, 1.0),
                                   Point_2(4.0, 2.0), Point_2(6.0, 1.0)};
  std::vector<Point_2> pointsQ2 = {Point_2(0.0, 1.0), Point_2(2.0, 3.0),
                                   Point_2(4.0, 2.0), Point_2(6.0, 0.0)};

  // Test Case 3: Horizontal and vertical lines
  std::vector<Point_2> pointsP3 = {Point_2(0.0, 0.0), Point_2(5.0, 0.0)};
  std::vector<Point_2> pointsQ3 = {Point_2(0.0, 1.0), Point_2(0.0, -2.0)};

  // Test Case 4: Zigzag patterns
  std::vector<Point_2> pointsP4 = {Point_2(0.0, 0.0), Point_2(1.0, 2.0),
                                   Point_2(2.0, 0.0), Point_2(3.0, 2.0)};
  std::vector<Point_2> pointsQ4 = {Point_2(0.0, 1.0), Point_2(1.0, -1.0),
                                   Point_2(2.0, 1.0), Point_2(3.0, -1.0)};

  // Test Case 5: Two identical curves
  std::vector<Point_2> pointsP5 = {Point_2(0.0, 0.0), Point_2(1.0, 1.0),
                                   Point_2(2.0, 2.0)};
  std::vector<Point_2> pointsQ5 = {Point_2(0.0, 0.0), Point_2(1.0, 1.0),
                                   Point_2(2.0, 2.0)};

  // Run tests
  std::cout << "Test Case 1: Simple Linear Curves" << std::endl;
  testFDistance(pointsP1, pointsQ1);

  std::cout << "\nTest Case 2: Complex Paths" << std::endl;
  testFDistance(pointsP2, pointsQ2);

  std::cout << "\nTest Case 3: Horizontal and Vertical Lines" << std::endl;
  testFDistance(pointsP3, pointsQ3);

  std::cout << "\nTest Case 4: Zigzag Patterns" << std::endl;
  testFDistance(pointsP4, pointsQ4);

  std::cout << "\nTest Case 5: Identical Curves" << std::endl;
  testFDistance(pointsP5, pointsQ5);

  return 0;
}