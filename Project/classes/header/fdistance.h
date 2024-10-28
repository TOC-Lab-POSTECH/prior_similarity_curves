#ifndef FDISTANCE_H
#define FDISTANCE_H

#include "critical_value.h"
#include "decision_problem.h"

class FDistance {
 public:
  // Constructor to initialize with two polygonal curves
  FDistance(const PolygonalCurve& P, const PolygonalCurve& Q);

  // Getter
  double getFDistance() const;

 private:
  PolygonalCurve P;           // Polygonal curve P
  PolygonalCurve Q;           // Polygonal curve Q
  CriticalValue criticalVal;  // Critical values object
  DecisionProblem decision;   // Decision problem object
  double fDistance;           // Computed F-distance

  // Helper function to perform binary search on critical values
  void computeFDistance();
};

#endif  // FDISTANCE_H
