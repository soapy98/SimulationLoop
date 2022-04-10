//#include "Sphere.h"
//#include <algorithm>
//#include <Windows.h>
//#include <gl\gl.h>  
//#include <gl\GLU.h>
//#define _USE_MATH_DEFINES
//#include <cmath>
//#include "TextureLoader.h"
//#include "Board.h"
//#include "Vect3f.h"
//#include "Peg.h"
//#include <iostream>
//
//int Sphere::countID = 0;
//
//Sphere::Sphere(void) : m_mass(1), m_radius(0.3)
//{
//	m_objectID = countID;
//	++countID;
//	m_texture = TextureLoader::LoadBMP("checker.bmp");
//}
//
//Sphere::~Sphere(void)
//{
//}
//void Sphere::ResetPos()
//{
//	m_newPos = m_pos;
//}
//
//void Sphere::Update()
//{
//	m_velocity = m_newVelocity;
//	m_pos = m_newPos;
//}
//
//void Sphere::SetPos(float x, float y)
//{
//	m_pos.Set(x, y);
//}
//
//void Sphere::SetNewPos(Vector2f pos)
//{
//	m_newPos = pos;
//}
//
//void Sphere::SetVel(float x, float y)
//{
//	m_velocity.Set(x, y);
//}
//
//void Sphere::SetNewVel(Vector2f vel)
//{
//	m_newVelocity = vel;
//}
//
//void Sphere::SetMass(float mass)
//{
//	m_mass = mass;
//}
//float Sphere::GetMass() const
//{
//	return m_mass;
//}
//
//Vector2f Sphere::GetPos() const
//{
//	return m_pos;
//}
//
//Vector2f Sphere::GetNewPos() const
//{
//	return m_newPos;
//}
//
//Vector2f Sphere::GetVel() const
//{
//	return m_velocity;
//}
//
//Vector2f Sphere::GetNewVel() const
//{
//	return m_newVelocity;
//}
//
//float Sphere::GetRadius() const
//{
//	return m_radius;
//}
//
//void Sphere::Render() const
//{
//	glLoadIdentity();
//	glPushMatrix();
//	glTranslatef(m_pos.GetX(), m_pos.GetY(), -5);
//	glColor3d(1, 0, 0);
//	glBindTexture(GL_TEXTURE_2D, m_texture);               // Select Our Texture
//	GLUquadric* quadric = gluNewQuadric();
//	gluQuadricDrawStyle(quadric, GLU_FILL);
//	gluQuadricTexture(quadric, GL_TRUE);
//	gluQuadricNormals(quadric, GLU_SMOOTH);
//	gluSphere(quadric, m_radius, 20, 20);
//	glPopMatrix();
//}
//
//void Sphere::CalculatePhysics(float dt)
//{
//	Vector2f dx, dx1, dx2, dx3, dx4, dv, dv1, dv2, dv3, dv4;
//	//// Calcuate total force for this sphere, e.g. F = F1+F2+F3+...
//	Vector2f force(0.0f, -9.81f * m_mass);
//
//	m_acc = (force / m_mass);
//	dx1 = m_velocity * dt;
//	dv1 = (m_acc * dt) * 0.8;
//
//	dx2 = (m_velocity + dv1 / 2) * dt;
//	dv2 = (m_acc + m_acc * dx1 / 2) * dt;
//
//	dx3 = (m_velocity + dv2 / 2) * dt;
//	dv3 = (m_acc + m_acc * dx2 / 2) * dt;
//
//	dx4 = (m_velocity + dv3 / 2) * dt;
//	dv4 = (m_acc + m_acc * dx3 / 2) * dt;
//
//	m_newPos = m_pos + (dx1 + dx2 * 2 + dx3 * 3 + dx4) / 6;
//	m_newVelocity = (m_velocity + (dv1 + dv2 * 2 + dv3 * 2 + dv4) / 6);
////Collision of 2 particles
//	
//	if (m_newPos.GetY() < -10.0f + m_radius)
//	{
//		
//		m_newPos.Set(m_newPos.GetX(), -10.0f + m_radius);
//		m_newVelocity.Set(m_acc.GetX()*-friction,m_acc.GetY()*-friction);
//friction /= 2;
//
//	}
//}
//void Sphere::CollisionWithSphere(Sphere* sphere2, ContactManifold* contactManifold)
//{
//	Vector2f pos1 = this->GetNewPos();
//	Vector2f pos2 = sphere2->GetNewPos();
//	float dist = pos1.distance(pos2) - (this->GetRadius() + sphere2->GetRadius());
//	float overlap = 0.03f;
//
//	if (dist < 0.0f)
//	{
//		m_newPos.Set(m_pos.GetX() - (overlap * (this->GetPos().GetX() - sphere2->GetPos().GetX())) / dist, m_pos.GetY() - (overlap * (this->GetPos().GetY() - sphere2->GetPos().GetY()) / dist));
//		sphere2->SetNewPos(Vector2f(sphere2->GetPos().GetX() + (overlap * (this->GetPos().GetX() - sphere2->GetPos().GetX()) / dist), sphere2->GetPos().GetY() + (overlap * (this->GetPos().GetY() - sphere2->GetPos().GetY()) / dist)));
//
//		/*	ManifoldPoint mp;
//			mp.contactID1 = this;
//			mp.contactID2 = sphere2;
//			mp.contactNormal = (pos2 - pos1).normalise();
//			contactManifold->Add(mp);*/
//	}
//
//}
//
//void Sphere::dynamicCol(Sphere* s, ContactManifold* contactManifold)
//{
//	Vector2f pos1 = this->GetNewPos();
//	Vector2f pos2 = s->GetNewPos();
//	float dist = pos1.distance(pos2) - (this->GetRadius() + s->GetRadius());
//	if (dist < 0.0f) {
//		ManifoldPoint mp;
//		mp.contactID1 = this;
//		mp.contactID2 = s;
//		mp.contactNormal = (pos2 - pos1).normalise();
//		contactManifold->Add(mp);
//	}
//}
//void Sphere::SphereResponse(ManifoldPoint& point)
//{
//	//Vector2f pos1 = point.contactID1->GetNewPos();
//	//Vector2f pos2 = point.contactID2->GetNewPos();
//	//float dist = pos1.distance(pos2) - (this->GetRadius() + point.contactID2->GetRadius());
//	//float nx = (point.contactID2->GetPos().GetX() - this->GetPos().GetX()) / dist;
//	//float ny = (point.contactID2->GetPos().GetY() - this->GetPos().GetY()) / dist;
//	//float tx = -ny;
//	//float ty = nx;
//	//float dpTan1 = point.contactID2->GetVel().GetX() * tx + point.contactID2->GetVel().GetY() * ty;
//	//float dpTan2 = this->GetVel().GetX() * tx + this->GetVel().GetY() * ty;
//	//float dpNorm1 = point.contactID2->GetVel().GetX() * nx + point.contactID2->GetVel().GetY() * ny;
//	//float dpNorm2 = this->GetVel().GetX() * nx + this->GetVel().GetY() * ny;
//	//float m1 = (dpNorm1 * (this->GetMass() - point.contactID2->GetMass()) + 2.0f * point.contactID2->GetMass() * dpNorm2) / this->GetMass() + point.contactID2->GetMass();
//	//float m2 = (dpNorm1 * (point.contactID2->GetMass() - this->GetMass()) + 2.0f * this->GetMass() * dpNorm1) / this->GetMass() + point.contactID2->GetMass();
//	/*this->GetNewVel().Set(this->GetVel().GetX() + (tx * dpTan1 + nx * m1), this->GetVel().GetX() + ty * dpTan1 + ny * m1);
//	point.contactID2->GetNewVel().Set(point.contactID2->GetVel().GetX() + tx * dpTan2 + nx * m1, point.contactID2->GetVel().GetY() + ty * dpTan2 + ny * m1);*/
//	auto newVel1 = (this->GetVel() * (this->GetMass() - point.contactID2->GetMass()) + (2 * point.contactID2->GetMass() * point.contactID2->GetVel())) / (this->GetMass() + point.contactID2->GetMass());
//	auto newVel2 = (point.contactID2->GetVel() * (point.contactID2->GetMass() - this->GetMass()) + (2 * this->GetMass() * this->GetVel())) / (this->GetMass() + point.contactID2->GetMass());
//	point.contactID2->SetNewVel(newVel2);
//	this->SetNewVel(newVel1);
//}
//
//
//float Sphere::DistPointAABB(Board* b)
//{
//	float sqDist = 0.0f;
//	float v;
//	// test the bounds against the points X axis
//	v = this->GetPos().GetX();
//
//	if (v < b->GetXMinMax().GetX()) sqDist += (b->GetXMinMax().GetX() - v) * (b->GetXMinMax().GetX() - v);
//	if (v > b->GetXMinMax().GetY()) sqDist += (v - b->GetXMinMax().GetY()) * (v - b->GetXMinMax().GetY());
//
//	// test the bounds against the points Y axis
//	v = this->GetPos().GetY();
//
//	if (v < b->GetYMinMax().GetX()) sqDist += (b->GetYMinMax().GetX() - v) * (b->GetYMinMax().GetX() - v);
//	if (v > b->GetYMinMax().GetY()) sqDist += (v - b->GetYMinMax().GetY()) * (v - b->GetYMinMax().GetY());
//
//	return sqDist;
//
//}
//
//void Sphere::testSphereAABB(Board* b, ContactManifold* cm)
//{
//	float sqDist = DistPointAABB(b);
//	if (sqDist <= this->GetRadius() * this->GetRadius())
//	{
//		Vector2f pos1 = this->GetNewPos();
//		Vector2f pos2 = b->GetPos();
//		ManifoldPoint mp;
//		mp.contactID1 = this;
//		mp.board = b;
//		mp.contactNormal = (pos2 - pos1).normalise();
//		cm->Add(mp);
//	}
//}
//
//
//
//
//
//
//void Sphere::CollisionWithPeg(Peg* p, ContactManifold* contactManifold)
//{
//	Vector2f pos1 = this->GetNewPos();
//	Vector2f pos2 = p->GetPos();
//	float dist = pos1.distance(pos2) - (this->GetRadius() + p->GetRadius());
//	if (dist < 0.0f)
//	{
//		ManifoldPoint mp;
//		mp.contactID1 = this;
//		mp.peg = p;
//		mp.contactNormal = (pos2 - pos1).normalise();
//		contactManifold->Add(mp);
//	}
//}
//void Sphere::CollisionResponsePeg(ManifoldPoint& point)
//{
//	Vector2f colNormal = point.contactNormal;
//
//	point.contactID1->ResetPos();
//	point.contactID1->SetNewVel(-0.5f * colNormal * colNormal.dot(point.contactID1->GetVel()));
//
//}
//
//
//void Sphere::CollisionResponseWithSphere(ManifoldPoint& point)
//{
//
//	Vector2f colNormal = point.contactNormal;
//
//	point.contactID1->ResetPos();
//	point.contactID1->SetNewVel(-1.0f * colNormal * colNormal.dot(point.contactID1->GetVel()));
//
//	point.contactID2->ResetPos();
//	point.contactID2->SetNewVel(-1.0f * colNormal * colNormal.dot(point.contactID2->GetVel()));
//}
//void Sphere::ResponsewithBoard(ManifoldPoint& point)
//{
//	Vector2f colNormal = Vector2f(0, 1);
//	
//
//	Vector2f j = -(1 + 1.1) * colNormal;
//	//j.divide(1 / m_mass);
//	//Vector2f impulse = j * colNormal;
//	point.contactID1->SetNewVel(point.contactID1->GetVel() + point.contactID1->GetVel().dot(j) * colNormal);
//}
//
//
//
//
//Vector2f Sphere::GetLinearVelocity(float angle)
//{
//	angle -= 90;
//	if (angle < 0)angle = 360 + angle;
//
//	auto a = std::sin(angle * (3.1415926f / 180.0f));
//	return Vector2f(a, a);
//}
//
//
//
//
//
//
