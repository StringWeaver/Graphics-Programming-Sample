// OpenGLStarter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//��Ҫ������ͷ�ļ�
#include <stdio.h>
#include <windows.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#pragma comment(lib, "glew32.lib")


//����������ڵĴ�С
#define WINDOW_HEIGHT 300
#define WINDOW_WIDTH 500

GLuint G_vShader_simple;
GLuint G_fShader_simple;
GLuint G_shaderProgram;

//�����������ľ���
float G_fDistance = 3.6f;
//�������ת�Ƕ� 
float G_fAngle_horizon = 0.0;
float G_fAngle_vertical = 0.0f;

////////////////////////////////////////////////
//���ղ���
float G_vLit0Position[4] = { 5.0f, 5.0f, 5.0f, 1.0f };
float G_vLit0Ambient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
float G_vLit0Diffuse[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
float G_vLit0Specular[4] = { 0.6f, 0.6f, 0.6f, 1.0f };

float G_vMaterialAmbient[4] = { 0.6f, 0.3f, 0.3f, 1.0f };
float G_vMaterialDiffuse[4] = { 1.0f, 0.5f, 0.5f, 1.0f };
float G_vMaterialSpecular[4] = { 1.0f, 0.5f, 0.5f, 1.0f };
float G_iShininess = 50;

////////////////////////////////////////////////
void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);
void setShaders(void);
void printShaderInfoLog(GLuint shaderObject);
void printProgramInfoLog(GLuint programObject);
char* textFileRead(const char *textFileName);

//��Ӧ��������, �Ӷ��趨�����ƽ���Զ�Լ���ת�Ļص�����
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key,int x,int y);

////////////////////////////////////////////////
//������
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	//��ʼ��OPENGL��ʾ��ʽ
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
//	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA);

	//�趨OPENGL����λ�úʹ�С
	glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT); 
	glutInitWindowPosition (100, 100);
		
	//�򿪴���
	glutCreateWindow ("OpenGL");

	//���ó�ʼ������
    myinit();

	//�趨���ڴ�С�仯�Ļص�����
	glutReshapeFunc(myReshape);

	//�趨���̿��ƵĻص�����
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);
	
	//��ʼOPENGL��ѭ��
	glutDisplayFunc(display); 
//	glutIdleFunc(display);

	glutMainLoop();

	return 0;
}

////////////////////////////////////////////////
//�û���ʼ������
void myinit(void)
{
    //your initialization code
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_COLOR_MATERIAL);
//	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	GLenum err = glewInit();   
	if (GLEW_OK != err)   
	{   
		printf("glew initionlize error: %s\n", glewGetErrorString(err));
	}
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		printf("Ready for GLSL\n");
	else {
		printf("Not totally ready \n");
		exit(1);
	}

	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else 
	{ 
		printf("OpenGL 2.0 not supported\n"); 
		exit(1);
	}

	setShaders();
}

//���ڴ�С�仯ʱ�Ļص�����
void myReshape(GLsizei w, GLsizei h)
{
	//�趨����
    glViewport(0, 0, w, h);

	//�趨͸�ӷ�ʽ
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60.0, 1.0*(GLfloat)w/(GLfloat)h, 1.0, 30.0);
//	gluPerspective(60.0, 1.0, 1.0, 30.0);
//  glFrustum (-1.0, 1.0, -1.0, 1.0, 1.0, 30.0);
}

//ÿ��OpenGL�����������������û�Ӧ�ð���ʾ����������������
void display(void)
{
	//���������Ļ����ɫ���������Ļ����Ȼ���
    glClearColor(1.0f,1.0f,1.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	//���ù��ա����ʲ���
	glLightfv(GL_LIGHT0, GL_POSITION, G_vLit0Position);		//���ù�Դ��λ��
	glLightfv(GL_LIGHT0, GL_AMBIENT, G_vLit0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, G_vLit0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, G_vLit0Specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, G_vMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, G_vMaterialDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, G_vMaterialSpecular);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, G_iShininess);

	//����������Z��ƽ��-G_fDistance(ʹ��������λ�������ǰ��)����������ת�Ƕ�
    glTranslatef(0.0, 0.0, -G_fDistance);
	glRotatef(G_fAngle_horizon, 0.0f, 1.0f, 0.0f);
	glRotatef(G_fAngle_vertical, 1.0f, 0.0f, 0.0f);

	////////////////////////////////////////////////
	//��������

	//�ù̶����߻�һ�����
	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glTranslatef(1.5f, 0.0f, 0.0f);
	glutSolidTeapot(1.0);
//	glutSolidSphere(1.0, 10, 10);
	glPopMatrix();

	//��shader��һ�����
	glUseProgram(G_shaderProgram);

	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glTranslatef(-1.0f, 0.0f, 0.0f);
	glutSolidTeapot(1.0);
//	glutSolidSphere(1.0, 10, 10);
	glPopMatrix();

	glUseProgram(0);

	glutSwapBuffers();
}


