#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
 vec3 dir = ray.direction.normalized();
 vec3 endP = ray.endpoint - this->center;

 double b = -dot(dir,endP); 
  double c = dot(endP, endP) - (this->radius * this->radius);
  double d = (b*b) - c;
 Hit oneHit;
oneHit.part=0;
 if (d < 0) {
  oneHit.object = NULL;
oneHit.dist = 0; 

} 

else{
  double  Res1 = (b - sqrt(d));
  double  Res2 = (b + sqrt(d));

if (Res1 >= small_t) {  
oneHit.dist = Res1;
oneHit.object = this;
}

else if( Res2 >= small_t)  {
oneHit.dist = Res2;
oneHit.object = this;

}


else   {
oneHit.object = NULL;
oneHit.dist=0;

}
    
}
return oneHit;
}
vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
//  normal calculation
    return (point - this->center).normalized();
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    
    return box;
}
