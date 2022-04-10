#include "ObjectType.h"
#include "TextureLoader.h"
object::object(object_type modelType, XMFLOAT3& pos, XMFLOAT4& rot, XMFLOAT3& scale) :m_model_type_(modelType), m_texture_(), col_()
{
	m_texture_ = TextureLoader::LoadBMP("checker.bmp");
	m_wall_text_ = TextureLoader::LoadBMP("wall.bmp");
}
object::object(object_type modelType) : m_model_type_(modelType), m_texture_()
{
	m_texture_ = TextureLoader::LoadBMP("checker.bmp");
	m_wall_text_ = TextureLoader::LoadBMP("w.bmp");
}
void object::render(XMVECTOR& pos, XMVECTOR& r, XMVECTOR& s) {
	translate_ = pos;
	rot_ = r;
	scale_ = s;
	//col = co;
	switch (m_model_type_)
	{
	case object_type::sphere:
		render_sphere();
		break;
	case object_type::cube:
		render_cube();
		break;
	case object_type::plane:
		render_cube();
		break;
	case object_type::peg:
		render_peg();
		break;
	default:
		m_fail_ = false;
		break;
	}
}


void object::render_cube() const
{
	glClearDepth(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	//Lside
	glLoadIdentity();
	if (XMVectorGetZ(rot_) > 0)
	{
		glTranslatef(XMVectorGetX(translate_), XMVectorGetY(translate_), XMVectorGetZ(translate_));
		glRotated(40, 0, 0, 1);
	}
	else if (XMVectorGetZ(rot_) < 0)
	{
		glTranslatef(XMVectorGetX(translate_), XMVectorGetY(translate_), XMVectorGetZ(translate_));
		glRotated(-40, 0, 0, 1);
	}
	else
	{
		glTranslatef(XMVectorGetX(translate_), XMVectorGetY(translate_), XMVectorGetZ(translate_));
	}
	glColor3f(XMVectorGetX(col_), XMVectorGetY(col_), XMVectorGetZ(col_));
	glScaled(XMVectorGetX(scale_), XMVectorGetY(scale_), XMVectorGetZ(scale_));
	glColor3d(1, 1, 1);
	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
									  // Top face (y = 1.0f)
									  // Define vertices in counter-clockwise (CCW) order with normal pointing out
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  // End of drawing color-cube
	glPopMatrix();
	glFlush();
}

void object::render_sphere() const
{
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(XMVectorGetX(translate_), XMVectorGetY(translate_), XMVectorGetZ(translate_));
	glRotatef(3, XMVectorGetX(rot_), XMVectorGetY(rot_), XMVectorGetZ(rot_));
	glScaled(XMVectorGetX(scale_), XMVectorGetY(scale_), XMVectorGetZ(scale_));
	glColor3d(1, 1, 0);
	// Select Our Texture
	GLUquadric* quadric = gluNewQuadric();
	glBindTexture(GL_TEXTURE_2D, m_texture_);
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, GL_TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, 0.5, 20, 20);
	glPopMatrix();
}

void object::render_peg() const
{
	//glClearColor(1, 1, 1, 1);
	//glDisable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(XMVectorGetX(translate_), XMVectorGetY(translate_), 0);
	glColor3d(0.5, 0.2, 0.7);
	GLUquadric* quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, GL_FALSE);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluCylinder(quadric, XMVectorGetX(scale_), XMVectorGetY(scale_), XMVectorGetZ(scale_), 20, 20);
	//glFinish();
}
