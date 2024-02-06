#include "../src/a1.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace R = COL781::Software;
// namespace R = COL781::Hardware;
using namespace glm;

int main() {
	R::Rasterizer r;

    int width = 640, height = 480;
    if (!r.initialize("Example 7", width, height)){
        std::cout << "failure to initialise\n";
        return EXIT_FAILURE;
    }      

    R::ShaderProgram program = r.createShaderProgram(
        r.vsIdentity(),
        r.fsConstant()
    );



    // R::Object wall = r.createObject();

    // vec4 vertices_wall[] = {
	// 	vec4(-0.9, -0.9, 0.0, 1.0),
    //     vec4(-0.9, 0.9, 0.0, 1.0),
    //     vec4( 0.9, 0.9, 0.0, 1.0),
    //     vec4( 0.9, -0.9, 0.0, 1.0)
    // };
    // vec4 colors_wall[] = {
	// 	vec4(1.0, 0.0, 0.0, 1.0),
    //     vec4(1.0, 0.0, 0.0, 1.0),
	// 	vec4(1.0, 1.0, 0.0, 1.0),
    //     vec4(1.0, 1.0, 0.0, 1.0)
    // };
	// ivec3 triangles_wall[] = {
	// 	ivec3(0, 1, 3),
	// 	ivec3(1, 2, 3)
	// };

    // r.setVertexAttribs(wall, 0, 4, vertices_wall);
    // r.setVertexAttribs(wall,1,4,colors_wall);
	// r.setTriangleIndices(wall, 2, triangles_wall);

    // mat4 back_wall = translate(mat4(1.0f), vec3(0.0f, 0.0f, 2.0f));
    
 
 
    R::Object cube = r.createObject();
    vec4 vert_cube[] = {
        vec4(-0.4, -0.4, 15.0, 1.0),
        vec4(-0.4, 0.4, 15.0, 1.0),
        vec4(0.4, 0.4, 15.0, 1.0),
        vec4(0.4, -0.4, 15.0, 1.0),
        vec4(-0.4, -0.4, 16.0, 1.0),
        vec4(-0.4, 0.4, 16.0, 1.0),
        vec4(0.4, 0.4, 16.0, 1.0),
        vec4(0.4, -0.4, 16.0, 1.0)
    };

    vec4 color_cube[] = {
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0)
    };

    ivec3 tri_cube[]={
        ivec3(0,1,3),
        ivec3(1,2,3),
        ivec3(0,1,4),
        ivec3(1,4,5),
        ivec3(2,3,6),
        ivec3(2,6,7),
        ivec3(4,5,6),
        ivec3(5,6,7)
    };
    r.setVertexAttribs(cube,0,8,vert_cube);
    r.setVertexAttribs(cube, 1, 8, color_cube);
    r.setTriangleIndices(cube, 8, tri_cube);

    mat4 projection = perspective(radians(60.0f), (float)width/(float)height, 0.1f, 100.0f);

    while (!r.shouldQuit()) {
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.useShaderProgram(program);
        r.setUniform<vec4>(program, "color", vec4(0.0, 0.6, 0.0, 1.0));
        r.setUniform(program, "transform", projection);
		// r.drawObject(wall);
        r.drawObject(cube);
        r.show();
    }
    r.deleteShaderProgram(program);
    return EXIT_SUCCESS;
}
