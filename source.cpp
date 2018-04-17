#include <iostream>
#include <fstream>
#include <string>
#include <iterator>

#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include <GLFW/glfw3.h>


bool loadShader(GLuint, const char*);
void onError(int, const char*);
void logMessage(int);

int main(){
    // set callback
    glfwSetErrorCallback(onError);

    // initialization
    if(glfwInit() != GL_TRUE){
        std::cerr << "Failed to initialize opengl. " << std::endl;
        exit(EXIT_FAILURE);
    }

    // create window
    GLFWwindow* window 
        = glfwCreateWindow(
            640, // width
            480, // height
            "OpenGL Sample", // title
            NULL,
            NULL
        );
    if(window == NULL){
        std::cerr << "Failed to create a window. " << std::endl;
        exit(EXIT_FAILURE);
    }

    // create opengl context
    glfwMakeContextCurrent(window);

	// set background color (= default color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//=========================================
	// Prepare Shader Programs
	//=========================================

	// create shader objects
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	if(!loadShader(vertex_shader, "glsl/vertex.glsl")){ exit(EXIT_FAILURE); }

	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if(!loadShader(frag_shader, "glsl/fragment.glsl")){ exit(EXIT_FAILURE); }

	// create shader program
	GLuint shader_program = glCreateProgram();

	// bind shader objects
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, frag_shader);

	// bind attributes
	glBindAttribLocation(shader_program, 0, "position");
	glBindAttribLocation(shader_program, 1, "color");

	// link
	glLinkProgram(shader_program);
	GLint result = GL_FALSE;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
	if(result != GL_TRUE){
		GLsizei log_len = 0;
		GLchar log_msg[1024] = {};
		glGetProgramInfoLog(shader_program, 1024, &log_len, log_msg);

		std::cerr 
			<< "Failed to link shader program. \n"
			<< log_msg
			<< std::endl;
		exit(EXIT_FAILURE);
	}

	//=========================================
	// Prepare Buffers
	//=========================================
	
	float points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	// 
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	// allocate memory
	GLint buf_size = 9 * sizeof(float);
	//glBufferData(GL_ARRAY_BUFFER, buf_size, 0, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, buf_size, points, GL_STATIC_DRAW);
	GLint size_allocated = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size_allocated);
	if(size_allocated != buf_size){
		std::cerr << "Failed to allocate memory for buffer. " << std::endl;
		glDeleteBuffers(1, &vertex_buffer);
		exit(EXIT_FAILURE);
	}


	//=========================================
	// Main Loop
	//=========================================
    while(glfwWindowShouldClose(window) == GL_FALSE){
        // initialize
		glClear(GL_COLOR_BUFFER_BIT);

		// bind program
		glUseProgram(shader_program);

		// bind buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

		// draw
		glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	return 0;
}

bool loadShader(GLuint shader_id, const char* filename){
	// load shader file
	std::ifstream shader_file(filename);
	if(shader_file.fail()){
		std::cerr
			<< "Failed to load file: "
			<< filename
			<< std::endl;
		return false;
	}

	// interpret as chars
	std::string shader_code(
		(std::istreambuf_iterator<char>(shader_file)),
		std::istreambuf_iterator<char>());

	// bind shader code
	const char *source = shader_code.c_str();
	//std::cout << source << std::endl;
	glShaderSource(shader_id, 1, &source, NULL);

	// compile shader code
	glCompileShader(shader_id);
	GLint result = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	if(result != GL_TRUE){
		GLsizei log_len = 0;
		GLchar log_msg[1024] = {};
		glGetShaderInfoLog(shader_id, 1024, &log_len, log_msg);
		
		std::cerr 
			<< "Failed to compiler shader: "
			<< filename << "\n"
			<< log_msg
			<< std::endl;
		return false;
	}

	return true;
}

void onError(int err, const char* msg){
    std::cerr 
        << "GLFW Error  Code:" << err << "\n"
        << msg
        << std::endl;
    return;
}
