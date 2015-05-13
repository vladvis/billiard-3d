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

 GLuint raw_texture_load(const char *filename, int width, int height)
 {
     GLuint texture;
     unsigned char *data;
     FILE *file;

     // open texture data
     file = fopen(filename, "rb");
     if (file == NULL) return 0;

     // allocate buffer
     data = (unsigned char*) malloc(width * height * 4);

     // read texture data
     int res = fread(data, width * height * 4, 1, file);
     if (res < 0) assert ("Bad input file!");
     fclose(file);

     // allocate a texture name
     glGenTextures(1, &texture);

     // select our current texture
     glBindTexture(GL_TEXTURE_2D, texture);

     // select modulate to mix texture with color for shading
     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);

     // when texture area is small, bilinear filter the closest mipmap
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
     // when texture area is large, bilinear filter the first mipmap
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     // texture should tile
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

     // build our texture mipmaps
     gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

     // free buffer
     free(data);

     return texture;
 }

void renderString (void *font, char *string)
{
	char *c;
	glScalef(0.17f, 0.15f, 0.0f);
	for (c = string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}
}

void renderStrokeFontString (float x, float y, float z, void *font, char *string)
{
	char *c;
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(0.001f, 0.001f, 0.001f);
	for (c = string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}
	glPopMatrix();
}

Edit::Edit(float x, float y, float w, float h, std::string text) : Widget(x, y, w, h) {
    this->text = text;
    this->cursor_pos = text.size();
    this->setBackgroundColor(0.1f, 0.1f, 0.1f, 0.5f);
}

void Edit::receiveStroke(char c) {
    if (c == 8) {  // backspace
        if (this->cursor_pos == 0)
            return;
        std::string newtext = this->text.substr(0, this->cursor_pos-1) +
                              this->text.substr(this->cursor_pos, this->text.size()-this->cursor_pos);
        this->text = newtext;
        this->cursor_pos--;
    }
    if (this->text.size() >= EDITLENGTH-1)
        return;
    if (!is_char(c))
        return;
    std::string newtext = this->text.substr(0, this->cursor_pos) + c +
                          this->text.substr(this->cursor_pos, this->text.size()-this->cursor_pos);

    this->text = newtext;
    this->cursor_pos++;
}

bool is_char(char c) {
    return c >= 32 && c < 127;
}

Widget::Widget(float x, float y, float w, float h, bool visible) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->visible = visible;
    this->isFocused = false;
}

void Widget::receiveStroke(char c) {
}

void Widget::render() {
}

void Edit::render() {
    if (!this->visible)
        return;
    glPushMatrix();
        DrawRoundRect(this->x, this->y + this->h/2, this->w, this->h, this->bgColor, 0.0f);
    glPopMatrix();
    glPushMatrix();
        glColor3f(0.8f, 0.8f, 0.7f);
        glTranslatef(this->x+5.0f, this->y-5.0f, 0.0f);
        renderString(GLUT_STROKE_MONO_ROMAN, (char *)this->text.c_str());
    glPopMatrix();
    glPushMatrix();
        if (this->isFocused) {
            float xpos = 0.17f * 104.76f * (this->cursor_pos - 0.2f);
            glTranslatef(this->x + 5.0f + xpos, this->y - 5.0f, 0.0f);
            renderString(GLUT_STROKE_ROMAN, (char *) "|");
        }
    glPopMatrix();
}

typedef struct
{
    float x;
    float y;
} Vector2f;

