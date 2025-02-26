#include "sw.hpp"

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "util/util.hpp"
 
namespace COL781 {
	namespace Software {

		// Forward declarations

		template <> float Attribs::get(int index) const;
		template <> glm::vec2 Attribs::get(int index) const;
		template <> glm::vec3 Attribs::get(int index) const;
		template <> glm::vec4 Attribs::get(int index) const;

		template <> void Attribs::set(int index, float value);
		template <> void Attribs::set(int index, glm::vec2 value);
		template <> void Attribs::set(int index, glm::vec3 value);
		template <> void Attribs::set(int index, glm::vec4 value);

		// Built-in shaders

		// A vertex shader that uses the 0th vertex attribute as the pospition.
		VertexShader Rasterizer::vsIdentity() {
			return [](const Uniforms &uniforms, const Attribs &in, Attribs &out) {
				glm::vec4 vertex = in.get<glm::vec4>(0);
				return vertex;
			};
		}

		// A vertex shader that applies the transformation matrix given by the uniform named 'transform'.
		VertexShader Rasterizer::vsTransform() {
			return [](const Uniforms &uniforms, const Attribs &in, Attribs &out) {
				glm::vec4 vertex = in.get<glm::vec4>(0);
				glm::mat4 transform = uniforms.get<glm::mat4>("transform");
				return transform * vertex;
			};
		}

		// A vertex shader that uses the 0th vertex attribute as the position and passes on the 1th attribute as the color.
		VertexShader Rasterizer::vsColor() {
			return [](const Uniforms &uniforms, const Attribs &in, Attribs &out) {
				glm::vec4 vertex = in.get<glm::vec4>(0);
				glm::vec4 color = in.get<glm::vec4>(1);
				out.set<glm::vec4>(0, color);
				return vertex;
			};
		}

		// A vertex shader that handles both transformation and color attributes.
		VertexShader Rasterizer::vsColorTransform(){
			return [](const Uniforms &uniforms, const Attribs &in, Attribs &out){
				glm::vec4 vertex = in.get<glm::vec4>(0);
				glm::vec4 color = in.get<glm::vec4>(1);
				glm::mat4 transform = uniforms.get<glm::mat4>("transform");
				out.set<glm::vec4>(0, color);
				return transform * vertex;
			};
		}

		// A fragment shader that returns a constant colour given by the uniform named 'color'.
		FragmentShader Rasterizer::fsConstant() {
			return [](const Uniforms &uniforms, const Attribs &in) {
				glm::vec4 color = uniforms.get<glm::vec4>("color");
				return color;
			};
		}

		// A fragment shader that uses the 0th attribute as the color.
		FragmentShader Rasterizer::fsIdentity() {
			return [](const Uniforms &uniforms, const Attribs &in) {
				glm::vec4 color = in.get<glm::vec4>(0);
				return color;
			};
		}

		// Implementation of Attribs and Uniforms classes

		void checkDimension(int index, int actual, int requested) {
			if (actual != requested) {
				std::cout << "Warning: attribute " << index << " has dimension " << actual << " but accessed as dimension " << requested << std::endl;
			}
		}

		void Attribs::print(){
			std::cout << "values.size() : " << values.size() << "\n";
			std::cout << "dims.size() => " << dims.size() << "\n";
			std::cout << "values vector => \n";
			for (auto& elem : values){
				std::cout << elem.x << " " << elem.y << " " << elem.z << " " << elem.w << "\n";
			}
			std::cout << "dims vector => \n";
			for (auto& val : dims) std::cout << val << std::endl;
			std::cout << "\n";
		}

		template <> float Attribs::get(int index) const {
			checkDimension(index, dims[index], 1);
			return values[index].x;
		}

		template <> glm::vec2 Attribs::get(int index) const {
			checkDimension(index, dims[index], 2);
			return glm::vec2(values[index].x, values[index].y);
		}

		template <> glm::vec3 Attribs::get(int index) const {
			checkDimension(index, dims[index], 3);
			return glm::vec3(values[index].x, values[index].y, values[index].z);
		}

		template <> glm::vec4 Attribs::get(int index) const {
			checkDimension(index, dims[index], 4);
			return values[index];
		}

		void expand(std::vector<int> &dims, std::vector<glm::vec4> &values, int index) {
			if (dims.size() < index+1)
				dims.resize(index+1);
			if (values.size() < index+1)
				values.resize(index+1);
		}

