// Based on tutorial at http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-11-2d-text/

#ifndef TEXT2D_H
#define TEXT2D_H

void initText2D();
void printText2D(const char * text, int x, int y, int size);
void drawMenu(int x, int y, int size);
void cleanupText2D();

#endif