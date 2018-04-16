#include <iostream>
#include <fstream>
#include <string>
#include <iterator>

#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include <GLFW/glfw3.h>


bool loadShader(GLuint, const char*);
void onError(int, const char*);

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
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// create shader objects
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	if(!loadShader(vertex_shader, "glsl/vertex.glsl")){ exit(EXIT_FAILURE); }

	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if(!loadShader(frag_shader, "glsl/fragment.glsl")){ exit(EXIT_FAILURE); }

    while(glfwWindowShouldClose(window) == GL_FALSE){
        glClear(GL_COLOR_BUFFER_BIT);

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
		GLchar log_msg[1024] = "No message";
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
