
#ifndef windows_glfw
#define windows_glfw

#include "gl_common.hpp"


namespace imt2531 {

	struct Interface {

		static void * App; // <-- an unknown application to be defined later
/*

template<class APPLICATION>
static void OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods) {
((APPLICATION*)(App))->onKeyDown(key, action);
}

template<class APPLICATION>
static void OnMouseMove(GLFWwindow* window, double x, double y) {
((APPLICATION*)(App))->onMouseMove(x, y);
}

template<class APPLICATION>
static void OnMouseDown(GLFWwindow* window, int button, int action, int mods) {
((APPLICATION*)(App))->onMouseDown(button, action);
}

*/

	};

	void * Interface::App;
class  Window {
	public:
    GLFWwindow * window;
	Interface interface;
    int mWidth, mHeight;

    int width()  { return mWidth; }
    int height() { return mHeight; }
    float ratio() { return (float)mWidth/mHeight;}

    Window() {}

    //Create a Window Context
	template<class APPLICATION>
    void create(APPLICATION * app, int width, int height, const char * name = "demo")
	{

		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			getchar();
			exit(-1);
		}
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		 window = glfwCreateWindow(width, height, "OpenGL07", NULL, NULL);



		if (window == NULL) {
			fprintf(stderr, "Failed to open GLFW window.\n");
			getchar();
			glfwTerminate();
			exit(-1);
		}

		glfwMakeContextCurrent(window);


		glfwSwapInterval(1);
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		//glfwSetFramebufferSizeCallback(window, reshapeWindow);
		//glfwSetWindowSizeCallback(window, reshapeWindow);

		/* Register function to handle window close */
		//glfwSetWindowCloseCallback(window, quit);

		/* Register function to handle keyboard input */
		//glfwSetKeyCallback(window, keyboard);      // general keyboard input
		//glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

													/* Register function to handle mouse click */
		//glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks
		//glfwSetCursorPosCallback(window, mousePos);
		//glfwSetCursorEnterCallback(window, enterCallback);
		//	glfwSetScrollCallback(window, scrollCallback);
//		glfwSetKeyCallback(window, Interface::OnKeyDown<APPLICATION>);
//		glfwSetCursorPosCallback(window, Interface::OnMouseMove<APPLICATION>);
//		glfwSetMouseButtonCallback(window, Interface::OnMouseDown<APPLICATION>);
		//glfwSetFramebufferSizeCallback(window, reshapeWindow);

	}

    //Get the Current framebuffer Size in pixels and Set the Viewport to it    
    void setViewport(){ 
        glfwGetFramebufferSize(window, &mWidth, &mHeight); 
        glViewport(0,0,mWidth,mHeight);  
    }

    //Check whether window should close
    bool shouldClose(){
      return glfwWindowShouldClose(window);
    }

    //Swap front and back buffers
    void swapBuffers(){
      glfwSwapBuffers(window);
    }

    //Destroy the window
    void destroy(){
      glfwDestroyWindow(window);
    }

    ~Window(){
      destroy();
    }
};

} 
#endif