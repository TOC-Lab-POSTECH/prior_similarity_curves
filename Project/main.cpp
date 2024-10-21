#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>

#include <fstream>
#include <limits>

#include "critical_value.h"
#include "decision_problem.h"
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

int main(int, char**) {
  // Define points for Polygonal Curve P
  std::vector<Point_2> pointsP = {Point_2(0, 0), Point_2(1, 1), Point_2(2, 2),
                                  Point_2(3, 3)};
  PolygonalCurve P(pointsP);

  // Define points for Polygonal Curve Q
  std::vector<Point_2> pointsQ = {Point_2(1, -1), Point_2(2, 0), Point_2(3, 1),
                                  Point_2(4, 2)};
  PolygonalCurve Q(pointsQ);

  // Set the epsilon value
  double epsilon = 1.5;

  // Create a DecisionProblem object
  DecisionProblem decisionProblem(P, Q, epsilon);

  // Check if a monotone curve exists
  bool result = decisionProblem.doesMonotoneCurveExist();

  // Output the result
  if (result) {
    std::cout
        << "A monotone curve exists between the polygonal curves with epsilon "
        << epsilon << "." << std::endl;
  } else {
    std::cout
        << "No monotone curve exists between the polygonal curves with epsilon "
        << epsilon << "." << std::endl;
  }

  // Test updating epsilon and rechecking
  double newEpsilon = 1.4;
  decisionProblem.setEpsilon(newEpsilon);
  result = decisionProblem.doesMonotoneCurveExist();

  // Output the new result
  if (result) {
    std::cout << "With updated epsilon " << newEpsilon
              << ", a monotone curve exists." << std::endl;
  } else {
    std::cout << "With updated epsilon " << newEpsilon
              << ", no monotone curve exists." << std::endl;
  }

  return 0;
}