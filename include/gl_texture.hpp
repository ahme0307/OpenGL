
#include "gl_common.hpp"
namespace imt2531 {


  class Texture {
  public:
    GLuint TextureID;

    GLuint id() const { return TextureID; }
    

    Texture(const char* filename)
	{

      /*-----------------------------------------------------------------------------
       *  Generate Texture and Bind it
       *-----------------------------------------------------------------------------*/
      glGenTextures(1, &TextureID);
      glBindTexture(GL_TEXTURE_2D, TextureID); 

      /*-----------------------------------------------------------------------------
       *  Allocate Memory on the GPU
       *-----------------------------------------------------------------------------*/
       
        int twidth, theight, nrComponents;
		unsigned char* image = SOIL_load_image(filename, &twidth, &theight, &nrComponents, SOIL_LOAD_AUTO);
		if (image)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;
			// target | lod | internal_format | width | height | border | format | type | data
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, format, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D); // Generate MipMaps to use
			SOIL_free_image_data(image); // Free the data read from file after creating opengl texture 

		  /*-----------------------------------------------------------------------------
		   *  Set Texture Parameters
		   *-----------------------------------------------------------------------------*/
		   // Set these parameters to avoid a black screen caused by improperly mipmapped textures
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glActiveTexture(GL_TEXTURE0);
			/*-----------------------------------------------------------------------------
			 *  Unbind texture
			 *-----------------------------------------------------------------------------*/
			 // glBindTexture(GL_TEXTURE_2D, 0);
		}
		else {

			printf("SOIL cannot load image \n%s\n");

		}
    }

    void bind() { glBindTexture(GL_TEXTURE_2D, TextureID); }
    void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }


    void update(int width, int height, void * data){
       
       bind(); 
      /*-----------------------------------------------------------------------------
       *  Load data onto GPU
       *-----------------------------------------------------------------------------*/
      // target | lod | xoffset | yoffset | width | height | format | type | data
	  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, data );
      glGenerateMipmap(GL_TEXTURE_2D);

      unbind();

    }
    

  };

}
