#include "polygonal_curve.h"

#include <cmath>
#include <iostream>

using namespace std;

// Constructor: initializes the polygonal curve with the given points
PolygonalCurve::PolygonalCurve(const vector<Point_2>& points)
    : m_points(points) {}

// Copy constructor: performs a deep copy
PolygonalCurve::PolygonalCurve(const PolygonalCurve& P_)
    : m_points(P_.m_points) {}

// Adds a point to the polygonal curve
void PolygonalCurve::addPoint(const Point_2& point) {
  m_points.push_back(point);
}

// Returns the number of points in the polygonal curve
size_t PolygonalCurve::numPoints() const { return m_points.size(); }

// Returns the point at a given index
const Point_2& PolygonalCurve::getPoint(size_t index) const {
  if (index >= m_points.size()) {
    throw out_of_range("Index out of range.");
  }
  return m_points[index];
}

// Computes the length of the polygonal curve (sum of Euclidean distances
// between consecutive points)
double PolygonalCurve::curveLength() const {
  double length = 0.0;
  for (size_t i = 1; i < m_points.size(); ++i) {
    double dx = m_points[i].x() - m_points[i - 1].x();
    double dy = m_points[i].y() - m_points[i - 1].y();
    length += sqrt(dx * dx + dy * dy);  // Euclidean distance
  }
  return length;
}

// Prints the points of the polygonal curve
void PolygonalCurve::printCurve() const {
  for (const auto& point : m_points) {
    cout << "(" << point.x() << ", " << point.y() << ")" << endl;
  }
}

// Shifts the origin of the grid by subtracting the coordinates of newOrigin
// from each point of the polygonal curve
void PolygonalCurve::shiftOrigin(const Point_2& newOrigin) {
  for (auto& point : m_points) {
    double newX = point.x() - newOrigin.x();
    double newY = point.y() - newOrigin.y();
    point = Point_2(newX, newY);  // Update the point
  }
}

// Scales the grid on which the points of the curve lie
void PolygonalCurve::scaleGrid(double scalingFactor) {
  if (scalingFactor == 0) {
    throw invalid_argument("Scaling factor cannot be zero.");
  }

  for (auto& point : m_points) {
    double scaledX = point.x() / scalingFactor;
    double scaledY = point.y() / scalingFactor;
    point = Point_2(scaledX, scaledY);  // Update the point
  }
}

// Converts the coordinates of points to floor integer values
void PolygonalCurve::floorCoordinates() {
  for (auto& point : m_points) {
    double flooredX = floor(point.x());
    double flooredY = floor(point.y());
    point = Point_2(flooredX, flooredY);  // Update the point
  }
}
