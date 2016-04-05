#include<GL\freeglut.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include<time.h>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <vector>
#include "imageloader.h"
using namespace std;

double camX = 3, camY = 1.5, camZ = 8, camAng = 0.0;    //Camera position and direction                    //Working variables
int cameraCount = 0; // count thet camera angles is it when mouse is click 
bool keystates[256]; //this array contain all the keyboard keys that are pressed i.e if it is true then it is pressed else not pressed
int roadLength = 300;
bool gameOver = false;
bool gamePause = true;
bool gameRestart = false;
bool startText = true;
double  axis = 0;
double  axis2 = 0;
double speedArray[] = { 0.6, 1.1};
double speed = speedArray[0]; 
// in this game -3 is important number 
GLuint HouseTexture;
GLuint TreesTexture;
GLuint IceTexture;
GLuint StartTexture;
GLuint SnowTexture;
GLuint FinishTexture;
GLuint SnowmanTexture;
GLuint HappyTexture;
GLUquadric *quad;


double mZ = 0.0;
double mX = 0.0;

struct Enamy
{
	vector<int> rowPosition; // is between 0 and 3 [0,3]
};

vector <Enamy> vectorOfEnamy;
void generatedEnamyVector();

GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
		//as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void LoadImages(){

	Image* image1 = loadBMP("trees.bmp");
	TreesTexture = loadTexture(image1);
	delete image1;

	Image* image2 = loadBMP("ice.bmp");
	IceTexture = loadTexture(image2);
	delete image2;


	Image* image3 = loadBMP("snow.bmp");
	SnowTexture = loadTexture(image3);
	delete image3;

	Image* image4 = loadBMP("snowman.bmp");
	SnowmanTexture = loadTexture(image4);
	delete image4;

	Image* image5 = loadBMP("happy.bmp");
	HappyTexture = loadTexture(image5);
	delete image5;

	quad = gluNewQuadric(); // need for spheres texture

}

void drawTextOnScreen(float x, float y, float z, char* text) // write text on the screen 
{
	char *text2;
	glColor3f(0.2, 0, 0.9);
	glRasterPos3f(x, y, z);
	for (text2 = text; *text2 != '\0'; text2++)// can't use glutBitmapString it does not work
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text2);
	}

}

//This Function is called when the keyboard key is release after being pressed and this function sets the key released to false
void KeyUpPress(unsigned char key, int x, int y){
	switch (key){
	case 'a':
	case 'A':
		keystates['a'] = false;
		keystates['A'] = false;
		break;
	case 'd':
	case 'D':
		keystates['d'] = false;
		keystates['D'] = false;
		break;
	case 'w':
	case 'W':
		keystates['w'] = false;
		keystates['W'] = false;
		break;
	case 's':
	case 'S':
		keystates['s'] = false;
		keystates['S'] = false;
		break;
	case 32://space
		keystates[32] = false;
		break;
	case  13: //enter
		keystates[13] = false;
		break;
	case 27:
		exit(0);

	}
}


//This Function is called when the keyboard key is pressed and this function sets the key pressed to true
void KeyDownPress(unsigned char key, int x, int y){
	switch (key){
	case 'a':
	case 'A':
		keystates['a'] = true;
		keystates['A'] = true;
		break;
	case 'd':
	case 'D':
		keystates['d'] = true;
		keystates['D'] = true;
		break;
	case 'w':
	case 'W':
		keystates['w'] = true;
		keystates['W'] = true;
		break;
	case 's':
	case 'S':
		keystates['s'] = true;
		keystates['S'] = true;
		break;
	case 32://space
		keystates[32] = true;
		break;
	case  13: //enter
		keystates[13] = true;
		break;
	case 27:
		exit(0);

	}
}

//This function is called when the mouse wheel is turn (change camare Y coordinate (camY)  
void mouseWheel(int btn, int dir, int x, int y)
{
	int a = (camY * 10);
	if (dir > 0){
		camY += 0.2;
	}
	else{
		camY -= 0.2;
	}
}

void mouseComera(int btn, int state, int x, int y){ // camara angles settings then mouse click ( at all is 3 angles)
	if (gameOver == false){
		if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
			cameraCount++;
			if (cameraCount == 3)
				cameraCount = 0;
			if (cameraCount == 0){
				camX = 3;
				camY = 1.8;
				camZ = mZ + 9.0;
				camAng = 0.0;
			}
			else if (cameraCount == 1){
				camX = 3;
				camY = 7.2;
				camZ = mZ + 30.0;
				camAng = 0.0;
			}
			else if (cameraCount == 2){
				camX = 3;
				camY = 6.2;
				camZ = mZ - 40.0;
				camAng = 180.0;
			}
		}

	}

}
double spin(void) // this function spin the main actor (ball) 
{
	if (gameOver == false && gamePause == false) {
		axis += 4.2;
		if (axis > 360.0)
			axis -= 360.0;
	}
	return axis;
}

