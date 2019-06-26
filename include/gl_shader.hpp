

namespace imt2531 {

    class Shader {
        
    

	    public:
	    GLuint shaderProgram;
	    GLuint id() const { return shaderProgram; }

	    Shader(const char *path_vert_shader, const char *path_frag_shader){

		  /*-----------------------------------------------------------------------------
		   *  CREATE THE SHADER
		   *-----------------------------------------------------------------------------*/

		  GLuint vertexShader = load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
		  GLuint fragmentShader = load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);

		  // Create a program object and attach the two shaders we have compiled, the program object contains
		  // both vertex and fragment shaders as well as information about uniforms and attributes common to both.
		  shaderProgram = glCreateProgram();
		  glAttachShader(shaderProgram, vertexShader);
		  glAttachShader(shaderProgram, fragmentShader);

		  // Now that the fragment and vertex shader has been attached, we no longer need these two separate objects and should delete them.
		  // The attachment to the shader program will keep them alive, as long as we keep the shaderProgram.
		  glDeleteShader(vertexShader);
		  glDeleteShader(fragmentShader);

		  // Link the different shaders that are bound to this program, this creates a final shader that 
		  // we can use to render geometry with.
		  glLinkProgram(shaderProgram);
		  glUseProgram(shaderProgram);
	    }

	    void bind(){ glUseProgram(shaderProgram); }
	    void unbind() { glUseProgram(0); }


	    /*-----------------------------------------------------------------------------
	     *  FUNCION TO LOAD AND COMPILE SHADER FILES 
	     *-----------------------------------------------------------------------------*/
		GLuint load_and_compile_shader(const char *fname, GLenum shaderType) 
		{
		// Load a shader from an external file
			std::vector<char> buffer;
			read_shader_src(fname, buffer);
			const char *src = &buffer[0];

		// Create shaders
			GLuint shader = glCreateShader(shaderType);
		//attach the shader source code to the shader objec
			glShaderSource(shader, 1, &src, NULL);

		// Compile the shader
			glCompileShader(shader);
		// Comile the shader, translates into internal representation and checks for errors.
			GLint compileOK;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compileOK);
			if (!compileOK) {
				char infolog[1024];;
				glGetShaderInfoLog(shader, 1024, NULL, infolog);
				std::cout << "The program failed to compile with the error:" << std::endl << infolog << std::endl;
				glfwTerminate();
				getchar();
				exit(-1);
			}
		    return shader;
		}


		/*-----------------------------------------------------------------------------
		 *  FUNCION TO READ SHADER FILES AND CHECK FOR ERRORS
		 *-----------------------------------------------------------------------------*/
		void read_shader_src(const char *fname, std::vector<char> &buffer)
		{
			std::ifstream in;
			in.open(fname, std::ios::binary);
		
			if (!in.fail())
			{
				// Get the number of bytes stored in this file
				in.seekg(0, std::ios::end);
				size_t length = (size_t)in.tellg();

				// Go to start of the file
				in.seekg(0, std::ios::beg);

				// Read the content of the file in a buffer
				buffer.resize(length + 1);
				in.read(&buffer[0], length);
				in.close();
				// Add a valid C - string end
				buffer[length] = '\0';
			}
			else 
			{
			std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
			//std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
			exit(-1);
			}
	    }
	};
} 
