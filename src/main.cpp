#include "gl_app.hpp"
#include "gl_window.hpp"
#include "gl_texvaovbo.hpp"
#include "gl_model.hpp"
using namespace imt2531;
using namespace glm;

class game : public App {
public:
	Shader *ShaderID,*ShaderEarth;
	Model *ourModel,*ourModelEarth;
	Texture *textureProgramID;
	game()
	{
		init();
	}
	void init()
	{
	
		ShaderID = new Shader("../include/vertex.vert", "../include/fragment.frag");
		ShaderEarth = new Shader("../include/vertexearth.vert", "../include/fragmentearth.frag");
		ourModel= new Model (("../resources/nanosuit.obj"));
		ourModelEarth = new Model(("../resources3/Sphere.obj"));
		textureProgramID = new Texture("../resources3/EarthComposite.jpg");
		

	}
	void onDraw() {
		ShaderID->bind();
		static float time = 0.0;
		time += .01;
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 projection = glm::perspective(3.14f / 3.0f, (GLfloat)1024 / (GLfloat)768, 0.1f, -10.0f);
		GLint viewID = glGetUniformLocation(ShaderID->id(), "view");
		GLint projectionID = glGetUniformLocation(ShaderID->id(), "projection");
		GLint modelID = glGetUniformLocation(ShaderID->id(), "model");
		GLint normalMatrixID = glGetUniformLocation(ShaderID->id(), "normalMatrix");
		glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 modelm;
		modelm = glm::translate(modelm, glm::vec3(0.0f, -1.75f, -1.0f)); // translate it down so it's at the center of the scene
		modelm = glm::scale(modelm, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
																//glm::mat4 model = glm::rotate(glm::mat4(), time, glm::vec3(0, 1, 0));
		//model = glm::rotate(model, time, glm::vec3(0, 1, 0));														//ourShader.setMat4("model", model);
		glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelm));
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view*modelm)));
		glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		ourModel->Draw(*ShaderID);
		
		ShaderEarth->bind();
		textureProgramID->bind();
		glm::mat4 model;
		view = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		projection = glm::perspective(3.14f / 3.0f, (GLfloat)1024 / (GLfloat)768, 0.1f, -10.0f);
		viewID = glGetUniformLocation(ShaderEarth->id(), "view");
		projectionID = glGetUniformLocation(ShaderEarth->id(), "projection");
		modelID = glGetUniformLocation(ShaderEarth->id(), "model");
		normalMatrixID = glGetUniformLocation(ShaderEarth->id(), "normalMatrix");
		glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projection));
		// render the loaded model
		
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
																//glm::mat4 model = glm::rotate(glm::mat4(), time, glm::vec3(0, 1, 0));
		model = glm::rotate(model, time, glm::vec3(0, 1, 0));														//ourShader.setMat4("model", model);
		glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model));
		normalMatrix = glm::transpose(glm::inverse(glm::mat3(view*model)));
		glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		ourModelEarth->Draw(*ShaderEarth);
	}


};

int main()
{
	game Mygame;
	Mygame.start();

	return 0;
}
