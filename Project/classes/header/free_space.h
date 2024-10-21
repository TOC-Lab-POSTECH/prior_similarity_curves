#ifndef FREE_SPACE_H
#define FREE_SPACE_H

#include <utility>
#include <vector>

#include "polygonal_curve.h"

using namespace std;

typedef pair<Point_2, Point_2> PointPair;
typedef vector<PointPair> PointPairVector;
class FreeSpace {
 public:
  // Constructor to initialize with two polygonal curves and an epsilon value
  FreeSpace(const PolygonalCurve& P, const PolygonalCurve& Q, double epsilon);

  // Destructor
  ~FreeSpace();

  // Getter
  const PolygonalCurve& getCurveP() const;
  const PolygonalCurve& getCurveQ() const;
  double getEpsilon() const;
  const PointPairVector& getL() const;
  const PointPairVector& getB() const;
  // Setter
  void setEpsilon(double newEpsilon);

  void computeFreeSpace();

 private:
  PolygonalCurve P;  // Polygonal curve P
  PolygonalCurve Q;  // Polygonal curve Q
  double epsilon;    // Epsilon value

  PointPairVector L;  // Results for P
  PointPairVector B;  // Results for Q

  void processCurveForL();
  void processCurveForB();
  pair<int, vector<float>> checkPointsOnEdge(const Point_2& start,
                                             const Point_2& end,
                                             const Point_2& point) const;
};

#endif  // FREE_SPACE_H
