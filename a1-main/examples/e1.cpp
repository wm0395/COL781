#include "../src/a1.hpp"
#include <iostream>

// namespace R = COL781::Software;
namespace R = COL781::Hardware;
using namespace glm;

int main() {
    std::cout << "hello\n\n";
	R::Rasterizer r;
    if (!r.initialize("Example 1", 640, 480)){
        std::cout << "failure to initialise\n";
        return EXIT_FAILURE;
    }      

    R::ShaderProgram program = r.createShaderProgram(
        r.vsIdentity(),
        r.fsConstant()
    );

    std::cout << "ShaderProgram works well\n\n";

    vec4 vertices[] = {
		vec4(-0.8,  0.0, 0.0, 1.0),
        vec4(-0.4, -0.8, 0.0, 1.0),
        vec4( 0.8,  0.8, 0.0, 1.0),
        vec4(-0.4, -0.4, 0.0, 1.0)
    };
	ivec3 triangles[] = {
		ivec3(0, 1, 3),
		ivec3(1, 2, 3)
	};

    std::cout << "About to create the object\n\n";

	R::Object tickmark = r.createObject();

    std::cout << "Empty object creation succesful\n\n";
    
	r.setVertexAttribs(tickmark, 0, 4, vertices);
    std::cout << "Vertices assigned correctly\n\n";

	r.setTriangleIndices(tickmark, 2, triangles);

    std::cout << "About to print the object after assigning correctly\n\n";
    // printObject(tickmark);

    std::cout << "i have created my object\n\n";
    
    // std::cout << "TESTING\n";
    // R::test();
    // std::cout << "TEST DONE\n";

    while (!r.shouldQuit()) {
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        // std::cout << "cleared\n";
        r.useShaderProgram(program);
        // std::cout << "shaderProgram well\n";
        r.setUniform<vec4>(program, "color", vec4(0.0, 0.6, 0.0, 1.0));
        // std::cout << "Uniform set\n";
		r.drawObject(tickmark);
        r.show();
        // break;
    }
    // r.deleteShaderProgram(program);
    return EXIT_SUCCESS;
}
