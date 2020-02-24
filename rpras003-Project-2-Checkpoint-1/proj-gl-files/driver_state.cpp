#include "driver_state.h"
#include <cstring>

driver_state::driver_state()
{
}

driver_state::~driver_state()
{
    delete [] image_color;
    delete [] image_depth;
}

// This function should allocate and initialize the arrays that store color and
// depth.  This is not done during the constructor since the width and height
// are not known when this class is constructed.
void initialize_render(driver_state& state, int width, int height)
{
    state.image_width=width;
    state.image_height=height;
    state.image_depth=0;
   int dimensions = width * height;   
 state.image_color = new pixel[dimensions];
    
for(size_t i = 0; i < width * height; i++)
 state.image_color[i] = make_pixel(0, 0, 0);
}  




// This function will be called to render the data that has been stored in this class.
// Valid values of type are:
//   render_type::triangle - Each group of three vertices corresponds to a triangle.
//   render_type::indexed -  Each group of three indices in index_data corresponds
//                           to a triangle.  These numbers are indices into vertex_data.
//   render_type::fan -      The vertices are to be interpreted as a triangle fan.
//   render_type::strip -    The vertices are to be interpreted as a triangle strip.
void render(driver_state& state, render_type type)
{
int k = 0;
    const data_geometry *tri[3];
    
//auto *tri = new data_geometry[3];
data_geometry temp_dg[3];

   auto first_vertex = state.vertex_data;
    auto is_first_vertex = true;
    data_vertex in[3];

    switch(type) {
        case render_type::triangle: {
            
            for (int i = 0; i < state.num_vertices / 3; i++) {
                for (int j = 0; j < 3; j++, k += state.floats_per_vertex) {
                    in[j].data = &state.vertex_data[k];
                    temp_dg[j].data = in[j].data;
                    state.vertex_shader(in[j], temp_dg[j], state.uniform_data);
                    tri[j] = &temp_dg[j];
                }
                
                    rasterize_triangle(state, (const data_geometry**) &tri);
            }
            break;
          }
     }
}


 /*auto *tri = new data_geometry[3];
 auto ptr = state.vertex_data;
 data_vertex in{};

 switch(type) {
 case render_type::triangle:
 for(size_t i = 0, j = 0; i < state.num_vertices; i++, j++) {
 tri[i].data = ptr;
 in.data = ptr;
 state.vertex_shader(in, tri[i], state.uniform_data);
 if(j == 2) {

 rasterize_triangle(state, (const data_geometry**) &tri);
 j = 0;
 }
 ptr += state.floats_per_vertex;
 }
 break;
 case render_type::indexed:
 break;
 case render_type::fan:
 break;
 case render_type::strip:
 break;
 default:
 break;
 }

 delete [] tri;
*/
//}



// This function clips a triangle (defined by the three vertices in the "in" array).
// It will be called recursively, once for each clipping face (face=0, 1, ..., 5) to
// clip against each of the clipping faces in turn.  When face=6, clip_triangle should
// simply pass the call on to rasterize_triangle.
void clip_triangle(driver_state& state, const data_geometry* in[3],int face)
{
    if(face==6)
    {
        rasterize_triangle(state, in);
        return;
    }
       clip_triangle(state,in,face+1);
}

// Rasterize the triangle defined by the three vertices in the "in" array.  This
// function is responsible for rasterization, interpolation of data to
// fragments, calling the fragment shader, and z-buffering.
void rasterize_triangle(driver_state& state, const data_geometry* in[3])
{

 float x[3], y[3];
    
    // calculates i and j coords in NDC for vertices
         for(int m = 0; m < 3; m++) {
           float i = ((state.image_width / 2.0f) * ((*in)[m].gl_Position[0]/(*in)[m].gl_Position[3]) + ((state.image_width / 2.0f) - 0.5f));
           float j = ((state.image_height / 2.0f) * ((*in)[m].gl_Position[1]/(*in)[m].gl_Position[3]) + ((state.image_height / 2.0f) - 0.5f));
           float k = ((state.image_width / 2.0f) * ((*in)[m].gl_Position[2]/(*in)[m].gl_Position[3]) + ((state.image_width / 2.0f) - 0.5f));
                 x[m] = i;
                 y[m] = j;
                 //z[m] = k;
          }
 float max_x = std::max(std::max(x[0], x[1]), x[2]);
float min_y = std::min(std::min(y[0], y[1]), y[2]);
 float min_x = std::min(std::min(x[0], x[1]), x[2]);
   float max_y = std::max(std::max(y[0], y[1]), y[2]); 

    if(( std::min(std::min(x[0], x[1]), x[2])) < 0)
        min_x = 0;
    if((std::min(std::min(y[0], y[1]), y[2])) < 0)
        min_y = 0;
    if((std::max(std::max(x[0], x[1]), x[2])) > state.image_width)
        max_x = state.image_width;
    if((std::max(std::max(y[0], y[1]), y[2])) > state.image_height)
        max_y = state.image_height;


 float ABC_area = (0.5f * ((x[1]*y[2] - x[2]*y[1]) - (x[0]*y[2] - x[2]*y[0]) - (x[0]*y[1] - x[1]*y[0])));




//static_cast<float>(i);
//static_cast<float>(j);
 for(int j = min_y; j < max_y; j++) {
 for(int i = min_x; i < max_x; i++) {
 float alpha = (0.5f * ((x[1] * y[2] - x[2] * y[1]) + (y[1] - y[2])*i + (x[2] - x[1])*j)) / ABC_area;
 float beta = (0.5f * ((x[2] * y[0] - x[0] * y[2]) + (y[2] - y[0])*i + (x[0] - x[2])*j)) / ABC_area;
 float gamma = (0.5f * ((x[0] * y[1] - x[1] * y[0]) + (y[0] - y[1])*i + (x[1] - x[0])*j)) / ABC_area;
float added = alpha + beta + gamma;
 if (alpha >= 0 && beta >= 0 && gamma >= 0 && (added <= 1.01)) {
 //state.image_color[i + j * state.image_width] = make_pixel  (static_cast<int>255,  static_cast<int>255,  static_cast<int>255);

                    state.image_color[i + j * state.image_width] = make_pixel(
                            static_cast<int>(255),
                            static_cast<int>(255),
                            static_cast<int>(255)); 

}
 }
 }

}
