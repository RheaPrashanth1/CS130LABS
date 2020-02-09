#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"
#include <cmath>

  vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 ambient, diffuse, specular, color, lightCol;

     ambient =  world.ambient_color * world.ambient_intensity *color_ambient;

// lightCol = world.lights[i]->Emitted_Light(l); 
     unsigned i = 0;   
    //Point p; 

       for ( i = 0; i < world.lights.size(); i++) { 
        vec3 l = world.lights[i]->position - intersection_point;

        Ray check_shadow(intersection_point, l);
        //check_shadow.endpoint = intersection_point;       

       // check_shadow.direction = l;
       lightCol = world.lights[i]->Emitted_Light(l);
  
       check_shadow.endpoint = intersection_point;
        Hit objhit = world.Closest_Intersection(check_shadow);
        vec3 objToHit = check_shadow.Point(objhit.dist) - intersection_point;
        if (!world.enable_shadows || (!objhit.object|| objToHit.magnitude() > l.magnitude())) {

        //vec3 r = (-l + 2 * dot(l, normal) * normal).normalized();

         //diffuse += color_diffuse  * world.lights[i]->Emitted_Light(l) * std::max(dot(normal, l.normalized()), 0.0);

//
     


    specular += color_specular * lightCol * std::pow(std::max(dot((-l + 2 * dot(l, normal) * normal).normalized(), -(ray.direction)), 0.0), specular_power);
          diffuse += color_diffuse  * lightCol * std::max(dot(normal, l.normalized()), 0.0);
   }
        
    }


    return ambient+ diffuse + specular;
}
