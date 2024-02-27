#include "Game.h"
#include "headers/Map.h"

void Map::init()
{
	// floor texture
	floor.LoadTexture("Map/floor2.bmp");
	floor.SetTiling(true);

	// set size of the game world
	//floor.SetSize(9000, 9000);

	portal.LoadModel("portal/portal.obj");
	//portal.SetSize(1200, 1200, 1200);
	portal.SetScale(12.f);
	portal.SetPosition(400, 0, 300);
	portal.SetHealth(100);


	// wall model
	wallSegment.LoadModel("wall/wall2.obj");
	wallSegment.SetScale(4.0f);
	wallSegment.SetToFloor(0);

	//forest
	tree.LoadModel("Ogro/Ogro.obj");
	tree.SetScale(4.0f);
	tree.SetToFloor(0);

	// floor texture
	floor.LoadTexture("Map/floor2.bmp");
	floor.SetTiling(true);

	LoadData();
}

void Map::LoadData()
{
	// --- clear model lists ----
	modelList.clear();

	// set size of the game world (2000x2000)
	floor.SetSize(15000, 15000);

	// move gameworld so that it starts at 0,0 and extends in positive xz-direction
	floor.SetPosition(floor.GetWidth() / 2, floor.GetDepth() / 2);

	// declaring new file
	fstream myfile;
	// opening file for reading
	myfile.open("level3.txt", ios_base::in);

	int type, x, y, z, rot;
	// reading while the end of file has not been reached
	bool neof; // not end of file
	do
	{
		myfile >> type >> x >> y >> z >> rot;

		neof = myfile.good();
		//cout << type << " " << x << " " << y << " " << z << " " << rot << endl;
		if (neof) // if not end of file
		{
			if (type == 1)   // wall segments
			{
				CModel* pWall = wallSegment.Clone(); // clone wall segment
				pWall->SetPosition((float)x, (float)y, (float)z);
				pWall->SetRotation(float(rot));
				pWall->SetToFloor(0);
				pWall->SetStatus(1); // indicate the type of object
				modelList.push_back(pWall);
			}

			if (type == 3)   // wall segments
			{
				CModel* newTree = tree.Clone(); // clone wall segment
				newTree->SetPosition((float)x, (float)y, (float)z);
				newTree->SetRotation(float(rot));
				newTree->SetToFloor(0);
				newTree->SetStatus(1); // indicate the type of object
				modelList.push_back(newTree);
			}
 
		}
	} while (neof);
	myfile.close();
	cout << "Level data loaded" << endl;

}

void Map::OnUpdate(int t)
{
	modelList.Update(t);
}

void Map::OnDraw(CGraphics* g)
{

}


void Map::OnRender3D(CGraphics* g)
{
	if(portal.GetHealth() > 0) portal.Draw(g);
	floor.Draw(g);
	modelList.Draw(g);
}
