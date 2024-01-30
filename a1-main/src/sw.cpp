#include "sw.hpp"

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "util/util.hpp"
 
namespace COL781 {
	namespace Software {

		ShaderProgram current_program;

		// Forward declarations

		SDL_Surface* framebuffer = NULL;
		SDL_Surface *windowSurface = NULL;
		std::vector<std::vector<std::vector<float>>> pointBuffer; 
		bool depth;
		int frameWidth;
		int frameHeight;
		int displayScale;
		int SPP; // Samples-per-pixel

		template <> float Attribs::get(int index) const;
		template <> glm::vec2 Attribs::get(int index) const;
		template <> glm::vec3 Attribs::get(int index) const;
		template <> glm::vec4 Attribs::get(int index) const;

		template <> void Attribs::set(int index, float value);
		template <> void Attribs::set(int index, glm::vec2 value);
		template <> void Attribs::set(int index, glm::vec3 value);
		template <> void Attribs::set(int index, glm::vec4 value);

		// Built-in shaders

		VertexShader Rasterizer::vsIdentity() {
			return [](const Uniforms &uniforms, const Attribs &in, Attribs &out) {
				glm::vec4 vertex = in.get<glm::vec4>(0);
				return vertex;
			};
		}

		VertexShader Rasterizer::vsTransform() {
			return [](const Uniforms &uniforms, const Attribs &in, Attribs &out) {
				glm::vec4 vertex = in.get<glm::vec4>(0);
				glm::mat4 transform = uniforms.get<glm::mat4>("transform");
				return transform * vertex;
			};
		}

		VertexShader Rasterizer::vsColor() {
			return [](const Uniforms &uniforms, const Attribs &in, Attribs &out) {
				glm::vec4 vertex = in.get<glm::vec4>(0);
				glm::vec4 color = in.get<glm::vec4>(1);
				out.set<glm::vec4>(0, color);
				return vertex;
			};
		}


		FragmentShader Rasterizer::fsConstant() {
			return [](const Uniforms &uniforms, const Attribs &in) {
				glm::vec4 color = uniforms.get<glm::vec4>("color");
				return color;
			};
		}

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
			// if (it != values.end()) {
			// 	delete it->second;
			// }
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
                    frameHeight = height;
                    frameWidth = width;
                    displayScale = 1;
					SPP = spp;
                    int screenWidth = frameWidth * displayScale;
                    int screenHeight = frameHeight * displayScale;
                    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
                    if (window == NULL) {
                        printf("Window could not be created! SDL_Error: %s", SDL_GetError());
                        success = false;
                    } else {
                        windowSurface = SDL_GetWindowSurface(window);
                        framebuffer = SDL_CreateRGBSurface(0, frameWidth, frameHeight, 32, 0, 0, 0, 0);
						pointBuffer = std::vector<std::vector<std::vector<float>>>(frameWidth, std::vector<std::vector<float>>(frameHeight, std::vector<float>(5, -1.0f)));
						// renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
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
			return program;
		}

		Object Rasterizer::createObject(){
			Object object;
			return object;
		}

		// struct Object {
		// 	using Buffer = std::vector<float>;
		// 	std::vector<Buffer> attributeValues;
		// 	std::vector<int> attributeDims;
		// 	std::vector<glm::ivec3> indices;
		// };


