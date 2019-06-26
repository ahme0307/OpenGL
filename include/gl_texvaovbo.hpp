#ifndef vaos
#define vaos


#include "gl_texture.hpp"
#include "gl_shader.hpp"

//#include "gl_window.hpp"
namespace imt2531 {


	class TexVAOVBO :public VAO, public Window {
	public:
		Shader *ShaderID;
		Texture *textureProgramID;

		GLuint id() const { return VertexArrayID;}
		Shader getVshader() const { return *ShaderID; }
		Texture getTexture() const { return *textureProgramID; }
		TexVAOVBO(GLenum primitive_mode, int numVertices, const GLfloat* vertices, const GLfloat* normal , const GLfloat* texture,  Shader *vertexProgram, Texture * textureProgram, GLenum fill_mode = GL_FILL)
		{
			//vertices, normal, color, texture,
			/*-----------------------------------------------------------------------------
			*  Generate Vertex and Bind it
			*-----------------------------------------------------------------------------*/
			this->ShaderID = vertexProgram;
			this->textureProgramID = textureProgram;
			this->NumVertices = numVertices;
			this->PrimitiveMode = primitive_mode;
			
			this->FillMode = fill_mode;
			//
			//
			// Create Vertex Array Object
			// Should be done after CreateWindow and before any other GL calls
			glGenVertexArrays(1, &(this->VertexArrayID)); // VAO
			glBindVertexArray(this->VertexArrayID); // Bind the VAO
					
			glGenBuffers(1, &(this->VertexBuffer)); // VBO - vertices
			glBindBuffer(GL_ARRAY_BUFFER, this->VertexBuffer); // Bind the VBO vertices
			glBufferData(GL_ARRAY_BUFFER, 6 * 6 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Copy the vertices into VBO
			GLint positionID = glGetAttribLocation(this->ShaderID->id(), "position");
			glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(positionID);

			glGenBuffers(1, &(this->NormalBuffer));  // VBO - Normal
			glBindBuffer(GL_ARRAY_BUFFER, this->NormalBuffer); // Bind the VBO vertices
			glBufferData(GL_ARRAY_BUFFER, 6 * 6 * 3 * sizeof(GLfloat), normal, GL_STATIC_DRAW); // Copy the vertices into VBO
			GLint normalID = glGetAttribLocation(this->ShaderID->id(), "normal");
			glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(normalID);   //<-- Enable normalID attribute

			

			glGenBuffers(1, &(this->TextureBuffer));  // VBO - textures
			glBindBuffer(GL_ARRAY_BUFFER, this->TextureBuffer); // Bind the VBO textures
			glBufferData(GL_ARRAY_BUFFER, 6 * 6 * 2 * sizeof(GLfloat), texture, GL_STATIC_DRAW);  // Copy the vertex into VBO
			GLint textureCoordinateID = glGetAttribLocation(this->ShaderID->id(), "textureCoordinate");
			glVertexAttribPointer(textureCoordinateID, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(textureCoordinateID);


		}

		void bind() { glBindVertexArray(this->VertexArrayID); }
		void unbind() { glBindVertexArray(0); }


		void update(const GLfloat* vertices_position) {

			bind();
			/*-----------------------------------------------------------------------------
			*  Load data onto GPU
			*-----------------------------------------------------------------------------*/
		
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_position), vertices_position);

			unbind();

		}
		void draw()
		{
			static float time = 0.0;
			time += .01;
			glUseProgram(this->textureProgramID->id());

			glBindTexture(GL_TEXTURE_2D, this->textureProgramID->id());
			glBindVertexArray(this->id());
			

			GLint modelID = glGetUniformLocation(this->ShaderID->id(), "model");
			GLint viewID = glGetUniformLocation(this->ShaderID->id(), "view");
			GLint projectionID = glGetUniformLocation(this->ShaderID->id(), "projection");
			GLint normalMatrixID = glGetUniformLocation(this->ShaderID->id(), "normalMatrix");


			glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			glm::mat4 proj = glm::perspective(3.14f / 3.0f, (GLfloat)1024 / (GLfloat)768, 0.1f, -10.0f);
			glm::mat4 model = glm::rotate(glm::mat4(), time, glm::vec3(0, 1, 0));
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view*model)));

			glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(proj));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normalMatrix));

			glDrawArrays(this->PrimitiveMode, 0, this->NumVertices);


		}


	};
	class Mesh {
	public:
		/*  Mesh Data  */
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<TextureA> textures;
		unsigned int VAO;
		int typeobj;

		/*  Functions  */
		// constructor
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureA> textures)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			// now that we have all the required data, set the vertex buffers and its attribute pointers.
			createVAOVBO();
		}

		// render the mesh
		void Draw(Shader shader)
		{
			
			// bind appropriate textures
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			for (unsigned int i = 0; i < textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
												  // retrieve texture number (the N in diffuse_textureN)
				std::string number;
				std::string name = textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++); // transfer unsigned int to stream
				else if (name == "texture_normal")
					number = std::to_string(normalNr++); // transfer unsigned int to stream
				else if (name == "texture_height")
					number = std::to_string(heightNr++); // transfer unsigned int to stream

														 // now set the sampler to the correct texture unit
				glUniform1i(glGetUniformLocation(shader.id(), (name + number).c_str()), i);
				// and finally bind the texture
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}

			// draw mesh
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// always good practice to set everything back to defaults once configured.
			glActiveTexture(GL_TEXTURE0);
		}

	private:
		/*  Render data  */
		unsigned int VBO, EBO;

		/*  Functions    */
		// initializes all the buffer objects/arrays
		void createVAOVBO()
		{
			// create buffers/arrays
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			// load data into vertex buffers
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// A great thing about structs is that their memory layout is sequential for all its items.
			// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
			// again translates to 3/2 floats which translates to a byte array.
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			// set the vertex attribute pointers
			// vertex Positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
			// vertex texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
			// vertex tangent
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
			// vertex bitangent
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

			glBindVertexArray(0);
		}
	};

}
#endif