double spin2(void) // this function spin happy world ball
{
	axis2 += 1.2;
	if (axis2 > 360.0)
		axis2 -= 360.0;
	return axis2;
}


// this function restart the game when enter is press and gameOver is true
void restartGame(){
	vectorOfEnamy.clear();
	generatedEnamyVector();
	axis = 0;
	axis2 = 0;
	camAng = 0.0;
	camX = 3;
	camY = 1.5;
	camZ = 8;
	gameOver = false;
	gamePause = true;
	gameRestart = false;
	startText = true;
	mZ = 0.0;
	mX = 0.0;
	cameraCount = 0;
	speed = speedArray[0];
}

void cameraContral(){ // this work when game is lost
		if (keystates['w'] == true || keystates['W'] == true){
			camX = camX - sin(camAng * 3.14 / 180) * .7; //here 3.14/180 is used to convert angle to radian
			camZ = camZ - cos(camAng * 3.14 / 180) * .7;
		}
		if (keystates['S'] == true || keystates['s'] == true){
			camX = camX + sin(camAng * 3.14 / 180) * .7;
			camZ = camZ + cos(camAng * 3.14 / 180) * .7;
		}
		if (keystates['a'] == true || keystates['A'] == true){
			camAng = camAng + 0.7;
		}
		if (keystates['D'] == true || keystates['d'] == true){
			camAng = camAng - 0.7;
		}
}

void addSpeed(){
	//// add more speed if needed
	if (round(abs(mZ)) < round(abs(roadLength * 2)) && round(abs(mZ)) > round(abs(roadLength))){
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0); 
	}else if (round(abs(mZ)) > round(abs(roadLength *2))){
		speed = speedArray[1];
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
}


//This Function is called every 10 milliseconds and this function does the neccessary variable updation of the game
void updategame(int value)
{
	glutTimerFunc(10, updategame, 0); //call updategame function after 10 milliseconds
	if (gameOver == false && gamePause == false){


		if (keystates['a'] == true || keystates['A'] == true)
		{
			if (mX > 0)
				mX--;
			keystates['a'] = false;
			keystates['A'] = false;

		}
		if (keystates['d'] == true || keystates['D'] == true)	{
			if (mX < 3)
				mX++;
			keystates['d'] = false;
			keystates['D'] = false;
		}
		addSpeed();
		//////
		camZ -= speed; // camera always goes the some speed like ball ( in Z coordinate)
		mZ -= speed;

	}
	// game Pause 
	if (gameOver == false){ // 33 is space code
		if (keystates[32] == true && gamePause == true){
			keystates[32] = false;
			gamePause = false;
		}
		else if (keystates[32] == true && gamePause == false){
			keystates[32] = false;
			gamePause = true;
			startText = false;
		}
	}
	// restarting game
	if (gameOver == true && keystates[13] == true){
		keystates[13] = false;
		restartGame();
	}
	//////////////////////////////////
	if (gameOver == true){
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		cameraContral();
	}
}

void drawIceRoad(unsigned texture = IceTexture){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);		// Draw The Cube Using quads
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(5.0f, 0.0f, -3.0f*roadLength);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, -3.0f*roadLength);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 3.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(5.0f, 0.0f, 3.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawLeftWall(unsigned texture = TreesTexture){
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);		// Draw The Cube Using quads
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(5.0f, 2.5f, 3.0f);	// Top Right Of The Quad (Left)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(5.0f, 2.5f, 0.0f);	// Top Left Of The Quad (Left)
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(5.0f, 0.0f, 0.0f);	// Bottom Left Of The Quad (Left)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(5.0f, 0.0f, 3.0f);	// Bottom Right Of The Quad (Left)
	glEnd();			// End Dra
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void drawRightWall(unsigned texture = TreesTexture){
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);		// Draw The Cube Using quads
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 2.5f, 0.0f);	// Top Right Of The Quad (Right)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 2.5f, 3.0f);	// Top Left Of The Quad (Right)
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 3.0f);	// Bottom Left Of The Quad (Right)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);	// Bottom Right Of The Quad (Right)
	glEnd();			// End Dra
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}


void drawSphere(){
	glPushMatrix();
	glTranslatef(1.5 + mX, 0.3, 3 * (0.8) + mZ);
	glRotatef(spin(), -1, 0, 0);
	glEnable(GL_BLEND);// for  transparent
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// for  transparent
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, SnowTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 0.3f, 16, 16);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);// for  transparent
	glPopMatrix();
}
void drawTrack(){
	glColor3f(1, 1, 1);
	glPushMatrix();
	for (int i = 0; i <= roadLength; i++){
		drawRightWall();
		drawLeftWall();
		glTranslatef(0, 0, -3);
	}
	glPopMatrix();
}