		template <> void Attribs::set(int index, float value) {
			expand(dims, values, index);
			dims[index] = 1;
			values[index].x = value;
		}

		template <> void Attribs::set(int index, glm::vec2 value) {
			expand(dims, values, index);
			dims[index] = 2;
			values[index].x = value.x;
			values[index].y = value.y;
		}

		template <> void Attribs::set(int index, glm::vec3 value) {
			expand(dims, values, index);
			dims[index] = 3;
			values[index].x = value.x;
			values[index].y = value.y;
			values[index].z = value.z;
		}

		template <> void Attribs::set(int index, glm::vec4 value) {
			expand(dims, values, index);
			dims[index] = 4;
			values[index] = value;
		}

		template <typename T> T Uniforms::get(const std::string &name) const {
			return *(T*)values.at(name);
		}

		template <typename T> void Uniforms::set(const std::string &name, T value) {
			auto it = values.find(name);
			if (it != values.end()) {
				delete it->second;
			}
			values[name] = (void*)(new T(value));
		}

		// Implementation of Windows class
		// Creates a window with the given title, size, and samples per pixel.
		
		
		bool Rasterizer::initialize(const std::string &title, int width, int height, int spp){
			bool success = true;
                if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                    printf("SDL could not initialize! SDL_Error: %s", SDL_GetError());
                    success = false;
                } else {
                    this->frameHeight = height;
                    this->frameWidth = width;
                    this->displayScale = 1;
					this->spp = spp;
                    int screenWidth = frameWidth * displayScale;
                    int screenHeight = frameHeight * displayScale;
                    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
                    if (window == NULL) {
                        printf("Window could not be created! SDL_Error: %s", SDL_GetError());
                        success = false;
                    } else {
                        this->windowSurface = SDL_GetWindowSurface(window);
                        this->framebuffer = SDL_CreateRGBSurface(0, this->frameWidth, this->frameHeight, 32, 0, 0, 0, 0);
						this->pointBuffer = std::vector<std::vector<Attribs>>(this->frameWidth, std::vector<Attribs>(this->frameHeight, Attribs()));
					}
                }
                return success;
		}

