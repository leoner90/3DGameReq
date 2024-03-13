#include "Game.h"
#include "headers/Map.h"

Map::Map()
{
	font.LoadDefault();

	// wall model
	wallSegment.LoadModel("wall/wall.obj");
	wallSegment.SetScale(10.0f);
	wallSegment.SetToFloor(0);

	//forest
	tree.LoadModel("tree/tree1.obj");
	//random scale in load function
	tree.SetToFloor(0);

	collidingTree.LoadModel("tree/tree1.obj");
	//random scale in load function
	collidingTree.SetToFloor(0);

	// floor texture
	//floor.LoadTexture("Map/floor2.bmp");
	floor.LoadTexture("Map/floor.jpg");
	floor.SetTiling(true);

	// rock model
	rock.LoadModel("rock2/rock3.obj");
	rock.SetY(0);
	rock.SetScale(1.f);

	// coin model
	grass.LoadModel("grass/grass.obj");
	grass.SetY(50);
	grass.SetScale(1.0f);

	// flowers model
	flowers.LoadModel("flowers/flowers2.obj");
	flowers.SetY(0);
	flowers.SetScale(900.f);

	// mushroom
	mushroom.LoadModel("mushroom/MushroomShiitake.obj");
	mushroom.SetScale(10.0f);
	mushroom.SetToFloor(0);
}

void Map::init()
{
	LoadData();
}

void Map::LoadData()
{
	modelList.clear();
	floor.SetSize(15000, 15000);

	// move gameworld so that it starts at 0,0 and extends in positive xz-direction
	//floor.SetPosition(floor.GetWidth() / 2, floor.GetDepth() / 2);

	fstream myfile;
	myfile.open("level3.txt", ios_base::in);
	int type, x, y, z, rot;
	bool neof; // not end of file
	do
	{
		myfile >> type >> x >> y >> z >> rot;

		neof = myfile.good();
		//cout << type << " " << x << " " << y << " " << z << " " << rot << endl;
		if (neof) 
		{
			if (type == 1)  
			{
				CModel* pWall = wallSegment.Clone(); // clone wall segment
				pWall->SetPosition((float)x, (float)y, (float)z);
				pWall->SetRotation(float(rot));
				pWall->SetToFloor(0);
				pWall->SetStatus(-1); // indicate the type of object
				mapCollision.push_back(pWall);
			}

			if (type == 2)  // grass
			{
				CModel* newGrass = grass.Clone();
				newGrass->SetPosition((float)x, (float)y, (float)z); 
				grass.SetRotation((float)rot + rand() % 180);
				newGrass->SetStatus(2); // indicate the type of object
				modelList.push_back(newGrass);
			}

			if (type == 3)   // tree
			{
				CModel* newTree = tree.Clone(); // clone wall segment
				newTree->SetPosition((float)x, (float)y, (float)z);
				newTree->SetRotation(float(rot) + rand() % 180);

				newTree->SetToFloor(0);
				newTree->SetScale(50.0f + rand() % 25);
				newTree->SetStatus(3); // indicate the type of object
				modelList.push_back(newTree);
			}

			if (type == 4) // rock
			{
				CModel* newRock = rock.Clone();
				newRock->SetPosition((float)x, (float)y - 100, (float)z);
				newRock->SetRotation(float(rot) + rand() % 180);
				newRock->SetStatus(4); // indicate the type of object
				collidingObjects.push_back(newRock);
			}

			if (type == 5) // flowers
			{
				CModel* newflowers = flowers.Clone();
				newflowers->SetPosition((float)x, (float)y, (float)z);
				newflowers->SetStatus(5); // indicate the type of object
				modelList.push_back(newflowers);
			}

			if (type == 6) // mushroom
			{
				CModel* newMushroom = mushroom.Clone();
				newMushroom->SetPosition((float)x, (float)y, (float)z);
				newMushroom->SetStatus(6); // indicate the type of object
				modelList.push_back(newMushroom);
			}

			if (type == 7) // collidingTree
			{
				CModel* newcollidingTree = collidingTree.Clone();
				newcollidingTree->SetPosition((float)x, (float)y, (float)z);
				newcollidingTree->SetScale(50.0f + rand() % 25);
				newcollidingTree->SetStatus(7); // indicate the type of object
				collidingObjects.push_back(newcollidingTree);
			}
 
		}
	} while (neof);
	myfile.close();
}

void Map::OnUpdate(Uint32 t)
{
	modelList.Update(t);
}

void Map::OnDraw(CGraphics* g) {}


void Map::OnRender3D(CGraphics* g)
{
	//mapCollision.Draw(g);
	collidingObjects.Draw(g);
	floor.Draw(g);
	modelList.Draw(g);
}
