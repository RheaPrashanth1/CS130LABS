#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{   
     Hit closest_hit;
     Hit hit;
    hit.object = 0;
   closest_hit.object = 0;
 // iterate through each object in objects vector, find hits, of the hits find the closest one    
    double min_t = std::numeric_limits<double>::max();
    int PART = 0;
    for (Object *obj : objects) {     
       hit = (*obj).Intersection(ray,PART);
          if (hit.object) {
           if (hit.dist > small_t && hit.dist <=  min_t){
            closest_hit = hit; 
            min_t = hit.dist;
            // closest_hit = hit; 

          }
       }
     }
    
    return closest_hit;
 
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
 
   // set up the initial view ray here
 
    Ray ray(camera.position,(camera.World_Position(pixel_index)-(camera.position)).normalized() );
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
  return;
}


void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    vec3 color;
    Hit closestHit = Closest_Intersection(ray);
    const Object *OBJ = closestHit.object;   
    vec3 randVec1;
    vec3 randVec2;
 
 //   Ray *noIntRay = new Ray(randVec1, randVec2);

 if(!OBJ) {
   //   color = OBJ->material_shader->Shade_Surface(ray, ray.Point(closestHit.dist),OBJ->Normal(ray.Point(closestHit.dist),0),recursion_depth);       
 
   color = background_shader->Shade_Surface(ray ,randVec1, randVec2,recursion_depth);

    } 

  else {
    //color = background_shader->Shade_Surface(ray ,randVec1, randVec2,recursion_depth);
      color = OBJ->material_shader->Shade_Surface(ray, ray.Point(closestHit.dist),OBJ->Normal(ray.Point(closestHit.dist),0),recursion_depth);      
   
 }    
    return color;
//vec3 checkingError;
//return checkingError;
}

void Render_World::Initialize_Hierarchy()
{
    // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();

}
