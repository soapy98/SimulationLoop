#include <Windows.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include "TextureLoader.h"
#include "Vect3f.h"
//
//Peg::Peg(void) :m_radius(0.2)
//{
//	/*m_objectID = countID;
//	++countID;*/
//	m_texture = TextureLoader::LoadBMP("checker.bmp");
//}
//Peg::~Peg(void)
//{
//}
//
//void Peg::Render() const
//{
//	glEnable(GL_DEPTH_TEST);
//	//glRotatef(10, 0, 0, 1);
//	glLoadIdentity();
//	glTranslatef(m_pos.GetX(), m_pos.GetY(), -5);
//	//glColor3d(1, 0, 0);
//	//glBindTexture(GL_TEXTURE_2D, m_texture);               // Select Our Texture
//	GLUquadric* quadric = gluNewQuadric();
//	gluQuadricDrawStyle(quadric, GLU_FILL);
//	gluQuadricTexture(quadric, GL_TRUE);
//	gluQuadricNormals(quadric, GLU_SMOOTH);
//	gluCylinder(quadric, m_radius,m_radius, 1.0, 20, 20);
//}
//
//void Peg::SetPos(float x, float y)
//{
//	m_pos.Set(x, y);
//}
//Vector2f Peg::GetPos() const
//{
//	return m_pos;
//}
//
//float Peg::GetRadius() const
//{
//	return m_radius;
//}
