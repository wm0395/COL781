#include "../src/a1.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

// namespace R = COL781::Software;
namespace R = COL781::Hardware;
using namespace glm;

int main() {
	R::Rasterizer r;

    int width = 640, height = 640;
    if (!r.initialize("Example 7", width, height)){
        std::cout << "failure to initialise\n";
        return EXIT_FAILURE;
    }      

    R::ShaderProgram program = r.createShaderProgram(
        r.vsColorTransform(),
        r.fsIdentity()
    );

    float half_edge_length = 0.4f;
    float depth = -1.0f;

    R::Object room = r.createObject();
    vec4 vert_room[] = {
        vec4(-half_edge_length, -half_edge_length, 0.0, 1.0), // room starts
        vec4(-half_edge_length, half_edge_length, 0.0, 1.0), 
        vec4(half_edge_length, half_edge_length, 0.0, 1.0),
        vec4(half_edge_length, -half_edge_length, 0.0, 1.0),
        vec4(-half_edge_length, -half_edge_length, depth, 1.0),
        vec4(-half_edge_length, half_edge_length, depth, 1.0),
        vec4(half_edge_length, half_edge_length, depth, 1.0),
        vec4(half_edge_length, -half_edge_length, depth, 1.0) // room end
    };

    vec4 color_room[] = {
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    };

    ivec3 tri_room[]={
        ivec3(0,1,4),
        ivec3(1,4,5),
        ivec3(0,3,4),
        ivec3(7,4,3),
        ivec3(7,3,6),
        ivec3(2,6,3),
        ivec3(1,2,6),
        ivec3(5,6,1)
    };

    r.setVertexAttribs(room, 0, 8, vert_room);
    r.setVertexAttribs(room, 1, 8, color_room);
    r.setTriangleIndices(room, 8, tri_room);

    R::Object back_wall = r.createObject();

    float back_wall_start = -1.0f;
    vec4 back_wall_vert[] = {
        vec4(-half_edge_length, -half_edge_length, back_wall_start, 1.0),
        vec4(-half_edge_length, half_edge_length, back_wall_start, 1.0),
        vec4(half_edge_length, half_edge_length, back_wall_start, 1.0),
        vec4(half_edge_length, -half_edge_length, back_wall_start, 1.0) 
    };
    vec4 back_wall_color[] = {
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    };
    ivec3 back_wall_tr[] = {
        ivec3(0,1,3),
        ivec3(1,2,3)
    };
    r.setVertexAttribs(back_wall, 0, 4, back_wall_vert);
    r.setVertexAttribs(back_wall, 1, 4, back_wall_color);
    r.setTriangleIndices(back_wall, 2, back_wall_tr);
    
 
    R::Object triangle = r.createObject();
    float triangle_length = 0.35f, triangle_thickness = 0.1f;
    vec4 triangles[] = {
        vec4(0, triangle_length, back_wall_start+0.05, 1.0),
        vec4(sqrt(3)/2 * triangle_length, -0.5*triangle_length, back_wall_start+0.05, 1.0),
        vec4(-sqrt(3)/2 * triangle_length, -0.5*triangle_length, back_wall_start+0.05, 1.0),
        vec4(0, (triangle_length-triangle_thickness), back_wall_start+0.05, 1.0),
        vec4(sqrt(3)/2 *(triangle_length - triangle_thickness), -0.5*(triangle_length-triangle_thickness), back_wall_start+0.05, 1.0),
        vec4(-sqrt(3)/2 * (triangle_length - triangle_thickness), -0.5 *(triangle_length-triangle_thickness), back_wall_start+0.05, 1.0)
    };vec4 triangle_color[] = {
        vec4(1.0, 1.0, 0.0, 1.0),
        vec4(1.0, 1.0, 0.0, 1.0),
        vec4(1.0, 1.0, 0.0, 1.0),
        vec4(1.0, 1.0, 0.0, 1.0),
        vec4(1.0, 1.0, 0.0, 1.0),
        vec4(1.0, 1.0, 0.0, 1.0)
    };
    ivec3 triangles_tr[] = {
        ivec3(0,3,4),
        ivec3(0,1,4),
        ivec3(1,2,4),
        ivec3(2,4,5),
        ivec3(2,5,0),
        ivec3(3,5,0)
    };
    r.setVertexAttribs(triangle, 0, 6, triangles);
    r.setVertexAttribs(triangle, 1, 6, triangle_color);
    r.setTriangleIndices(triangle, 6, triangles_tr);
    r.enableDepthTest();

    mat4 translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f));
    mat4 projection = perspective(radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
    mat4 matrix = projection * translation;

    mat4 model = mat4(1.0f);
    mat4 invert = mat4(1.0f);
    invert[1][1] = -1.0f;

    float speed1 = 10.0f;
    float speed2 = 0.5f;
    while (!r.shouldQuit()) {
        float time = SDL_GetTicks64()*1e-3;
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.useShaderProgram(program);
        model = rotate(mat4(1.0f), radians(speed1 * time), vec3(0.0f, 0.0f, 1.0f));
        r.setUniform(program, "transform", matrix);
		
        r.drawObject(room);
        r.drawObject(back_wall);

        r.setUniform(program, "transform", matrix * model);
        r.drawObject(triangle);

        r.setUniform(program, "transform", matrix * model * model * invert);
        r.drawObject(triangle);

        r.show();
    }
    r.deleteShaderProgram(program);
    return EXIT_SUCCESS;
}