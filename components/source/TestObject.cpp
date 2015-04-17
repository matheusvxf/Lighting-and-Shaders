#include "TestObject.h"
#include "Window.h"
#include "main.h"

TestObject::TestObject()
{
    camera = Camera(Vector4(0, 0, 5), Vector4(0, 0, -1), Vector4(0, 1, 0));
}

TestObject::~TestObject()
{
}

void TestObject::render()
{
    Object::render();
    // Background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, 800, 600);
    glLoadIdentity();
    glFrustum(-.5f, .5f, -.5f*(600.0f / 800.0f), .5f*(600.0f / 800.0f), 1.0f, 500.0f);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glEnable(GL_TEXTURE_2D);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glTranslatef(0.0f, -10.0f, -50.0f);

    glBindTexture(GL_TEXTURE_2D, Globals::texture[0]);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);

    // Draw floor plane
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);

    glColor4f(1.0, 0.0, 0.1, 1.0);
    GLdouble PARTICLE_SIZE = 0.1;
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-PARTICLE_SIZE, PARTICLE_SIZE, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(PARTICLE_SIZE, PARTICLE_SIZE, 0.0f);
    glEnd();

}