void drawOneEnamy(int rowPlace, int zLenght){
	glPushMatrix();
	glTranslatef(1.5 + rowPlace, 0.4, 3 * ((0.91) - zLenght * 3 - 4)); // enamy place coordinates
	glutSolidSphere(0.4, 16, 16);
	glTranslatef(0, 0.65, 0);
	glRotatef(110, -1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, SnowmanTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 0.3f, 16, 16);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void enamyReflection(){
	glEnable(GL_STENCIL_TEST); //Enable using the stencil buffer
	glColorMask(0, 0, 0, 0); //Disable drawing colors to the screen
	glDisable(GL_DEPTH_TEST); //Disable depth testing
	glStencilFunc(GL_ALWAYS, 1, 1); //Make the stencil test always pass
	//Make pixels in the stencil buffer be set to 1 when the stencil test passes
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//Set all of the pixels covered by the floor to be 1 in the stencil buffer
	drawIceRoad();
	glColorMask(1, 1, 1, 1); //Enable drawing colors to the screen
	glEnable(GL_DEPTH_TEST); //Enable depth testing
	//Make the stencil test pass only when the pixel is 1 in the stencil buffer
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Make the stencil buffer not change
	for (int i = 0; i < vectorOfEnamy.size(); i++){
		for (int j = 0; j < vectorOfEnamy[i].rowPosition.size(); j++){
			glPushMatrix();
			glScalef(1, -1, 1);
			glTranslatef(0, 0.1, 0);
			drawOneEnamy(vectorOfEnamy[i].rowPosition[j], i);
			glPopMatrix();
		}
	}

	glDisable(GL_STENCIL_TEST); //Disable using the stencil buffer
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// very important (without this work bad, I dont know why)
	glColor4f(1, 1, 1, 0.7f);
	drawIceRoad();
	glDisable(GL_BLEND);

}

void drawEnamies(){
	for (int i = 0; i < vectorOfEnamy.size(); i++){
		for (int j = 0; j < vectorOfEnamy[i].rowPosition.size(); j++){
			glPushMatrix();
			glColor3f(1, 1, 1);
			glPushMatrix();
			drawOneEnamy(vectorOfEnamy[i].rowPosition[j], i);
			glPopMatrix();
			if (mX == vectorOfEnamy[i].rowPosition[j] && round(mZ) == (3 * (-i * 3 - 4) + 1) || round(mZ) == (-3 * roadLength - 2)){   // check collisions  +1 that snowman not eat ball grafically and check is not road end
				gameOver = true;                                                  
			}
		}
	}

}

void drawHappyWorld(){
	glPushMatrix();
	glTranslatef(3, 13.0 / 2, -roadLength * 3 - (2 * 13));
	glRotatef(spin2(), 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, HappyTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 15.0f, 20, 20);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void display()
{
	// Clear screen
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Position camera
	glRotatef(-camAng, 0, 1, 0);
	glTranslatef(-camX, -camY, -camZ);
	//My funcions
	drawEnamies();
	drawTrack();
	enamyReflection();
	drawHappyWorld();
	drawSphere();
	// writes on screen when pause is it
	if (gamePause == true && startText==false)
		drawTextOnScreen(2.7, 1.9, mZ - 3.0, "Pause");
	else if (gamePause == true && startText == true)
		drawTextOnScreen(2.2, 2.1, mZ - 3.0, "START GAME");


	glColor3f(1, 1, 1);
	glutSwapBuffers();
}

//This function is called upon windows resize
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (GLdouble)w / (GLdouble)h, 0.5, 190.0);
	glMatrixMode(GL_MODELVIEW);
}


void main(int argc, char **argv)
{
	generatedEnamyVector();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Fun Winter Game");
	LoadImages();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(KeyDownPress);
	glutKeyboardUpFunc(KeyUpPress);
	glutMouseFunc(mouseComera);
	glutMouseWheelFunc(mouseWheel);
	//Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glutTimerFunc(10, updategame, 0);
	glutIdleFunc(display);
	glutMainLoop();
}

void generatedEnamyVector(){

	int partOfRoad = roadLength / 3;

	for (int i = 1; i < partOfRoad; i++){
		int rowSize = rand() % 3;
		vector<int> size;
		for (int j = 0; j <= rowSize; j++){
			int temp = rand() % 4;
			if (j == 0){
				size.push_back(temp);
			}
			else if (j == 1){
				if (temp != size[0])
					size.push_back(temp);
			}
			else if (j == 2){
				if (temp != size[0] && size.size() == 1)
					size.push_back(temp);
				else if (temp != size[0] && temp != size[1] && size.size() == 2)
					size.push_back(temp);

			}
		}

		vectorOfEnamy.push_back({ size });
	}

}