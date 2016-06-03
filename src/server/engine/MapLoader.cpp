#include "MapLoader.h"
#include "ObjectId.h"
#include <ConvexDecomposition\ConvexDecomposition.h>
#include <ConvexDecomposition\ConvexBuilder.h>
#include "ConvexDecomposition\cd_vector.h"
#include "ConvexDecomposition\cd_hull.h"
#include <ConvexDecomposition\cd_wavefront.h>

#include <BulletCollision\CollisionShapes\btTriangleMesh.h>
#include <BulletCollision\CollisionShapes\btShapeHull.h>
#include <BulletCollision\CollisionShapes\btBvhTriangleMeshShape.h>

/*
#include "ConvexDecomposition\bestfit.h"
#include "ConvexDecomposition\planetri.h"
#include "ConvexDecomposition\vlookup.h"
#include "ConvexDecomposition\splitplane.h"
#include "ConvexDecomposition\meshvolume.h"
#include "ConvexDecomposition\concavity.h"
#include "ConvexDecomposition\bestfitobb.h"
#include "ConvexDecomposition\float_math.h"
#include "ConvexDecomposition\fitsphere.h"
*/

MapLoader::MapLoader(btDiscreteDynamicsWorld* dynamicsWorld)
{
	this->curWorld = dynamicsWorld;
}

MapLoader::~MapLoader()
{
	//delete fileNames;
}


// Loads all the object files as Rigid Bodies into the physics world
void MapLoader::loadMap()
{

	ConvexDecomposition::WavefrontObj wobj;

	// push all the strings for loading objects
	std::string* file;
	/*
	file = new std::string("./assets/map/new_objects/structures/barn.obj");
	fileNames.push_back((*file).c_str());
	file = new std::string("./assets/map/new_objects/structures/bench.obj");
	fileNames.push_back((*file).c_str());
	file = new std::string("./assets/map/new_objects/structures/house.obj");
	fileNames.push_back((*file).c_str());
	file = new std::string("./assets/map/new_objects/structures/boat.obj");
	fileNames.push_back((*file).c_str());
	file = new std::string("./assets/map/new_objects/structures/floating_hottub.obj");
	fileNames.push_back((*file).c_str());
	file = new std::string("./assets/map/new_objects/structures/silo.obj");
	fileNames.push_back((*file).c_str());
	file = new std::string("./assets/map/new_objects/structures/windmill.obj");
	fileNames.push_back((*file).c_str());
	file = new std::string("./assets/map/new_objects/structures/house_under_construction.obj");
	fileNames.push_back((*file).c_str());
	file = new std::string("./assets/map/new_objects/structures/patio.obj");
	fileNames.push_back((*file).c_str());
	file = new std::string("./assets/map/new_objects/nature/pumpkin_patch.obj");
	fileNames.push_back((*file).c_str());*/


	int result;
	for (auto const& file : fileNames)
	{
		result = wobj.loadObj(file);
		printf("loading file: %s\n", file);

		// Error loops if the obj file wasn't loaded correctly
		while (!result)
		{
			printf("loading failed \"%s\"\n", file);
		}

		btTriangleMesh* trimesh = new btTriangleMesh();
		int i;
		for (i = 0; i < wobj.mTriCount; i++)
		{
			int index0 = wobj.mIndices[i * 3];
			int index1 = wobj.mIndices[i * 3 + 1];
			int index2 = wobj.mIndices[i * 3 + 2];

			btVector3 vertex0(wobj.mVertices[index0 * 3], wobj.mVertices[index0 * 3 + 1], wobj.mVertices[index0 * 3 + 2]);
			btVector3 vertex1(wobj.mVertices[index1 * 3], wobj.mVertices[index1 * 3 + 1], wobj.mVertices[index1 * 3 + 2]);
			btVector3 vertex2(wobj.mVertices[index2 * 3], wobj.mVertices[index2 * 3 + 1], wobj.mVertices[index2 * 3 + 2]);

			trimesh->addTriangle(vertex0, vertex1, vertex2);
		}

		btBvhTriangleMeshShape* tmpConvexShape = new btBvhTriangleMeshShape(trimesh, false);

		/*
		// Reducing vertices and lines by converting to a btShapeHull
		btShapeHull* hull = new btShapeHull(tmpConvexShape);
		btScalar margin = tmpConvexShape->getMargin();
		hull->buildHull(margin);
		tmpConvexShape->setUserPointer(hull);

		btConvexHullShape* convexShape = new btConvexHullShape();
		bool updateLocalAabb = false;

		for (i = 0; i<hull->numVertices(); i++)
		{
		convexShape->addPoint(hull->getVertexPointer()[i], updateLocalAabb);
		}
		convexShape->recalcLocalAabb();

		//if (sEnableSAT)
		convexShape->initializePolyhedralFeatures();
		delete tmpConvexShape;
		delete hull;
		*/

		btDefaultMotionState*playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
		//btScalar mass = 500;
		//btVector3 playerInertia(0, 0, 0);
		//convexShape->calculateLocalInertia(mass, playerInertia);
		btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(0, playerMotionState, tmpConvexShape, btVector3(0, 0, 0));
		btRigidBody* pRigidBody = new btRigidBody(playerRigidBodyCI);
		pRigidBody->setFriction((btScalar)0.5);
		pRigidBody->setDamping((btScalar)100, (btScalar)100);
		pRigidBody->setUserIndex(ClassId::OBSTACLE);
		curWorld->addRigidBody(pRigidBody);

	}

}


