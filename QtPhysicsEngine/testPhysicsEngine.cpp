#include "testPhysicsEngine.h"
#include "QtPhysicsEngine.h"
#include "TestSettings.h"
#include "Renderable.h"

void PhysicsEngineTest::initTestCase()
{

}

void PhysicsEngineTest::testXMLRead()
{
	QtPhysicsEngine testEngine;
	testEngine.loadFromFile(TestSettings::getTestFileDirectory() + "\\SceneRendering\\test_cube.xml");
	QVector<Renderable> objects = testEngine.getObjects();
	QVERIFY(objects.size() == 1);
	Renderable object = objects.at(0);

	QMatrix4x4 modelMatrix(1.00000, 0.00000, 0.00000, 0.00000, 0.00000, 1.00000, 0.00000, 0.00000, 0.00000, 0.00000, 1.00000, 0.00000, 0.00000, 0.00000, 0.00000, 1.00000);
	QVERIFY(modelMatrix == object.getModelMatrix());

	QVector3D velocity(15.00000, -7.00000, 12.5000);
	QVERIFY(velocity == object.getVelocity());
}

void PhysicsEngineTest::testXMLReadFailure()
{
	QtPhysicsEngine testEngine;
	int res = testEngine.loadFromFile(TestSettings::getTestFileDirectory() + "\\SceneRendering\\nonexistant.xml");
	QVERIFY(res != tinyxml2::XML_SUCCESS);

	QVector<Renderable> objects = testEngine.getObjects();
	QVERIFY(objects.size() == 0);
}

void PhysicsEngineTest::testRayCollision()
{
	//bounding box 
	QVector3D vecMin(-1.0, -1.0, -1.0);
	QVector3D vecMax(1.0, 1.0, 1.0);
	AABB boundingBox(vecMin, vecMax);

	// -------------- SIMPLE TEST CASE 1: ray moving along z towards bounding box at the origin ------------------//
	QVector3D origin1(0, 0, -30);
	QVector3D direction1(0, 0, 1);
	QVERIFY(boundingBox.intersects(origin1, direction1));

	// --------------- SIMPLE TEST CASE 2: intersection method considers only touching edge as not a proper collision ------------//
	QVector3D origin2(1, 0, -30);
	QVERIFY(!boundingBox.intersects(origin2, direction1));
	QVector3D origin3(0.9999, 0, -30);
	QVERIFY(boundingBox.intersects(origin3, direction1));

	// --------------- SIMPLE TEST CASE 3: ray slightly angled ------------//
	QVector3D direction2(-0.006397, 0.0750924, 0.997156);
	QVERIFY(!boundingBox.intersects(origin1, direction2));
}

void PhysicsEngineTest::testAABBCollision()
{
	// --------------- SIMPLE TEST CASE 1: two AABBs in the same place ------------//
	QVector3D vecMin1(-1.0, -1.0, -1.0);
	QVector3D vecMax1(1.0, 1.0, 1.0);
	AABB boundingBox1(vecMin1, vecMax1);

	QVector3D vecMin2(-1.0, -1.0, -1.0);
	QVector3D vecMax2(1.0, 1.0, 1.0);
	AABB boundingBox2(vecMin2, vecMax2);

	QVERIFY(boundingBox1.intersects(boundingBox2));
	QVERIFY(boundingBox2.intersects(boundingBox1));

	// --------------- SIMPLE TEST CASE 2: two AABBs in that do not collide ------------//

	QVector3D vecMin3(1.5, -1.0, -1.0);
	QVector3D vecMax3(2.5, 1.0, 1.0);
	AABB boundingBox3(vecMin3, vecMax3);
	QVERIFY(!boundingBox1.intersects(boundingBox3));
	QVERIFY(!boundingBox3.intersects(boundingBox1));
}