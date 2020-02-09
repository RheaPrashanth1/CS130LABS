#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    //  vec3 color = shader->Shade_Surface(ray, intersection_point,
    //    normal, recursion_depth);
int depth = recursion_depth;
    if (depth >= world.recursion_depth_limit) {
        return (1 - reflectivity) * shader->Shade_Surface(ray, intersection_point,normal, depth);
    }

    
    else {
      Ray r(intersection_point, ray.direction - 2 * dot(ray.direction, normal) * normal);

    return (1 - reflectivity) * shader->Shade_Surface(ray, intersection_point,normal, depth) + reflectivity * world.Cast_Ray(r, ++depth);
}
}
