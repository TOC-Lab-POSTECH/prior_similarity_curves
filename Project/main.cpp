#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>

#include <fstream>
#include <limits>
#include <random>

#include "critical_value.h"
#include "decision_problem.h"
#include "fdistance.h"
#include "free_space.h"
#include "ged.h"
#include "polygonal_curve.h"

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;

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

void testGED(const vector<Point_2>& pointsP, const vector<Point_2>& pointsQ) {
  PolygonalCurve P(pointsP);
  PolygonalCurve Q(pointsQ);

  // Compute the approximated GED
  double gedValue = GED::computeSquareRootApproxGED(P, Q);

  // Print the result
  cout << "Approximated GED: " << gedValue << endl;
}

vector<Point_2> generateRandomPoints(size_t numPoints, double minCoord,
                                     double maxCoord) {
  vector<Point_2> points;
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(minCoord, maxCoord);

  for (size_t i = 0; i < numPoints; ++i) {
    double x = dis(gen);
    double y = dis(gen);
    points.emplace_back(Point_2(x, y));
  }

  return points;
}

int main(int, char**) {
  // Define multiple sets of points for testing

  // Test Case 1: Simple linear curves
  vector<Point_2> pointsP1 = {Point_2(0.0, 0.0), Point_2(3.0, 3.0)};
  vector<Point_2> pointsQ1 = {Point_2(1.0, 0.0), Point_2(3.0, 2.0)};

  // Test Case 2: Curves with more complex paths
  vector<Point_2> pointsP2 = {Point_2(0.0, 0.0), Point_2(2.0, 1.0),
                              Point_2(4.0, 2.0), Point_2(6.0, 1.0)};
  vector<Point_2> pointsQ2 = {Point_2(0.0, 1.0), Point_2(2.0, 3.0),
                              Point_2(4.0, 2.0), Point_2(6.0, 0.0)};

  // Test Case 3: Horizontal and vertical lines
  vector<Point_2> pointsP3 = {Point_2(0.0, 0.0), Point_2(5.0, 0.0)};
  vector<Point_2> pointsQ3 = {Point_2(0.0, 1.0), Point_2(0.0, -2.0)};

  // Test Case 4: Zigzag patterns
  vector<Point_2> pointsP4 = {Point_2(0.0, 0.0), Point_2(1.0, 2.0),
                              Point_2(2.0, 0.0), Point_2(3.0, 2.0)};
  vector<Point_2> pointsQ4 = {Point_2(0.0, 1.0), Point_2(1.0, -1.0),
                              Point_2(2.0, 1.0), Point_2(3.0, -1.0)};

  // Test Case 5: Two identical curves
  vector<Point_2> pointsP5 = {Point_2(0.0, 0.0), Point_2(1.0, 1.0),
                              Point_2(2.0, 2.0)};
  vector<Point_2> pointsQ5 = {Point_2(0.0, 0.0), Point_2(1.0, 1.0),
                              Point_2(2.0, 2.0)};

  // Test Case 6: Randomly generated complex point sequences
  vector<Point_2> pointsP6 = generateRandomPoints(256, -2.0, 2.0);
  vector<Point_2> pointsQ6 = generateRandomPoints(256, -2.0, 2.0);

  // Run tests
  cout << "Test Case 1: Simple Linear Curves" << endl;
  testFDistance(pointsP1, pointsQ1);
  testGED(pointsP1, pointsQ1);

  cout << "\nTest Case 2: Complex Paths" << endl;
  testFDistance(pointsP2, pointsQ2);
  testGED(pointsP2, pointsQ2);

  cout << "\nTest Case 3: Horizontal and Vertical Lines" << endl;
  testFDistance(pointsP3, pointsQ3);
  testGED(pointsP3, pointsQ3);

  cout << "\nTest Case 4: Zigzag Patterns" << endl;
  testFDistance(pointsP4, pointsQ4);
  testGED(pointsP4, pointsQ4);

  cout << "\nTest Case 5: Identical Curves" << endl;
  testFDistance(pointsP5, pointsQ5);
  testGED(pointsP5, pointsQ5);

  cout << "\nTest Case 6: Complex Random Sequences" << endl;
  testFDistance(pointsP6, pointsQ6);
  testGED(pointsP6, pointsQ6);

  return 0;
}