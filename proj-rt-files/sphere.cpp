#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
 

 double a = dot(ray.direction.normalized(), ray.direction.normalized());
 double b = 2 * dot(ray.direction.normalized(),ray.endpoint - center);
 double c = dot(ray.endpoint - center, ray.endpoint - center) - (radius*radius); 
  

  int Res1 = (-b - (sqrt(b*b-(4*a*c))))/ (2*a); 
  int Res2 = (-b + (sqrt(b*b-(4*a*c))))/ (2*a);

if (Res1 && Res2 < 0) {  //no intersection
//no hit
Hit noHit;
return noHit;
}

else if(Res1 < 0 && Res2 > 0)  {
Hit hitRes2;
hitRes2.dist = Res2;
hitRes2.object = this;
return hitRes2;
}


else if(Res1 > 0 && Res2 < 0)  {
Hit hitRes1;
hitRes1.dist = Res1;
hitRes1.object = this;
return hitRes1;
}

else {
if(Res1 == Res2 || Res1 != Res2) {
  if(Res1>Res2) {
    Hit one;
    one.dist = Res2;
    one.object = this;
    return one;
   }
   else {
    Hit two;
    two.dist = Res2;
    two.object = this;
    return two;


   }


}
}

return {0,0,0};    
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
     // compute the normal direction
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    
    return box;
}