		void setAttribs(Object& object, int attribIndex, int n, int d, const float* data){
			std::cout << "Inside the setAttribs\n";
			
			if (object.attributeValues.empty()){
				for (int i = 0; i<n; i++){
					object.attributeValues.push_back({});
					object.attributeDims.push_back(0);
				}
			}

			for (int i = 0; i<n; i++){
				for (int j = 0; j<d; j++){
					// object.attributeValues[attribIndex+j].push_back(data[i*d + j]);
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
			// std::cout << "Inside the setVertexAttribs fxn\n";
			setAttribs(object, attribIndex, n, 4, (float*)data);
		}

		void Rasterizer::setTriangleIndices(Object &object, int n, glm::ivec3* indices){
			object.indices.assign(indices, indices + n);
		}

		void Rasterizer::clear(glm::vec4 color){
			// SDL_SetRenderDrawColor(renderer, 255*color[0], 255*color[1], 255*color[2], 255*color[3]);
			// SDL_RenderClear(renderer);
			// SDL_FreeSurface(framebuffer);
			// framebuffer = SDL_CreateRGBSurface(0, frameWidth, frameHeight, 32, 255*color[0], 255*color[1], 255*color[2], 255*color[3]);
			// SDL_FillRect(framebuffer, nullptr, SDL_MapRGBA(framebuffer->format, 
			// static_cast<Uint8> (255*color.r), 
			// static_cast<Uint8> (255*color.g),
			// static_cast<Uint8> (255*color.b),
			// static_cast<Uint8> (255*color.a)));
			for(int i = 0; i < frameWidth; i++){
				for(int j = 0; j < frameHeight; j++){
					pointBuffer[i][j] = {-1.0f, color.r, color.g, color.b, color.a};
				}
			}
		}

		void Rasterizer::useShaderProgram(const ShaderProgram &program){
			current_program = program;
		}

		void Rasterizer::deleteShaderProgram(ShaderProgram &program) {
			current_program.fs = NULL;
			current_program.vs = NULL;
		}

		// template <> void Rasterizer::setUniform(ShaderProgram &program, const std::string &name, glm::vec4 value){
		// 	current_program.uniforms.set(name, value);
		// }

		void Rasterizer::drawObject(const Object &object){
			// Assuming atributeValues and Dims are vertices in-order
			auto vertices = object.attributeValues;

			// for (auto& x : vertices){
			// 	for (auto& elem:x){
			// 		std::cout << elem << " ";
			// 	}
			// 	std::cout << std::endl;
			// }

			// helper::frameData* frame;
			// frame->framebuffer = framebuffer;
			// frame->frameWidth = frameWidth;
			// frame->frameHeight = frameHeight;

			std::vector<float> color = {0,0,1,0,1};

			// glm::vec4 screenMat = glm::vec4(glm::vec4(0.0f));


			if(!depth){
				// std::cout << "here\n";
				std::vector<glm::vec3> a = std::vector<glm::vec3>(3, glm::vec3(0.0f));
				for(auto index: object.indices){

					for(int i = 0; i < 3; i++){
						// std::cout << index[i] << std::endl;
						// std::cout << vertices[index[i]][0] << " " << vertices[index[i]][1] << "\n";
						// std::cout << frameWidth << " " << frameHeight << std::endl;
						// std::cout << (frameWidth/2)*vertices[index[i]][0] << std::endl;
						// std::cout << (frameHeight/2)*vertices[index[i]][1] << std::endl;
						a[i] = glm::vec3((frameWidth/2)*(vertices[index[i]][0] + 1), (frameHeight/2)*(vertices[index[i]][1] + 1), 0);
						// std::cout << "ok\n";
					}

					// std::cout << "created a\n";

					// for (auto& elem : a){
					// 	std::cout << elem.x << " " << elem.y << " " << elem.z << std::endl;
					// }

					Geometric::triangle T = Geometric::triangle( a[0], a[1], a[2]);

					// std::cout << "created triangle\n";
					
					// T.print();

					raster::anti_alias( T, SPP, color, pointBuffer);
				}
			}
			else{
				std::vector<glm::vec3> a = std::vector<glm::vec3>();
				for(auto index: object.indices){

					for(int i = 0; i < 3; i++){
						a[i] = glm::vec3(frameWidth/2*(vertices[index[i]][0]+1), frameHeight/2*(vertices[index[i]][1]+1), vertices[index[i]][2] != 0 ? 1/vertices[index[i]][2] :LONG_LONG_MAX);
					}

					Geometric::triangle T = Geometric::triangle( a[0], a[1], a[2]);

					raster::anti_alias( T, SPP, color, pointBuffer);
				}
			}
		}

		void Rasterizer::show(){
			Uint32 *pixels = (Uint32*)framebuffer->pixels;
			SDL_PixelFormat *format = framebuffer->format;
			for(int j = 0; j < frameHeight; j++){
				for(int i = 0; i < frameWidth; i++){
					pixels[i + frameWidth*j] = SDL_MapRGBA(format, 255*pointBuffer[i][frameHeight-1-j][1], 255*pointBuffer[i][frameHeight-1-j][2], 255*pointBuffer[i][frameHeight-1-j][3], 255*pointBuffer[i][frameHeight-1-j][4]); 
				}
			}
			SDL_BlitScaled(framebuffer, NULL, windowSurface, NULL);
            SDL_UpdateWindowSurface(window);
		}		

		// Print the contents of the Object
		void Rasterizer::printObject(const Object& object) {
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

		void test(){
			glm::vec3 a = glm::vec3(0,0, 0), b = glm::vec3(0,10,5), c = glm::vec3(7, 18, 9);
			Geometric::triangle T = Geometric::triangle(a,b,c);
			T.print();
			int spp = 1;
			std::vector<float> attrib = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
			std::vector<std::vector<std::vector<float>>> pointBuffer = std::vector<std::vector<std::vector<float>>>(50, std::vector<std::vector<float>>(50, std::vector<float>(5, 0.0f)));
			raster::anti_alias(T, spp, attrib, pointBuffer);
			for(int j = 0; j < 50; j++){
				for(int i = 0; i< 50 ;i++)
					std::cout<<pointBuffer[i][j][0]<<" ";
				std::cout<<"\n";
			}
			
		}

	}
}
