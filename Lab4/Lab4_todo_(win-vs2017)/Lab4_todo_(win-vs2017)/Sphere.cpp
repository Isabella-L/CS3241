//============================================================
// STUDENT NAME: Lu Sicheng
// NUS User ID.: E0564634
// COMMENTS TO GRADER:
//
// ============================================================

#include "Sphere.h"
#include <cmath>

using namespace std;

bool Sphere::hit(const Ray &r, double tmin, double tmax,
                 SurfaceHitRecord &rec) const {
  //***********************************************
  //*********** WRITE YOUR CODE HERE **************
  //***********************************************
  Vector3d origin_sphere = r.origin() - center;

  double a = dot(r.direction(), r.direction());
  double b = 2 * dot(origin_sphere, r.direction());
  double c = dot(origin_sphere, origin_sphere) - radius * radius;
  double discriminant = b * b - 4 * a * c;
  double t = 0;

  if (discriminant < 0) {
    return false;
  } else if (discriminant == 0) {
    t = -b / 2 / a;
  } else {
    double t_max = (-b + pow(discriminant, 0.5)) / (2 * a);
    double t_min = (-b - pow(discriminant, 0.5)) / (2 * a);

    if (t_max < tmin && t_min < tmin) {
      return false;
    } else {
      double t = t_min > 0 ? t_min : t_max;
      if (t < tmin || t > tmax) {
        return false;
      }
      rec.t = t;
      rec.p = r.pointAtParam(t);
      Vector3d normal = rec.p - center;
      rec.normal = normal / normal.length();
      rec.material = material;
      return true;
    }
  }
}

bool Sphere::shadowHit(const Ray &r, double tmin, double tmax) const {
  //***********************************************
  //*********** WRITE YOUR CODE HERE **************
  //***********************************************
  Vector3d new_origin = r.origin() - center;

  double a = 1;
  double b = 2 * dot(r.direction(), new_origin);
  double c = dot(new_origin, new_origin) - radius * radius;

  double discriminant = b * b - 4 * a * c;

  if (discriminant < 0) {
    return false;
  } else {
    double t0 = (-b + pow(discriminant, 0.5)) / (2 * a);
    double t1 = (-b - pow(discriminant, 0.5)) / (2 * a);
    double t;
    if (t0 < tmin && t1 < tmin) {
      return false;
    } else if (t0 >= tmin && t1 <= tmin) {
      t = t0;
    } else if (t0 <= tmin && t1 >= tmin) {
      t = t1;
    } else {
      t = min(t0, t1);
    }

    return (t <= tmax);
  }
}