		// Returns true if the user has requested to quit the program.
		bool Rasterizer::shouldQuit(){
			SDL_Event e;
        	while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}
			return quit;
		}

		ShaderProgram Rasterizer::createShaderProgram(const VertexShader &vs, const FragmentShader &fs){
			ShaderProgram program;
			program.fs = fs;
			program.vs = vs;
			program.uniforms.set("transform", glm::mat4(1.0f));
			return program;
		}

		Object Rasterizer::createObject(){
			Object object;
			return object;
		}

		void setAttribs(Object& object, int attribIndex, int n, int d, const float* data){
			// std::cout << "Inside the setAttribs\n";
			
			if (object.attributeValues.empty()){
				for (int i = 0; i<n; i++){
					object.attributeValues.push_back({});
					object.attributeDims.push_back(0);
				}
			}

			for (int i = 0; i<n; i++){
				for (int j = 0; j<d; j++){
					object.attributeValues[i].push_back(data[i*d + j]);

				}
			}
			
			for (int j = 0; j<n; j++){
				object.attributeDims[j] = attribIndex+1;
			}
		}


		template <> void Rasterizer::setVertexAttribs(Object &object, int attribIndex, int n, const float* data) {
			setAttribs(object, attribIndex, n, 1, data);
		}

		template <> void Rasterizer::setVertexAttribs(Object &object, int attribIndex, int n, const glm::vec2* data) {
			setAttribs(object, attribIndex, n, 2, (float*)data);
		}

		template <> void Rasterizer::setVertexAttribs(Object &object, int attribIndex, int n, const glm::vec3* data) {
			setAttribs(object, attribIndex, n, 3, (float*)data);
		}

		template <> void Rasterizer::setVertexAttribs(Object &object, int attribIndex, int n, const glm::vec4* data) {
			setAttribs(object, attribIndex, n, 4, (float*)data);
		}

		void Rasterizer::setTriangleIndices(Object &object, int n, glm::ivec3* indices){
			object.indices.assign(indices, indices + n);
		}

		void Rasterizer::clear(glm::vec4 color){
			SDL_FillRect(framebuffer, nullptr, SDL_MapRGBA(framebuffer->format, 
			static_cast<Uint8> (255*color.r), 
			static_cast<Uint8> (255*color.g),
			static_cast<Uint8> (255*color.b),
			static_cast<Uint8> (255*color.a)));
			Attribs defaultAttrib = Attribs();
			defaultAttrib.set(0,(float)LONG_LONG_MAX);
			color.x = color.r;
			color.y = color.g;
			color.z = color.b;
			color.w = color.a;
			defaultAttrib.set(1, color);			
			for(int i = 0; i < frameWidth; i++){
				for(int j = 0; j < frameHeight; j++){
					pointBuffer[i][j] = defaultAttrib;
				}
			}
		}

		void Rasterizer::useShaderProgram(const ShaderProgram &program){
			this->program = program;
		}

		void Rasterizer::deleteShaderProgram(ShaderProgram &program) {
			this->program.fs = NULL;
			this->program.vs = NULL;
		}

		template <> void Rasterizer::setUniform(ShaderProgram &program, const std::string &name, glm::vec4 value){
			this->program.uniforms.set(name, value);
		}

		template <> void Rasterizer::setUniform(ShaderProgram &program, const std::string &name, glm::mat4 mvp){
			this->program.uniforms.set(name, mvp);
		}

		void Rasterizer::enableDepthTest(){
			depth = true;
		}

		void Rasterizer::drawObject(const Object &object){
			// Assuming atributeValues and Dims are vertices in-order
			auto vertices = object.attributeValues;
			Uniforms uniform = program.uniforms;
			VertexShader vsTf = vsTransform();
	
			for(auto index: object.indices){

				std::vector<Attribs> v_prop(3);

				for(int i = 0; i < 3; i++){
					Attribs pointData = Attribs();

					pointData.set(0, glm::vec4(vertices[index[i]][0],vertices[index[i]][1],vertices[index[i]][2],vertices[index[i]][3]));
					glm::vec4 pos = vsTf(uniform, pointData, pointData);
					pos.x /= pos.w;
					pos.y /= pos.w;
					pos.z /= pos.w;
					pos.w = 1/pos.w;
					pointData.set(0, glm::vec4(((frameWidth/2)*(pos.x + 1)), ((frameHeight/2)*(pos.y+1)), pos.z, pos.w));
					if(vertices[index[i]].size() == 8){
						glm::vec4 color = glm::vec4(vertices[index[i]][4], vertices[index[i]][5], vertices[index[i]][6], vertices[index[i]][7]);
						pointData.set(1, color);
					}
					else
						pointData.set(1, program.uniforms.get<glm::vec4>("color")); // Color-information
					v_prop[i] = pointData;
				}

				Geometric::triangle T = Geometric::triangle(v_prop);
				raster::anti_alias(T, spp, pointBuffer);
			}
		}

		void Rasterizer::show(){
			Uint32 *pixels = (Uint32*)framebuffer->pixels;
			SDL_PixelFormat *format = framebuffer->format;
			for(int j = 0; j < frameHeight; j++){
				for(int i = 0; i < frameWidth; i++){
					glm::vec4 color = pointBuffer[i][frameHeight-1-j].get<glm::vec4>(1);
					pixels[i + frameWidth*j] = SDL_MapRGBA(format, 255*color.x, 255*color.y, 255*color.z, 255*color.w); 
				}
			}
			SDL_BlitScaled(framebuffer, NULL, windowSurface, NULL);
            SDL_UpdateWindowSurface(window);
		}		

		// Print the contents of the Object
		void printObject(const Object& object) {
			// Print attributeValues
			std::cout << "attributeValues:" << std::endl;
			for (const auto& buffer : object.attributeValues) {
				for (float value : buffer) {
					std::cout << value << " ";
				}
				std::cout << std::endl;
			}

			std::cout << std::endl;

			// Print attributeDims
			std::cout << "attributeDims:" << std::endl;
			for (int dim : object.attributeDims) {
				std::cout << dim << " ";
			}
			std::cout << std::endl;
			std::cout << std::endl;

			// Print indices
			std::cout << "indices:" << std::endl;
			for (const auto& idx : object.indices) {
				std::cout << "(" << idx.x << ", " << idx.y << ", " << idx.z << ") ";
			}
			std::cout << std::endl;
			std::cout << std::endl;
		}
	}
}
