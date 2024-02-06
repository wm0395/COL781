#include "../src/a1.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace R = COL781::Software;
// namespace R = COL781::Hardware;
using namespace glm;
int main() {
	R::Rasterizer r;
	int width = 640, height = 640;
    if (!r.initialize("Example 6", width, height))
        return EXIT_FAILURE;

    R::ShaderProgram program = r.createShaderProgram(
        r.vsColorTransform(),
        r.fsIdentity()
    );
    
    vec4 vertices[] = {
        vec4( -0.8,  -0.8, 0.0, 1.0),
        vec4(  0.8,  -0.8, 0.0, 1.0),
		vec4( -0.8,   0.8, 0.0, 1.0),
        vec4(  0.8,   0.8, 0.0, 1.0)
    };
    
	ivec3 triangles[] = {
		ivec3(0, 1, 2),
		ivec3(1, 2, 3)
	};

	R::Object shape = r.createObject();
	r.setVertexAttribs(shape, 0, 4, vertices);
	r.setTriangleIndices(shape, 2, triangles);

    // The transformation matrix.
    mat4 model = mat4(1.0f);

    float hour = 0.1;
	mat4 trans_hour = translate(mat4(1.0f), vec3(0.8f, 0.8f, 0.0f));
    mat4 scale_hour = scale(mat4(1.0f), vec3(hour/1.6f, 0.2/1.6f, 0.0f));
    mat4 trans_hour2 = translate(mat4(1.0f), vec3(-hour/2.0f, 0.0f, 0.0f));
    mat4 hour_matrix = trans_hour2 * scale_hour * trans_hour;

    float min = 0.075;
	mat4 trans_min = translate(mat4(1.0f), vec3(0.8f, 0.8f, 0.0f));
    mat4 scale_min = scale(mat4(1.0f), vec3(min/1.6f, 0.5/1.6f, 0.0f));
    mat4 trans_min2 = translate(mat4(1.0f), vec3(-min/2.0f, 0.0f, 0.0f));
    mat4 min_matrix = trans_min2 * scale_min * trans_min;

    float sec = 0.05;
	mat4 trans_sec = translate(mat4(1.0f), vec3(0.8f, 0.8f, 0.0f));
    mat4 scale_sec = scale(mat4(1.0f), vec3(sec/1.6f, 0.6/1.6f, 0.0f));
    mat4 trans_sec2 = translate(mat4(1.0f), vec3(-sec/2.0f, 0.0f, 0.0f));
    mat4 sec_matrix = trans_sec2 * scale_sec * trans_sec;

    float ticks_large = 0.07;
	mat4 trans_ticks_large = translate(mat4(1.0f), vec3(0.8f, 0.8f, 0.0f));
    mat4 scale_ticks_large = scale(mat4(1.0f), vec3(ticks_large/1.6f, 0.1/1.6f, 0.0f));
    mat4 trans_ticks_large2 = translate(mat4(1.0f), vec3(-ticks_large/2.0f, 0.8f, 0.0f));
    mat4 ticks_large_mat = trans_ticks_large2 * scale_ticks_large * trans_ticks_large;
    std::vector<mat4> rot_larger_ticks(12);
    for (int i = 0; i<12; i++){
        rot_larger_ticks[i] = rotate(mat4(1.0f), radians(30.0f * i), vec3(0.0f, 0.0f, 1.0f));
    }

    float ticks_small = 0.03;
	mat4 trans_ticks_small = translate(mat4(1.0f), vec3(0.8f, 0.8f, 0.0f));
    mat4 scale_ticks_small = scale(mat4(1.0f), vec3(ticks_small/1.6f, 0.06/1.6f, 0.0f));
    mat4 trans_ticks_small2 = translate(mat4(1.0f), vec3(-ticks_small/2.0f, 0.8f, 0.0f));
    mat4 ticks_small_mat = trans_ticks_small2 * scale_ticks_small * trans_ticks_small;
    std::vector<mat4> rot_small_ticks(60);
    for (int i = 0; i<60; i++){
        rot_small_ticks[i] = rotate(mat4(1.0f), radians(6.0f * i), vec3(0.0f, 0.0f, 1.0f));
    }
    
    while (!r.shouldQuit()) {

        // Get current time
        time_t currentTime = time(nullptr);
        struct tm* localTime = localtime(&currentTime);

        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.useShaderProgram(program);
        r.setUniform<vec4>(program, "color", vec4(0.0,0.0,0.0,1.0));

        // Calculate angle for the hour hand
        float hourAngle = (localTime->tm_hour % 12 + localTime->tm_min / 60.0f) * 30.0f;
        model = rotate(mat4(1.0f), radians(-hourAngle), vec3(0.0f,0.0f,1.0f)); 
        r.setUniform(program, "transform", model * hour_matrix);
		r.drawObject(shape);

        // Calculate angle for the minutes hand
        float minuteAngle = (localTime->tm_min % 60 + localTime->tm_sec / 60.0f) * 6.0f;
        model = rotate(mat4(1.0f), radians(-minuteAngle), vec3(0.0f,0.0f,1.0f));
        r.setUniform(program, "transform", model * min_matrix);
		r.drawObject(shape);

        // Calculate angle for the seconds hand
        float secondsAngle = (localTime->tm_sec % 60) * 6.0f;
        model = rotate(mat4(1.0f), radians(-secondsAngle), vec3(0.0f,0.0f,1.0f));
        r.setUniform(program, "transform", model * sec_matrix);
		r.drawObject(shape);

        for (int i = 0; i<12; i++){
            r.setUniform(program, "transform", rot_larger_ticks[i] * ticks_large_mat);
		    r.drawObject(shape);
        }

        for (int i = 0; i<60; i++){
            r.setUniform(program, "transform", rot_small_ticks[i] * ticks_small_mat);
		    r.drawObject(shape);
        }
        r.show();
    }
    r.deleteShaderProgram(program);
    return EXIT_SUCCESS;
}