#define ROUNDING_POINT_COUNT 8
void DrawRoundRect( float x, float y, float width, float height, float* color, float radius )
{
    Vector2f top_left[ROUNDING_POINT_COUNT];
    Vector2f bottom_left[ROUNDING_POINT_COUNT];
    Vector2f top_right[ROUNDING_POINT_COUNT];
    Vector2f bottom_right[ROUNDING_POINT_COUNT];

    if( radius == 0.0 )
    {
        radius = min(width, height);
        radius *= 0.10; // 10%
    }

    int i = 0;
    float x_offset, y_offset;
    float step = ( 2.0f * PI ) / (ROUNDING_POINT_COUNT * 4),
            angle = 0.0f;

    unsigned int index = 0, segment_count = ROUNDING_POINT_COUNT;
    Vector2f bottom_left_corner = { x + radius, y - height + radius };


    while( i != segment_count )
    {
        x_offset = cosf( angle );
        y_offset = sinf( angle );


        top_left[ index ].x = bottom_left_corner.x -
                              ( x_offset * radius );
        top_left[ index ].y = ( height - ( radius * 2.0f ) ) +
                              bottom_left_corner.y -
                              ( y_offset * radius );


        top_right[ index ].x = ( width - ( radius * 2.0f ) ) +
                               bottom_left_corner.x +
                               ( x_offset * radius );
        top_right[ index ].y = ( height - ( radius * 2.0f ) ) +
                               bottom_left_corner.y -
                               ( y_offset * radius );


        bottom_right[ index ].x = ( width - ( radius * 2.0f ) ) +
                                  bottom_left_corner.x +
                                  ( x_offset * radius );
        bottom_right[ index ].y = bottom_left_corner.y +
                                  ( y_offset * radius );


        bottom_left[ index ].x = bottom_left_corner.x -
                                 ( x_offset * radius );
        bottom_left[ index ].y = bottom_left_corner.y +
                                 ( y_offset * radius );


        top_left[ index ].x = top_left[ index ].x;
        top_left[ index ].y = top_left[ index ].y;


        top_right[ index ].x = top_right[ index ].x;
        top_right[ index ].y = top_right[ index ].y;


        bottom_right[ index ].x = bottom_right[ index ].x ;
        bottom_right[ index ].y = bottom_right[ index ].y;


        bottom_left[ index ].x =  bottom_left[ index ].x ;
        bottom_left[ index ].y =  bottom_left[ index ].y ;

        angle -= step;

        ++index;

        ++i;
    }

    static GLubyte clr[] = { 0, 0, 0, 128 };

    if( color )
        glColor4fv(color);
    else
        glColor4ubv(clr);

    glBegin( GL_TRIANGLE_STRIP );
    {
        // Top
        for( i = segment_count - 1 ; i >= 0 ; i--)
        {
            glVertex2f( top_left[ i ].x, top_left[ i ].y );
            glVertex2f( top_right[ i ].x, top_right[ i ].y );
        }

        // In order to stop and restart the strip.
        glVertex2f( top_right[ 0 ].x, top_right[ 0 ].y );
        glVertex2f( top_right[ 0 ].x, top_right[ 0 ].y );

        // Center
        glVertex2f( top_right[ 0 ].x, top_right[ 0 ].y );
        glVertex2f( top_left[ 0 ].x, top_left[ 0 ].y );
        glVertex2f( bottom_right[ 0 ].x, bottom_right[ 0 ].y );
        glVertex2f( bottom_left[ 0 ].x, bottom_left[ 0 ].y );

        // Bottom
        for( i = 0; i != segment_count ; i++ )
        {
            glVertex2f( bottom_right[ i ].x, bottom_right[ i ].y );
            glVertex2f( bottom_left[ i ].x, bottom_left[ i ].y );
        }
    }
    glEnd();
}

void EditFloat::receiveStroke(char c) {
    if (c == 8) {  // backspace
        if (this->cursor_pos == 0)
            return;
        std::string newtext = this->text.substr(0, this->cursor_pos-1) +
                              this->text.substr(this->cursor_pos, this->text.size()-this->cursor_pos);
        this->text = newtext;
        if (!std::regex_match(newtext, std::regex("^[0-9]+(.[0-9]*)?$")))
            this->setBackgroundColor(0.9f, 0.1f, 0.1f, 0.5f);
        else
            this->setBackgroundColor(0.1f, 0.1f, 0.1f, 0.5f);
        this->cursor_pos--;
    }
    if (this->text.size() >= EDITLENGTH-1)
        return;
    if (!is_char(c))
        return;
    std::string newtext = this->text.substr(0, this->cursor_pos) + c +
                          this->text.substr(this->cursor_pos, this->text.size()-this->cursor_pos);
    if (!std::regex_match(newtext, std::regex("^[0-9]+(\\.[0-9]*)?$")))
        return;
    else
        this->setBackgroundColor(0.1f, 0.1f, 0.1f, 0.5f);
    this->text = newtext;
    this->cursor_pos++;
}

void Edit::setBackgroundColor(float r, float g, float b, float transparent) {
    this->bgColor[0] = r;
    this->bgColor[1] = g;
    this->bgColor[2] = b;
    this->bgColor[3] = transparent;
}
