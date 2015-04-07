#include "GlutHFunc.h"

    void checkGLError (const char *errorLocation)
{
    assert (errorLocation);

    unsigned int gle = glGetError();

    if (gle != GL_NO_ERROR)
    {
        std::cout << "GL Error discovered from caller " << errorLocation << ": ";

        switch (gle)
        {
        case GL_INVALID_ENUM:
            std::cout << "Invalid enum." << std::endl;
            break;

        case GL_INVALID_VALUE:
            std::cout << "Invalid value.\n";
            break;

        case GL_INVALID_OPERATION:
            std::cout << "Invalid Operation.\n";
            break;

        case GL_STACK_OVERFLOW:
            std::cout << "Stack overflow.\n";
            break;

        case GL_STACK_UNDERFLOW:
            std::cout << "Stack underflow.\n";
            break;

        case GL_OUT_OF_MEMORY:
            std::cout << "Out of memory.\n";
            break;
        default:
            std::cout << "Unknown error! Enum code is: " << gle << std::endl;
            break;
        }

    }
}

// Routine to read a bitmap file.
// Works only for uncompressed bmp files of 24-bit color.
BitMapFile *getBMPData(const char *filename)
{
  std::cout << "getBMPData : "<<filename<<std::endl;

   BitMapFile *bmp = new BitMapFile;
   unsigned int size, offset, headerSize;

   // Read input file name.
   std::ifstream infile(filename, std::ios::binary);

   // Get the starting point of the image data.
   infile.seekg(10);
   infile.read((char *) &offset, 4);

   // Get the header size of the bitmap.
   infile.read((char *) &headerSize,4);

   // Get width and height values in the bitmap header.
   infile.seekg(18);
   infile.read( (char *) &bmp->sizeX, 4);
   infile.read( (char *) &bmp->sizeY, 4);

   // Allocate buffer for the image.
   size = bmp->sizeX * bmp->sizeY * 24;
   bmp->data = new unsigned char[size];

   // Read bitmap data.
   infile.seekg(offset);
   infile.read((char *) bmp->data , size);

   // Reverse color from bgr to rgb.
   int temp;
   for (unsigned int i = 0; i < size; i += 3)
   {
      temp = bmp->data[i];
	  bmp->data[i] = bmp->data[i+2];
	  bmp->data[i+2] = temp;
   }

   return bmp;
}

GLuint LoadTexture(const char *filename)
{
    /*
    assert (filename);
    std::cout << "LoadTexture : "<<filename<<std::endl;

    GLuint ret_tex = 0;

    BitMapFile *image;
    image = getBMPData(filename);
    std::cout << "getBMPData with "<<image<<std::endl;


    glBindTexture(GL_TEXTURE_2D, ret_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->sizeX, image->sizeY, 0,
	            GL_RGB, GL_UNSIGNED_BYTE, image->data);

    delete image;
*/
    return ret_tex;
}

void renderBitmapString(float x, float y, float z, void *font, char *string)
{
	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}


void renderStrokeFontString(
	float x,
	float y,
	float z,
	void *font,
	char *string) {

	char *c;
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(0.001f, 0.001f, 0.001f);
	for (c = string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}
	glPopMatrix();
}
