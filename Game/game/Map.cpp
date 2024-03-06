#include "Game.h"
#include "headers/Map.h"

void Map::init()
{
	font.LoadDefault();

	// floor texture
	floor.LoadTexture("Map/floor2.bmp");
	floor.SetTiling(true);

	//PORTAL
	portal.LoadModel("portal/portal.obj");
	portal.SetScale(12.f);
	portal.SetPosition(400, 0, 300);
	portal.SetHealth(100);

	portalPartOne.LoadModel("portal/portalPart.obj");
	portalPartOne.SetScale(8.f);
	portalPartOne.SetPosition(600, 0, 300);

	portalPartTwo.LoadModel("portal/portalPart.obj");
	portalPartTwo.SetScale(8.f);
	portalPartTwo.SetPosition(200, 0, 300);
	
	portalPartThree.LoadModel("portal/portalPart.obj");
	portalPartThree.SetScale(8.f);
	portalPartThree.SetPosition(400, 0, 500);

	portalPartFour.LoadModel("portal/portalPart.obj");
	portalPartFour.SetScale(8.f);
	portalPartFour.SetPosition(400, 0, 100);

	TotaltimeForPortalToCharg = 60000 * 1;
	currentportalChargingTime = 0;


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

			if (type == 3)  
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
}

void Map::OnUpdate(int t)
{
	modelList.Update(t);

	currentportalChargingTime = t;
 
}

void Map::OnDraw(CGraphics* g)
{

	//font.DrawText(200, 200, "PortalChargeTime", CColor::White(), 18);
	//font.DrawNumber(300,300, currentportalChargingTime, CColor::White(), 18);

}


void Map::OnRender3D(CGraphics* g)
{
	portal.Draw(g);

	portalPartOne.Draw(g);
	portalPartTwo.Draw(g);
	portalPartThree.Draw(g);
	portalPartFour.Draw(g);

	floor.Draw(g);
	modelList.Draw(g);
 
}
