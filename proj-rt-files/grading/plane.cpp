#include "plane.h"
#include "ray.h"
#include <cfloat>
#include <limits>

// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
Hit Plane::Intersection(const Ray& ray, int part) const
{
     double res = dot(normal,ray.direction);
            //  Hit hEET;
    if(res) {
    //Hit hEET;
    //double t1 = dot((x1 - ray.endpoint),normal)/denom; 
         
        if (dot((x1 - ray.endpoint),normal)/res <= small_t) {
            Hit h1;
            return h1;
         }

         else {
         Hit hit;
         hit.dist = dot((x1 - ray.endpoint),normal)/res;
         hit.object = this;
         return hit; 
         }

    }
return{0,0,0};
}

vec3 Plane::Normal(const vec3& point, int part) const
{
    return normal;
}

// There is not a good answer for the bounding box of an infinite object.
// The safe thing to do is to return a box that contains everything.
Box Plane::Bounding_Box(int part) const
{
    Box b;
    b.hi.fill(std::numeric_limits<double>::max());
    b.lo=-b.hi;
    return b;
}