void processSpecialKeys(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_LEFT:
			G_fAngle_horizon -= 5.0f;
			break;
		case GLUT_KEY_RIGHT:
			G_fAngle_horizon += 5.0f;
			break;
		case GLUT_KEY_UP:
			G_fAngle_vertical -= 5.0f;
			break;
		case GLUT_KEY_DOWN:
			G_fAngle_vertical += 5.0f;
			break;
	}
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key,int x,int y)
{
	switch(key) {
		case 97:	//"a"
			G_fDistance -= 0.3f;
			break;
		case 65:		//"A"
			G_fDistance += 0.3f;
			break;
		case 27:	//"esc"
			exit(0);
	}
	glutPostRedisplay();
}

void setShaders(void)
{  
    char *vs,*fs;  
  
    G_vShader_simple = glCreateShader(GL_VERTEX_SHADER);  
    G_fShader_simple = glCreateShader(GL_FRAGMENT_SHADER);    
  
    vs = textFileRead("Shader/simple.vert");  
    fs = textFileRead("Shader/simple.frag");  
  
    const char *vv = vs;  
    const char *ff = fs;  
  
    glShaderSource(G_vShader_simple, 1, &vv, NULL);  
    glShaderSource(G_fShader_simple, 1, &ff, NULL);  
  
	free(vs);
	free(fs);
	/////////////////////////////////////////////////////////
    glCompileShader(G_vShader_simple);  
    glCompileShader(G_fShader_simple);  

	int checkResult;
	glGetShaderiv(G_vShader_simple, GL_COMPILE_STATUS, &checkResult);  
	if(GL_FALSE == checkResult)
	{
		printf("vertex shader compile error\n");
		printShaderInfoLog(G_vShader_simple);
	}

	glGetShaderiv(G_fShader_simple, GL_COMPILE_STATUS, &checkResult);  
	if(GL_FALSE == checkResult)
	{
		printf("fragment shader compile error\n");
		printShaderInfoLog(G_fShader_simple);
	}
	////////////////////////////////////////////////////////////
    G_shaderProgram = glCreateProgram();  
  
    glAttachShader(G_shaderProgram, G_vShader_simple);  
    glAttachShader(G_shaderProgram, G_fShader_simple);  
  
    glLinkProgram(G_shaderProgram);  
	glGetProgramiv(G_fShader_simple, GL_LINK_STATUS, &checkResult);  
	if(GL_FALSE == checkResult)
	{
		printf("shader link error\n");
		printProgramInfoLog(G_shaderProgram);
	}
//    glUseProgram(G_shaderProgram);  
}  

char* textFileRead(const char *textFileName)
{
	FILE *fp;

    if(NULL == (fp = fopen(textFileName, "r")))  
    {  
        printf("text file read error\n");  
		exit(1);  
    }  
  
    char ch;
	int fileLen = 0;
	//���ȵõ��ļ�����
	while(EOF != (ch=fgetc(fp)))  
    {  
        fileLen ++;  
    }

	char *fileStr = (char *)malloc((fileLen+1)*sizeof(char));
	//�ڶ��ζ�ȡ�ļ�
	rewind(fp);
	int i = 0;
    while(EOF != (ch=fgetc(fp)))  
    {  
        fileStr[i] = ch;
		i++;
    }  
	fileStr[fileLen] = '\0';	//ע�����һ��Ҫ�ӡ�
  
    fclose(fp);
	return fileStr;
}

void printShaderInfoLog(GLuint shaderObject)
{
	GLint logLen = 0;
	GLint writtenLen = 0;
	GLchar* info_log;

	glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH , &logLen);       

	if (logLen > 1)
	{
		info_log = (GLchar*)malloc(logLen);
//		glGetInfoLogARB(shaderObject, logLen, &writtenLen, info_log);	//Ҳ�������ϰ汾�ĺ����ˡ�
		glGetShaderInfoLog(shaderObject, logLen, &writtenLen, info_log);  
//		printf("Information log: \n");
		printf("%s\n", info_log);
		free (info_log);
	}
}

void printProgramInfoLog(GLuint programObject)
{
	GLint logLen = 0;
	GLint writtenLen = 0;
	GLchar* info_log;

	glGetShaderiv(programObject, GL_INFO_LOG_LENGTH , &logLen);       

	if (logLen > 1)
	{
		info_log = (GLchar*)malloc(logLen);
//		glGetInfoLogARB(shaderObject, logLen, &writtenLen, info_log);
		glGetProgramInfoLog(programObject, logLen, &writtenLen, info_log);  
//		printf("Information log: \n");
		printf("%s\n", info_log);
		free (info_log);
	}
}