varying vec4 position;
varying vec3 normal;
varying vec3 light_direction;

void main()
{
    vec4 ambient = vec4(1, 0, 0, 1);
    vec4 diffuse = vec4(0, 1, 0, 1);
    vec4 specular = vec4(0, 0, 1, 1);

    //vec3 r;
//for (int i =0; i< gl_MaxLights; ++i) {

 // for(gl_LightSource[i]) {

  ambient = gl_FrontMaterial.ambient * gl_LightModel.ambient *  gl_LightSource[0].ambient;

  for(int i = 0; i < gl_MaxLights; ++i) {  
    

vec3 r = 2.0 * (dot(light_direction, normal))* normal - light_direction;



    diffuse += gl_LightSource[i].diffuse*gl_FrontMaterial.diffuse
         * (max(0.0, (dot(normal, light_direction))));


    vec4 reflected = vec4(r[0], r[1], r[2], 0);

    specular += gl_LightSource[i].specular*gl_FrontMaterial.specular * (pow(max(0.0,
    dot(-position, reflected))), gl_FrontMaterial.shininess));
}
}

