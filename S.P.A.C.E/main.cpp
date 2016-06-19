// S.P.A.C.E.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "stb_image.h"
#include "skybox.h"
#include "room.h"
#include "particle.h"
#include "time.h"
#include "objectBounds.h"
#include "roomdetails.h"
#include "console_object.h"
#include "text2D.h"
#include "AABBbounds.h"
#include <GL\glew.h>
#include <string>
#include <vector>
#include <deque>
#include <stack>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Windows.h>
#include <mmsystem.h>

using namespace std;
#undef main

#define WIDTH 800
#define HEIGHT 600
#define FPS 120


int main(int argc, char* argv[])
{
	cout << "Press TAB for Menu." << endl;

	//Setup initial display
	Display display(WIDTH, HEIGHT, "S.P.A.C.E");

#pragma region Setup Meshes

	//Setup Meshes to be used
	Mesh sphere("./res/Models/sphere.obj");
	Mesh lamp("./res/cube.obj");
	Mesh console("./res/Models/console.obj");
	Mesh consoleScreen("./res/Models/consolescreen.obj");
	Mesh disc("./res/Models/disc.obj");
	Mesh stand("./res/Models/stand.obj");
	Mesh soundTile("./res/Models/tile.obj");
	Mesh cubeFall("./res/cube.obj");

#pragma endregion

#pragma region Setup Shaders

	//Setup Shaders to be used
	Shader shaderC("./res/basicShader");
	Shader skyboxShader("./res/skyboxShader");
	Shader pointLight("./res/pointLight");
	Shader discShader("./res/disc");
	Shader standShader("./res/stand");

#pragma endregion

#pragma region Setup Rooms

	//Setup all room models
	Room roomStraight("./res/Models/room_straight.obj");
	Room roomLeft("./res/Models/room_left.obj");
	Room roomRight("./res/Models/room_right.obj");
	Room roomUp("./res/Models/room_up.obj");
	Room roomDown("./res/Models/room_down.obj");

#pragma endregion

#pragma region Transformation Matrices

	//Created transformation matrices for specific objects
	Transform transformS,//Sphere
		transformL,//Light
		transformStand,//Stand
		transformDisc,//Discs 
		transformTile,//Sound tile
		transformP;//Particle system

#pragma endregion

#pragma region Towers of Hanoi Setup

	// Disc position vectors
	glm::vec3 discXLPos(-3.0f, 0.2f, 0.0f);
	glm::vec3 discBigPos(-3.0f, 0.2f, 0.0f);
	glm::vec3 discMedPos(-3.0f, 0.5f, 0.0f);
	glm::vec3 discSmallPos(-3.0f, 0.8f, 0.0f);
	glm::vec3 discXSPos(-3.0f, 1.0f, 0.0f);

	// Stacks for each stand
	std::stack<int> st1;
	std::stack<int> st2;
	std::stack<int> st3;

	int discCount = 3;
	int turnCount = 0;

	// Start game with 3 rings on stand 1
	st1.push(4);
	st1.push(3);
	st1.push(2);

	int selection = 0;
	bool finished = true;

#pragma endregion

#pragma region int Setups

	int width = WIDTH;
	int height = HEIGHT;
	int nbFrames = 0;
	int turnCheck = 0;
	int nextRoom = 1;
	int currentPos = 0;
	int mapPos;
	GLint discLoc = glGetUniformLocation(discShader.GetProg(), "tint");
	GLint newLoc = glGetUniformLocation(discShader.GetProg(), "tint");

#pragma endregion

#pragma region float Setups

	float lastUpdate = 0.0f;
	float deltaTime = 0.0f;
	float currentTime = 0.0f;
	float counter = 0.0f;
	float doorMovement = 0.0f;
	float doorSpeed = 1.8f;
	float roomHalfLength = 7.5f;
	float roomHalfWidth = 5.0f;
	float roomHeight = 0.0f;
	float cameraSpeed = 0.05f;
	float autoMoveX = 0.0f;
	float autoMoveY = 1.5f;
	float autoMoveZ = 4.0f;
	GLfloat yaw = 90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
	GLfloat pitch = 0.0f;
	GLfloat lastX = WIDTH / 2.0;
	GLfloat lastY = HEIGHT / 2.0;
	//Interactable Sphere
	GLfloat sphereTouchx = 50, sphereTouchy = 1.5, sphereTouchz = 10;

#pragma endregion

#pragma region bool Setups

	bool doorsOpen = false;
	bool menu = false;
	bool generatedStart = false;
	bool restart = false;
	bool hanoiPresent = false;
	bool mapView = false;
	bool soundBackground = false;
	bool soundTile1 = false;
	bool soundTile2 = false;
	bool demoMode = false;
	bool autoBackward = false;
	GLboolean doorOpen = false, first = false;

#pragma endregion

#pragma region char Setups

	char curDir = 'f';
	char prevDir = curDir;

#pragma endregion

#pragma region double Setups

	double lastTime = SDL_GetTicks();

#pragma endregion

	//Setup console object for rooms that contain it
	ConsoleObject co;

	//Create camera and set it's position
	Camera camera(glm::vec3(0, 1.5, 4), 70.0f, (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);

	Skybox skybox;

	// Cubemap (Skybox)
	vector<const GLchar*> skyfaces;
	skyfaces.push_back("./res/skybox/purplenebula_rt.tga");
	skyfaces.push_back("./res/skybox/purplenebula_lf.tga");
	skyfaces.push_back("./res/skybox/purplenebula_up.tga");
	skyfaces.push_back("./res/skybox/purplenebula_dn.tga");
	skyfaces.push_back("./res/skybox/purplenebula_bk.tga");
	skyfaces.push_back("./res/skybox/purplenebula_ft.tga");
	Texture skyboxTex(skyfaces);

	//Setup Textures
	Texture rockTex("./res/textures/rock.jpg");
	Texture consoleTex("./res/textures/console.jpg");
	Texture screenTex("./res/textures/screen.jpg");
	Texture standTexture("./res/textures/wood.jpg");
	Texture tileTexture("./res/textures/musicTile.jpg");
	Texture earthTex("./res/textures/earth.jpg");
	Texture jupiterTex("./res/textures/jupitermap.jpg");
	Texture saturnTex("./res/textures/saturnmap.jpg");

	// Create camera transformations
	glm::mat4 view = camera.GetViewProjection();
	glm::mat4 projection = glm::perspective(70.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::vec3 lightPos(3.0f, 5.0f, 3.5f);

	


	//Create Object Bounds
	ObjectBounds cameraBound, doorBound;
	
	AABBbounds squareBound, floorbound;
	GLfloat cube1x = (((float)rand() / (float)(RAND_MAX)) * 8) - 4;
	GLfloat cube2x = (((float)rand() / (float)(RAND_MAX)) * 8) - 4;
	GLfloat cube3x = (((float)rand() / (float)(RAND_MAX)) * 8) - 4;
	GLfloat cube1z = (((float)rand() / (float)(RAND_MAX)) * 8) - 4;
	GLfloat cube2z = (((float)rand() / (float)(RAND_MAX)) * 8) - 4;
	GLfloat cube3z = (((float)rand() / (float)(RAND_MAX)) * 8) - 4;
	GLfloat cubey = 3.5;
	GLboolean cubeHitFloor = false;

	

	Particle p2[250];

	//Setup deque to hold all details of rooms that are created.
	deque<RoomDetails> roomdetails;
	//Initial room always the same
	roomdetails.push_back(RoomDetails(glm::vec3(0,0,0), curDir, turnCheck, false));

	initText2D();

	//Holds the coordinates of the next room generated.
	glm::vec3 nextRoomCoords;

	
	
	while (!display.IsClosed())
	{
		//   // Measure speed
		//	  double currentTime = SDL_GetTicks();
		//      nbFrames++;
		//      if (currentTime - lastTime >= 1000.0) {
		//	          // prints updates per second
		//		  cout << double(nbFrames) << endl;
		//	          nbFrames = 0;
		//			  lastTime += 1000.0;
		//			 
		//}

		if (!soundBackground) {
			
			if (PlaySound(TEXT("./res/sounds/background.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC | SND_NOSTOP)) {
				soundBackground = true;
			}
		}

//Reset method when 'r' is pressed.
#pragma region Reset
		if (restart == true) {
				cout << "Engine Reset!" << endl;
				camera.GetCameraPos() = glm::vec3(0, 1.5, 4);
				nextRoom = 1;
				currentPos = 0;
				doorOpen = false;
				counter = 0.0f;
				yaw = 90.0f;
				pitch = 0.0f;
				lastX = WIDTH / 2.0;
				lastY = HEIGHT / 2.0;
				roomdetails.clear();
				roomdetails.push_back(RoomDetails(glm::vec3(0, 0, 0), curDir, turnCheck, false));
				while (!st1.empty()) {
					st1.pop();
				}
				while (!st2.empty()) {
					st2.pop();
				}
				while (!st3.empty()) {
					st3.pop();
				}
				switch (discCount) {
				case 3: st1.push(4); st1.push(3); st1.push(2); break;
				case 4: st1.push(4); st1.push(3); st1.push(2); st1.push(1); break;
				case 5: st1.push(5); st1.push(4); st1.push(3); st1.push(2); st1.push(1); break;
				}
				selection = 0;
				turnCheck = 0;
				hanoiPresent = false;
				curDir = 'f';
				prevDir = curDir;
				generatedStart = false;
				roomStraight.Reset();
				roomLeft.Reset();
				roomRight.Reset();
				float cameraSpeed = 0.08f;
				mapView = false;
				roomHeight = 0;
				soundTile1 = false;
				soundTile2 = false;
				float autoMoveX = 0.0f;
				float autoMoveY = 1.5f;
				float autoMoveZ = 4.0f;
				demoMode = false;
				autoBackward = false;
				restart = false;
			}
		
#pragma endregion 

#pragma region Light
		display.Clear(0.0f, 0.3f, 0.15f, 1.0f);
		///////////// Point Light /////////////////////
		lightPos.x = cosf(counter) * 1.5;
		lightPos.z = sinf(counter) * 1.5 + roomHalfLength;
		lightPos.y = roomdetails.at(currentPos).GetY() + 2.0f;
		pointLight.Bind();
		transformL.SetScale(glm::vec3(0.1, 0.1, 0.1));
		transformL.GetPos().y = lightPos.y;
		transformL.GetPos().x = lightPos.x;
		transformL.GetPos().z = lightPos.z;
		pointLight.Update(transformL, camera);
		lamp.Draw();
#pragma endregion


		///////////// Bounding Box Declarations /////////////////////


		if (first) {
			printf("%6.4lf \n", camera.GetCameraPos().x);
			printf("%6.4lf \n", camera.GetCameraPos().y);
			printf("%6.4lf \n", camera.GetCameraPos().z);
			first = false;
		}

		//door bound coordinates need added so that a check can be preformed
		//doorBound.SphereBound();

		///////////// Drawing Interactable Sphere /////////////////////
		shaderC.Bind();
		rockTex.Bind();
		transformS.GetPos().x = sphereTouchx;
		transformS.GetPos().y = sphereTouchy;
		transformS.GetPos().z = sphereTouchz;
		shaderC.Update(transformS, camera);
		sphere.Draw();

		

		squareBound.AABBBound(15, 0, 0, 30, 10, 10);
		squareBound.AABBBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z);

		floorbound.AABBBound(-1, -1, -1, 1, 0, 1);
		//if (floorbound.AABBBoundCheck(0, cubey, 0, 0, cubey, 0) == true) {
			//cubeHitFloor = true;
		//}

		if (cubey < 0.5) {
			cubeHitFloor = true;
		}

		cameraBound.SphereBound(sphereTouchx, sphereTouchy, sphereTouchz, 1);
		if (cameraBound.SphereBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 4)) {
			sphereTouchx = cameraBound.GetCentreX();
			sphereTouchy = cameraBound.GetCentreY();
			sphereTouchz = cameraBound.GetCentreZ();
		}

		//GLboolean Overlap = cameraBound.SphereBoundCheck(doorBound.GetCentreX, doorBound.GetCentreY, doorBound.GetCentreZ, doorBound.GetRadius);


#pragma region Find Current Position 


		//This set of conditions determines the current position of the user
		//currentPos is used for generating rooms ahead of you, but also user to check the details of the current room.
		if (generatedStart) {
			//If the user is not in the first room
			if (currentPos != 0) {
				//Check the deque of roomdetails to find a region where the camera matches and thus finding its position.
				for (int j = currentPos - 1; j < roomdetails.size() - 1; j++) {
					//If the room that is being checked is a corner
					if (roomdetails.at(j).isCorner() == true) {
						switch (roomdetails.at(j).GetDirection()) {
						//Find orientation of corner which affects the area being check for the camera position.
						case 'f':
							if ((roomdetails.at(j).GetZ() <= camera.GetCameraPos().z  && camera.GetCameraPos().z <= roomdetails.at(j + 1).GetZ() + roomHalfLength)) {
								currentPos = j;
							}
							break;

						case 'l':
							if (roomdetails.at(j).GetX() <= camera.GetCameraPos().x && camera.GetCameraPos().x <= roomdetails.at(j + 1).GetX() + roomHalfLength) {
								currentPos = j;
							}
							break;

						case 'r':
							if (roomdetails.at(j).GetX() >= camera.GetCameraPos().x && camera.GetCameraPos().x >= roomdetails.at(j + 1).GetX() - roomHalfLength) {
								currentPos = j;
							}
							break;
							doorMovement = 0.0f;
						}
					}
					//If it's not a corner
					else {
						//Find orientation of corner which affects the area being check for the camera position.
						switch (roomdetails.at(j).GetDirection()) {

						case 'f':
							if (roomdetails.at(j).GetZ() <= camera.GetCameraPos().z && camera.GetCameraPos().z < roomdetails.at(j + 1).GetZ()) {

								currentPos = j;

							}
							break;

						case 'r':
							if (roomdetails.at(j).GetX() >= camera.GetCameraPos().x && camera.GetCameraPos().x > roomdetails.at(j + 1).GetX()) {
								currentPos = j;

							}
							break;

						case 'l':
							if (roomdetails.at(j).GetX() <= camera.GetCameraPos().x && camera.GetCameraPos().x < roomdetails.at(j + 1).GetX()) {
								currentPos = j;

							}
							break;

						}
					}
				}

			}
			//If the above conditions are not met, then the currentPos is 0 or 1.
			else {
				if (roomdetails.at(0).GetZ() <= camera.GetCameraPos().z && camera.GetCameraPos().z <= roomdetails.at(1).GetZ()) {
					currentPos = 0;
				}
				else {
					currentPos = 1;
				}
			}
		}


#pragma endregion

#pragma region Room Creation

		//If the user is not in map view
		if (!mapView) {
			//Find the current direction in which rooms should be spawning.
			switch (curDir) {
			//If its forward
			case 'f':
				//If camera position in the the last room, make a new room ahead
				if (camera.GetCameraPos().z >= roomdetails.at(roomdetails.size() - 1).GetZ()) {
					//If the last room is a corner
					if (roomdetails.at(nextRoom - 1).isCorner() && generatedStart) {
						//Find the direction that corner turns
						switch (prevDir) {
							//If left, add new room to roomDetails deque using coords of current last room
						case 'l':
							nextRoomCoords = glm::vec3(roomdetails.at(roomdetails.size() - 1).GetX() + roomHalfLength, roomHeight, roomdetails.at(roomdetails.size() - 1).GetZ() + 5);
							roomdetails.push_back(RoomDetails(nextRoomCoords, curDir, turnCheck, roomdetails.at(nextRoom - 1).isCorner()));
							if (roomdetails.at(nextRoom).GetMeshNum() == 4) {
								prevDir = curDir;
								//Change current direction
								curDir = 'r';
								turnCheck++;
							}
							break;
							//If left, add new room to roomDetails deque using coords of current last room
						case 'r':
							nextRoomCoords = glm::vec3(roomdetails.at(roomdetails.size() - 1).GetX() - roomHalfLength, roomHeight, roomdetails.at(roomdetails.size() - 1).GetZ() + 5);
							roomdetails.push_back(RoomDetails(nextRoomCoords, curDir, turnCheck, roomdetails.at(nextRoom - 1).isCorner()));
							if (roomdetails.at(nextRoom).GetMeshNum() == 9) {
								prevDir = curDir;
								//Change current direction
								curDir = 'l';
								turnCheck--;
							}
							break;
						}
					}
					//If not a corner
					else {
						//Add new room to roomDetails deque using coords of current last room
						nextRoomCoords = glm::vec3(roomdetails.at(roomdetails.size() - 1).GetX(), roomHeight, roomdetails.at(roomdetails.size() - 1).GetZ() + 15.0f);
						roomdetails.push_back(RoomDetails(nextRoomCoords, curDir, turnCheck, roomdetails.at(nextRoom - 1).isCorner()));
						//If meshNum is 4 (right corner)
						if (roomdetails.at(nextRoom).GetMeshNum() == 4) {
							prevDir = curDir;
							//Change current direction
							curDir = 'r';
							turnCheck++;
						}
						//If meshNum is 4 (left corner)
						if (roomdetails.at(nextRoom).GetMeshNum() == 9) {
							prevDir = curDir;
							//Change current direction
							curDir = 'l';
							turnCheck--;
						}

					}
					//If MeshNum is 2 (UpRoom), change current height at which rooms spawn
					if (roomdetails.at(nextRoom).GetMeshNum() == 2) {
						roomHeight += 5.68f;
					}
					//If MeshNum is 14 (DownRoom), change current height at which rooms spawn
					if (roomdetails.at(nextRoom).GetMeshNum() == 14) {
						roomHeight -= 5.68f;
					}
					//Increase the index of the nextroom to spawn
					nextRoom++;
					//If the first two rooms have been created
					if (roomdetails.size() == 2) {
						generatedStart = true;
					}
				}
				break;

			//If its Left
			case 'l':
				//If camera position in the the last room, make a new room ahead
				if (camera.GetCameraPos().x >= roomdetails.at(roomdetails.size() - 1).GetX()) {
					//If the last room is a corner
					if (roomdetails.at(nextRoom - 1).isCorner()) {
						//Add new room to roomDetails deque using coords of current last room
						nextRoomCoords = glm::vec3(roomdetails.at(roomdetails.size() - 1).GetX() + 5, roomHeight, roomdetails.at(roomdetails.size() - 1).GetZ() + roomHalfLength);
						roomdetails.push_back(RoomDetails(nextRoomCoords, curDir, turnCheck, roomdetails.at(nextRoom - 1).isCorner()));
						if (roomdetails.at(nextRoom).GetMeshNum() == 4) {
							prevDir = curDir;
							//Change current direction
							curDir = 'f';
							turnCheck++;
						}
					}
					else {
						//Add new room to roomDetails deque using coords of current last room
						nextRoomCoords = glm::vec3(roomdetails.at(roomdetails.size() - 1).GetX() + 15, roomHeight, roomdetails.at(roomdetails.size() - 1).GetZ());
						roomdetails.push_back(RoomDetails(nextRoomCoords, curDir, turnCheck, roomdetails.at(nextRoom - 1).isCorner()));
						if (roomdetails.at(nextRoom).GetMeshNum() == 4) {
							prevDir = curDir;
							//Change current direction
							curDir = 'f';
							turnCheck++;
						}
					}
					//If MeshNum is 2 (UpRoom), change current height at which rooms spawn
					if (roomdetails.at(nextRoom).GetMeshNum() == 2) {
						roomHeight += 5.68f;
					}
					//If MeshNum is 14 (DownRoom), change current height at which rooms spawn
					if (roomdetails.at(nextRoom).GetMeshNum() == 14) {
						roomHeight -= 5.68f;
					}
					//Increase the index of the nextroom to spawn
					nextRoom++;
					//If the first two rooms have been created
					if (roomdetails.size() == 2) {
						generatedStart = true;
					}
				}
				break;

			//If its Right
			case 'r':
				//If camera position in the the last room, make a new room ahead
				if (camera.GetCameraPos().x <= roomdetails.at(roomdetails.size() - 1).GetX()) {
					//If the last room is a corner
					if (roomdetails.at(nextRoom - 1).isCorner()) {
						//Add new room to roomDetails deque using coords of current last room
						nextRoomCoords = glm::vec3(roomdetails.at(roomdetails.size() - 1).GetX() - 5, roomHeight, roomdetails.at(roomdetails.size() - 1).GetZ() + roomHalfLength);
						roomdetails.push_back(RoomDetails(nextRoomCoords, curDir, turnCheck, roomdetails.at(nextRoom - 1).isCorner()));
						if (roomdetails.at(nextRoom).GetMeshNum() == 9) {
							prevDir = curDir;
							//Change current direction
							curDir = 'f';
							turnCheck--;
						}
					}
					else {
						//Add new room to roomDetails deque using coords of current last room
						nextRoomCoords = glm::vec3(roomdetails.at(roomdetails.size() - 1).GetX() - 15, roomHeight, roomdetails.at(roomdetails.size() - 1).GetZ());
						roomdetails.push_back(RoomDetails(nextRoomCoords, curDir, turnCheck, roomdetails.at(nextRoom - 1).isCorner()));
						if (roomdetails.at(nextRoom).GetMeshNum() == 9) {
							prevDir = curDir;
							//Change current direction
							curDir = 'f';
							turnCheck--;
						}
					}
					//If MeshNum is 2 (UpRoom), change current height at which rooms spawn
					if (roomdetails.at(nextRoom).GetMeshNum() == 2) {
						roomHeight += 5.68f;
					}
					//If MeshNum is 14 (DownRoom), change current height at which rooms spawn
					if (roomdetails.at(nextRoom).GetMeshNum() == 14) {
						roomHeight -= 5.68f;
					}
					//Increase the index of the nextroom to spawn
					nextRoom++;
					//If the first two rooms have been created
					if (roomdetails.size() == 2) {
						generatedStart = true;
					}
				}
				break;

			}
		}
#pragma endregion

#pragma region Drawing Room

		//If the user is in map view
		if (mapView) {
			//For all roomDetails
			for (int i = 0; i < roomdetails.size(); i++) {
				if (i <= 0) {
					roomStraight.GetDoor() = true;
					roomStraight.SetPos(roomdetails.at(0).GetPos());
					roomStraight.Reset();
					if (i == mapPos) {
						roomStraight.Draw(lightPos, camera, -1);
					}
					else {
						roomStraight.Draw(lightPos, camera);
					}
				}
				else {

					roomdetails.at(currentPos + 1).OpenDoor();
					//Finds orientation of room and rotates accordingly
					switch (roomdetails.at(i).GetDirection()) {
					case 'l':

						roomRight.SetRot(-4.71239);
						roomStraight.SetRot(1.5708);
						roomUp.SetRot(1.5708);
						roomDown.SetRot(1.5708);
						break;
					case 'r':
						roomLeft.SetRot(4.71239);
						roomStraight.SetRot(-1.5708);
						roomUp.SetRot(-1.5708);
						roomDown.SetRot(-1.5708);
						break;
					case 'f':
						roomStraight.Reset();
						roomLeft.Reset();
						roomRight.Reset();
						roomUp.Reset();
						roomDown.Reset();
						break;
					}

					if (roomdetails.size() > 1) {
						//Switch statement for find which mesh to draw based on roomDetails meshNum
						switch (roomdetails.at(i).GetMeshNum()) {
						case 4:
							roomRight.SetPos(roomdetails.at(i).GetPos());
							switch (roomdetails.at(i).GetDirection()) {
							case 'l':
								roomRight.SetDoorPosz(roomdetails.at(i).GetZ() + 3);
								break;
							case 'f':
								roomRight.SetDoorPosx(roomdetails.at(i).GetX() + 3);
								break;
							}
							if (i == mapPos) {
								roomRight.Draw(lightPos, camera, -1);
							}
							else {
								roomRight.Draw(lightPos, camera);
							}
							break;

						case 9:
							roomLeft.SetPos(roomdetails.at(i).GetPos());
							switch (roomdetails.at(i).GetDirection()) {
							case 'r':
								roomLeft.SetDoorPosz(roomdetails.at(i).GetZ() + 3);
								break;
							case 'f':
								roomLeft.SetDoorPosx(roomdetails.at(i).GetX() + 3);
								break;
							}
							if (i == mapPos) {
								roomLeft.Draw(lightPos, camera, -1);
							}
							else {
								roomLeft.Draw(lightPos, camera);
							}
							break;

						case 2:
							roomUp.SetPos(roomdetails.at(i).GetPos());
							switch (roomdetails.at(i).GetDirection()) {
							case 'l':
								roomUp.SetDoorPosz(roomdetails.at(i).GetZ() + 3);
								break;
							case 'r':
								roomUp.SetDoorPosz(roomdetails.at(i).GetZ() + 3);
								break;
							case 'f':
								roomUp.SetDoorPosx(roomdetails.at(i).GetX() + 3);
								break;
							}
							if (i == mapPos) {
								roomUp.Draw(lightPos, camera, -1);
							}
							else {
								roomUp.Draw(lightPos, camera);
							}						
							break;

						case 14:
							roomDown.SetPos(roomdetails.at(i).GetPos());
							switch (roomdetails.at(i).GetDirection()) {
							case 'l':
								roomDown.SetDoorPosz(roomdetails.at(i).GetZ() + 3);
								break;
							case 'r':
								roomDown.SetDoorPosz(roomdetails.at(i).GetZ() + 3);
								break;
							case 'f':
								roomDown.SetDoorPosx(roomdetails.at(i).GetX() + 3);
								break;
							}
							if (i == mapPos) {
								roomDown.Draw(lightPos, camera, -1);
							}
							else {
								roomDown.Draw(lightPos, camera);
							}
							break;

						default:
							roomStraight.SetPos(roomdetails.at(i).GetPos());
							switch (roomdetails.at(i).GetDirection()) {
							case 'l':
								roomStraight.SetDoorPosz(roomdetails.at(i).GetZ() + 3);
								break;
							case 'r':
								roomStraight.SetDoorPosz(roomdetails.at(i).GetZ() + 3);
								break;
							case 'f':
								roomStraight.SetDoorPosx(roomdetails.at(i).GetX() + 3);
								break;
							}
							if (i == mapPos) {
								roomStraight.Draw(lightPos, camera, -1);						
							}
							else {
								roomStraight.Draw(lightPos, camera);
							}
							break;
						}
					}
				}
			}
		}

		//If not in mapview
		else {

			//Keep door movement clamped between two values
			if (doorMovement > 2.0f)
			{
				doorMovement = 2.0f;
			}
			if (doorMovement < 0.0f)
			{
				doorMovement = 0.0f;
			}

			//Only draws 3 rooms
			for (int i = (currentPos - 1); i < (currentPos + 2); i++) {
			
				if (i <= 0) {
					roomStraight.GetDoor() = true;
					roomStraight.SetPos(roomdetails.at(0).GetPos());
					roomStraight.Draw(lightPos, camera);
					
				}
				else {

					roomdetails.at(currentPos + 1).OpenDoor();
					//Finds orientation of room and rotates accordingly
					switch (roomdetails.at(i).GetDirection()) {
					case 'l':

						roomRight.SetRot(-4.71239);
						roomStraight.SetRot(1.5708);
						roomUp.SetRot(1.5708);
						roomDown.SetRot(1.5708);
						break;
					case 'r':
						roomLeft.SetRot(4.71239);
						roomStraight.SetRot(-1.5708);
						roomUp.SetRot(-1.5708);
						roomDown.SetRot(-1.5708);
						break;
					case 'f':
						roomStraight.Reset();
						roomLeft.Reset();
						roomRight.Reset();
						roomUp.Reset();
						roomDown.Reset();
						break;
					}
				

/*
This block of code detects collisions between the camera's position and a spherical bound positioned at the entrance and exit 
of the room the camera is currently inside. When a collision is detected the door components of the room objects are moved to the 
open position and when no collision is detected the door mesh moves back to its starting position = the current room position.
*/
						if (roomdetails.size() > 1) {
							//Initialise the spherical bound and position it at the next room doorway
							doorBound.SphereBound(roomdetails.at(currentPos + 1).GetX(), roomdetails.at(currentPos + 1).GetY(), roomdetails.at(currentPos + 1).GetZ(), 3.0);
							//Switch statement for find which mesh to draw based on roomDetails meshNum
							switch (roomdetails.at(i).GetMeshNum()) {
							//Right room door animation for each orientation
							case 4:
								roomRight.SetPos(roomdetails.at(i).GetPos());
								switch (roomdetails.at(i).GetDirection()) {
								case 'l':
									if (doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1))
									{
										if ((doorMovement <= 2.0 / doorSpeed))
										{
											doorMovement += 0.01f;
											roomRight.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomRight.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z - 2.0f);
										}

									}
									if (!doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1) && doorMovement != 0)
									{
										if ((doorMovement >= 0.0f))
										{
											doorMovement -= 0.01f;
											roomRight.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomRight.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z);
											doorMovement = 0.0f;
										}
									}
									break;
								case 'f':

									if (doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1))
									{
										if (doorMovement <= 2.0f / doorSpeed)
										{
											doorMovement += 0.01f;
											roomRight.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomRight.SetDoorPosx(roomdetails.at(currentPos + 1).GetPos().x + 2.0f);
										}

									}
									if (!doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1) && doorMovement != 0)
									{
										if ((doorMovement >= 0.0f))
										{
											doorMovement -= 0.01f;
											roomRight.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomRight.SetDoorPosx(roomdetails.at(currentPos + 1).GetPos().x);
											doorMovement = 0.0f;
										}
									}
									break;
								}
								roomRight.Draw(lightPos, camera);
								break;
							//Left room door animation for each orientation
							case 9:
								roomLeft.SetPos(roomdetails.at(i).GetPos());
								switch (roomdetails.at(i).GetDirection()) {
								case 'r':
									if (doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1))
									{
										if (doorMovement <= 2.0f / doorSpeed)
										{
											doorMovement += 0.01f;
											roomLeft.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomLeft.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z + 2.0f);
										}

									}
									if (!doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1) && doorMovement != 0)
									{
										if ((doorMovement >= 0.0f))
										{
											doorMovement -= 0.01f;
											roomLeft.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomLeft.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z);
											doorMovement = 0.0f;

										}
									}
									break;
								case 'f':
									if (doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1))
									{
										if (doorMovement <= 2.0f / doorSpeed)
										{
											doorMovement += 0.01f;
											roomLeft.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomLeft.SetDoorPosx(roomdetails.at(currentPos + 1).GetPos().x + 2.0f);
										}

									}
									if (!doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1) && doorMovement != 0)
									{
										if ((doorMovement >= 0.0f))
										{
											doorMovement -= 0.01f;
											roomLeft.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomLeft.SetDoorPosx(roomdetails.at(currentPos + 1).GetPos().x);
											doorMovement = 0.0f;
										}
									}
									break;
								}
								roomLeft.Draw(lightPos, camera);
								break;
							//Upwards room door animation for each orientation
							case 2:
								roomUp.SetPos(roomdetails.at(i).GetPos());
								switch (roomdetails.at(i).GetDirection()) {
								case 'l':
									if (doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1))
									{
										if ((doorMovement <= 2.0 / doorSpeed))
										{
											doorMovement += 0.01f;
											roomUp.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomUp.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z - 2.0f);

										}

									}
									if (!doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1) && doorMovement != 0)
									{
										if ((doorMovement >= 0.0f))
										{
											doorMovement -= 0.01f;
											roomUp.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomUp.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z);
											doorMovement = 0.0f;
										}
									}
									break;
								case 'r':
									if (doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1))
									{
										if (doorMovement <= 2.0f / doorSpeed)
										{
											doorMovement += 0.01f;
											roomUp.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomUp.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z + 2.0f);

										}

									}
									if (!doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1) && doorMovement != 0)
									{
										if ((doorMovement >= 0.0f))
										{
											doorMovement -= 0.01f;
											roomUp.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomUp.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z);
											doorMovement = 0.0f;

										}
									}
									break;
								case 'f':
									if (doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1))
									{
										if (doorMovement <= 2.0f / doorSpeed)
										{
											doorMovement += 0.01f;
											roomUp.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomUp.SetDoorPosx(roomdetails.at(currentPos + 1).GetPos().x + 2.0f);
										}

									}
									if (!doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1) && doorMovement != 0)
									{
										if ((doorMovement >= 0.0f))
										{
											doorMovement -= 0.01f;
											roomUp.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomUp.SetDoorPosx(roomdetails.at(currentPos + 1).GetPos().x);
											doorMovement = 0.0f;
										}
									}
									break;
								}
								roomUp.Draw(lightPos, camera);
								break;
							//downwards room door animation for each orientation
							case 14:
								roomDown.SetPos(roomdetails.at(i).GetPos());
								switch (roomdetails.at(i).GetDirection()) {
								case 'l':
									if (doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1))
									{
										if ((doorMovement <= 2.0 / doorSpeed))
										{
											doorMovement += 0.01f;
											roomDown.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomDown.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z - 2.0f);

										}

									}
									if (!doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1) && doorMovement != 0)
									{
										if ((doorMovement >= 0.0f))
										{
											doorMovement -= 0.01f;
											roomDown.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomDown.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z);
											doorMovement = 0.0f;
										}
									}
									break;
								case 'r':
									if (doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1))
									{
										if (doorMovement <= 2.0f / doorSpeed)
										{
											doorMovement += 0.01f;
											roomDown.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomDown.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z + 2.0f);
										}

									}
									if (!doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1) && doorMovement != 0)
									{
										if ((doorMovement >= 0.0f))
										{
											doorMovement -= 0.01f;
											roomDown.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomDown.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z);
											doorMovement = 0.0f;

										}
									}
									break;
								case 'f':
									if (doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1))
									{
										if (doorMovement <= 2.0f / doorSpeed)
										{
											doorMovement += 0.01f;
											roomDown.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomDown.SetDoorPosx(roomdetails.at(currentPos + 1).GetPos().x + 2.0f);
										}

									}
									if (!doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1) && doorMovement != 0)
									{
										if ((doorMovement >= 0.0f))
										{
											doorMovement -= 0.01f;
											roomDown.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomDown.SetDoorPosx(roomdetails.at(currentPos + 1).GetPos().x);
											doorMovement = 0.0f;
										}
									}
									break;
								}
								roomDown.Draw(lightPos, camera);
								break;
							//straight room door animation for each orientation
							default:
								roomStraight.SetPos(roomdetails.at(i).GetPos());
								switch (roomdetails.at(i).GetDirection()) {
								case 'l':
									if (doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1))
									{
										if ((doorMovement <= 2.0 / doorSpeed))
										{
											doorMovement += 0.01f;
											roomStraight.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomStraight.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z - 2.0f);

										}

									}
									if (!doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1) && doorMovement != 0)
									{
										if ((doorMovement >= 0.0f))
										{
											doorMovement -= 0.01f;
											roomStraight.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomStraight.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z);
											doorMovement = 0.0f;
										}
									}
									break;
								case 'r':
									if (doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1))
									{
										if (doorMovement <= 2.0f / doorSpeed)
										{
											doorMovement += 0.01f;
											roomStraight.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomStraight.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z + 2.0f);
										}

									}
									if (!doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1) && doorMovement != 0)
									{
										if ((doorMovement >= 0.0f))
										{
											doorMovement -= 0.01f;
											roomStraight.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomStraight.SetDoorPosz(roomdetails.at(currentPos + 1).GetPos().z);
											doorMovement = 0.0f;

										}
									}
									break;
								case 'f':
									if (doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1))
									{
										if (doorMovement <= 2.0f / doorSpeed)
										{
											doorMovement += 0.01f;
											roomStraight.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomStraight.SetDoorPosx(roomdetails.at(currentPos + 1).GetPos().x + 2.0f);
										}

									}
									if (!doorBound.DoorBoundCheck(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z, 1) && doorMovement != 0)
									{
										if ((doorMovement >= 0.0f))
										{
											doorMovement -= 0.01f;
											roomStraight.moveDoor(doorMovement, roomdetails.at(i).GetDirection(), doorSpeed);
										}
										else
										{
											roomStraight.SetDoorPosx(roomdetails.at(currentPos + 1).GetPos().x);
											doorMovement = 0.0f;
										}
									}
									break;
								}
								roomStraight.Draw(lightPos, camera);
								break;
							}
						
						
#pragma endregion			

#pragma region Object Drawing


						float xOffset = 0, yOffset = 0, zOffset = 0;
						//Sets offsets, so when objects are drawn they are in the middle of the rooms
						switch (roomdetails.at(i).GetDirection()) {
						case 'f':
							xOffset = roomdetails.at(i).GetX();
							yOffset = roomdetails.at(i).GetY();
							zOffset = roomdetails.at(i).GetZ() + roomHalfLength;
							break;
						case 'l':
							xOffset = roomdetails.at(i).GetX() + roomHalfLength;
							yOffset = roomdetails.at(i).GetY();
							zOffset = roomdetails.at(i).GetZ();
							break;
						case 'r':
							xOffset = roomdetails.at(i).GetX() - roomHalfLength;
							yOffset = roomdetails.at(i).GetY();
							zOffset = roomdetails.at(i).GetZ();
							break;
						}

						switch (roomdetails.at(i).GetContents()) {
						case 1:
							////////////// Meteor Sphere ///////////////////
							glEnable(GL_CULL_FACE);
							shaderC.Bind();
							saturnTex.Bind();
							transformS.GetPos().x = (sinf(counter) * 4) + xOffset;
							transformS.GetPos().z = (cosf(counter) * 4) + zOffset;
							transformS.GetPos().y = 1.5f + yOffset;
							transformS.GetScale().x = 1;
							transformS.GetScale().y = 1;
							transformS.GetScale().z = 1;
							shaderC.Update(transformS, camera);
							sphere.Draw();
							transformS.Reset();
							jupiterTex.Bind();
							transformS.GetPos().x = (cosf(counter) * 2) + xOffset;
							transformS.GetPos().z = (sinf(counter) * 2) + zOffset;
							transformS.GetPos().y = 1.5f + yOffset;
							transformS.GetScale().x = 0.66;
							transformS.GetScale().y = 0.66;
							transformS.GetScale().z = 0.66;
							shaderC.Update(transformS, camera);
							sphere.Draw();
							transformS.Reset();
							earthTex.Bind();
							transformS.GetPos().x = (sinf(counter) * 1.5) + xOffset;
							transformS.GetPos().z = (cosf(counter)) + zOffset;
							transformS.GetPos().y = 1.5f + yOffset;
							transformS.GetScale().x = 0.3;
							transformS.GetScale().y = 0.3;
							transformS.GetScale().z = 0.3;
							shaderC.Update(transformS, camera);
							sphere.Draw();
							transformS.Reset();
							break;

						case 2:

							switch (roomdetails.at(i).GetDirection()) {
							case 'f':
								co.GetTransformObjectC().ResetRot();
								break;
							case 'l':
								co.GetTransformObjectC().SetRot(glm::vec3(0, 1.57, 0));
								break;
							case 'r':
								co.GetTransformObjectC().SetRot(glm::vec3(0, -1.57, 0));
								break;
							}
							co.Draw(lightPos, camera);
							co.SetPosx(xOffset);
							co.SetPosy(yOffset);
							co.SetPosz(zOffset);
							break;

						case 3:
							transformTile.GetPos().x = xOffset;
							transformTile.GetPos().y = yOffset;
							transformTile.GetPos().z = zOffset; // Set stand position
							shaderC.Bind(); // Bind the stand shader
							tileTexture.Bind(); // Bind the stand texture
							if (roomdetails.at(i).GetDirection() == 'f') {
								
								transformTile.GetPos().z = zOffset - 3.0f;
								shaderC.Update(transformTile, camera);
								soundTile.Draw();
								if (!soundTile1) {
									if ((transformTile.GetPos().x - 1 <= camera.GetCameraPos().x && camera.GetCameraPos().x <= transformTile.GetPos().x + 1) && (transformTile.GetPos().z - 1 <= camera.GetCameraPos().z && camera.GetCameraPos().z <= transformTile.GetPos().z + 1)) {
										soundTile1 = true;
										soundTile2 = false;
										soundBackground = false;
										PlaySound(TEXT("./res/sounds/noteD.wav"), NULL, SND_FILENAME | SND_ASYNC);
										soundBackground = false;
									}
								}
								transformTile.GetPos().z = zOffset + 3.0f;
								shaderC.Update(transformTile, camera);
								soundTile.Draw();
								if (!soundTile2) {
									if ((transformTile.GetPos().x - 1 <= camera.GetCameraPos().x && camera.GetCameraPos().x <= transformTile.GetPos().x + 1) && (transformTile.GetPos().z - 1 <= camera.GetCameraPos().z && camera.GetCameraPos().z <= transformTile.GetPos().z + 1)) {
										soundTile1 = false;
										soundTile2 = true;
										PlaySound(TEXT("./res/sounds/noteF.wav"), NULL, SND_FILENAME | SND_ASYNC );
										soundBackground = false;
									}
								}
							}
							else {
								
								transformTile.GetPos().x = xOffset - 3.0f;
								shaderC.Update(transformTile, camera);
								soundTile.Draw();
								if (!soundTile1) {
									if ((transformTile.GetPos().x - 1 <= camera.GetCameraPos().x && camera.GetCameraPos().x <= transformTile.GetPos().x + 1) && (transformTile.GetPos().z - 1 <= camera.GetCameraPos().z && camera.GetCameraPos().z <= transformTile.GetPos().z + 1)) {
										soundTile1 = true;
										soundTile2 = false;
										soundBackground = false;
										PlaySound(TEXT("./res/sounds/noteD.wav"), NULL, SND_FILENAME | SND_ASYNC);
										soundBackground = false;
									}
								}
								transformTile.GetPos().x = xOffset + 3.0f;
								shaderC.Update(transformTile, camera);
								soundTile.Draw();
								if (!soundTile2) {
									if ((transformTile.GetPos().x - 1 <= camera.GetCameraPos().x && camera.GetCameraPos().x <= transformTile.GetPos().x + 1) && (transformTile.GetPos().z - 1 <= camera.GetCameraPos().z && camera.GetCameraPos().z <= transformTile.GetPos().z + 1)) {
										soundTile1 = false;
										soundTile2 = true;
										soundBackground = false;
										PlaySound(TEXT("./res/sounds/noteF.wav"), NULL, SND_FILENAME | SND_ASYNC);
										soundBackground = false;
									}
								}
							}
							break;

						case 4:
							// Draw stands
							hanoiPresent = true;
							transformStand.GetPos().x = xOffset;
							transformStand.GetPos().y = 0.05f + yOffset;
							transformStand.GetPos().z = zOffset; // Set stand position
							standShader.Bind(); // Bind the stand shader
							standTexture.Bind(); // Bind the stand texture
							//if (roomdetails.at(i).GetDirection() == 'f') {
								standShader.Update(transformStand, camera); // Update the stand shader for the stand and camera position
								stand.Draw();
								transformStand.GetPos().x = xOffset - 3.0f;
								standShader.Update(transformStand, camera);
								stand.Draw();
								transformStand.GetPos().x = xOffset + 3.0f;
								standShader.Update(transformStand, camera);
								stand.Draw();
							//}
							//else {
							//	standShader.Update(transformStand, camera); // Update the stand shader for the stand and camera position
							//	stand.Draw();
							//	transformStand.GetPos().z = zOffset - 3.0f;
							//	standShader.Update(transformStand, camera);
							//	stand.Draw();
							//	transformStand.GetPos().z = zOffset + 3.0f;
							//	standShader.Update(transformStand, camera);
							//	stand.Draw();
							//}


							// DRAW DISCS /////////////////////////////////
							discShader.Bind(); // Bind the disc shader for use
							//GLint discLoc = glGetUniformLocation(discShader.GetProg(), "tint");

							// BIG DISC
							glUniform4f(discLoc, 0.2f, 0.1f, 0.0f, 1.0f); // Apply disc tint
							transformDisc.SetScale(glm::vec3(0.8f, 0.8f, 0.8f)); // Scale disc
							transformDisc.SetPos(discBigPos); // Update disc position
							transformDisc.GetPos().x += xOffset;
							transformDisc.GetPos().y += yOffset;
							transformDisc.GetPos().z += zOffset;
							discShader.Update(transformDisc, camera); // Update disc shader for position and camera position
							disc.Draw(); // Draw the disc

										 // MEDIUM DISC
							glUniform4f(discLoc, 0.2f, 0.2f, 0.0f, 1.0f);
							transformDisc.SetScale(glm::vec3(0.6f, 0.8f, 0.6f));
							transformDisc.SetPos(discMedPos);
							transformDisc.GetPos().x += xOffset;
							transformDisc.GetPos().y += yOffset;
							transformDisc.GetPos().z += zOffset;
							discShader.Update(transformDisc, camera);
							disc.Draw();

							// SMALL DISC
							glUniform4f(discLoc, 0.0f, 0.2f, 0.0f, 1.0f);
							transformDisc.SetScale(glm::vec3(0.45f, 0.8f, 0.45f));
							transformDisc.SetPos(discSmallPos);
							transformDisc.GetPos().x += xOffset;
							transformDisc.GetPos().y += yOffset;
							transformDisc.GetPos().z += zOffset;
							discShader.Update(transformDisc, camera);
							disc.Draw();
							break;
                        

							//Particles
						case 5:
							discShader.Bind(); // Bind the disc shader for use

							for (int i = 0; i < 250; i++) {
								glUniform4f(newLoc, p2[i].ColourR(), p2[i].ColourG(), p2[i].ColourB(), p2[i].Lifespan());
								p2[i].Update();
								transformP.GetScale().x = 0.4;
								transformP.GetScale().y = 0.4;
								transformP.GetScale().z = 0.4;
								transformP.GetPos().x = p2[i].PositionX() + xOffset; //+ (i);
								transformP.GetPos().y = p2[i].PositionY() + yOffset;
								transformP.GetPos().z = p2[i].PositionZ() + (zOffset);
								transformP.GetRot().x = p2[i].RotationX();
								discShader.Update(transformP, camera);
								p2[i].Draw();

							}
							break;

						case 6:

							shaderC.Bind();
							
							transformS.Reset();
							transformS.GetPos().x = cube1x + xOffset;
							transformS.GetPos().y = cubey + yOffset;
							transformS.GetPos().z = cube1z + zOffset;
							shaderC.Update(transformS, camera);
							cubeFall.Draw();
							transformS.GetPos().x = cube2x + xOffset;							
							transformS.GetPos().z = cube2z + zOffset;
							shaderC.Update(transformS, camera);
							cubeFall.Draw();
							
							transformS.GetPos().x = cube3x + xOffset;							
							transformS.GetPos().z = cube3z + zOffset;
							shaderC.Update(transformS, camera);
							cubeFall.Draw();

							cubey -= 0.01;
							//printf("%6.4lf \n", cubey);
							if(cubeHitFloor == true){
								cubey = 3.5;
								cubeHitFloor = false;
							}
							break;


						}
					}
				}
				
			}
		}

#pragma endregion

#pragma region Hanoi

		if (hanoiPresent) {



			// POSITION DISCS /////////////////////////////


			std::stack<int> temp = st1; // Create temp stack to hold each stand's stack 

										// Move through stand 1's contents and find out which discs are currently in it and set their position based on their height
			while (!temp.empty()) {
				float height = 0.2f + (temp.size() - 1.0f) * 0.3f;
				switch (temp.top()) {
				case 1:	discXSPos = glm::vec3(3.0f, height, 0.0f); break;
				case 2: discSmallPos = glm::vec3(3.0f, height, 0.0f); break;
				case 3: discMedPos = glm::vec3(3.0f, height, 0.0f); break;
				case 4: discBigPos = glm::vec3(3.0f, height, 0.0f); break;
				case 5: discXLPos = glm::vec3(3.0f, height, 0.0f); break;
				}
				temp.pop();
			}

			temp = st2; // Set the temp stack to stand 2
						// Move through stack 2's contents and find out which discs are currently in it and set their position based on their height
			while (!temp.empty()) {
				float height = 0.2f + (temp.size() - 1.0f) * 0.3f;
				switch (temp.top()) {
				case 1:	discXSPos = glm::vec3(0.0f, height, 0.0f); break;
				case 2: discSmallPos = glm::vec3(0.0f, height, 0.0f); break;
				case 3: discMedPos = glm::vec3(0.0f, height, 0.0f); break;
				case 4: discBigPos = glm::vec3(0.0f, height, 0.0f); break;
				case 5: discXLPos = glm::vec3(0.0f, height, 0.0f); break;
				}
				temp.pop();
			}

			temp = st3; // Set the temp stack to stand 3
						// Move through stand 3's contents and find out which discs are currently in it and set their position based on their height
			while (!temp.empty()) {
				float height = 0.2f + (temp.size() - 1.0f) * 0.3f;
				switch (temp.top()) {
				case 1:	discXSPos = glm::vec3(-3.0f, height, 0.0f); break;
				case 2: discSmallPos = glm::vec3(-3.0f, height, 0.0f); break;
				case 3: discMedPos = glm::vec3(-3.0f, height, 0.0f); break;
				case 4: discBigPos = glm::vec3(-3.0f, height, 0.0f); break;
				case 5: discXLPos = glm::vec3(-3.0f, height, 0.0f); break;
				}
				temp.pop();
			}

			// DISC CONTROL ///////////////////////// 

			// Selecting the 1st disc stand
			if (display.Is1Pressed() && selection == 0 && !st1.empty()) {
				hanoiPresent = false;
				if (st2.empty() || st3.empty() || (st2.top() > st1.top() || st3.top() > st1.top())) {
					selection = 1;
				}
			}
			// Selecting the 2nd disc stand
			if (display.Is2Pressed() && selection == 0 && !st2.empty()) {
				hanoiPresent = false;
				if (st1.empty() || st3.empty() || (st1.top() > st2.top() || st3.top() > st2.top())) {
					selection = 2;
				}
			}
			// Selecting the 3rd disc stand
			if (display.Is3Pressed() && selection == 0 && !st3.empty()) {
				hanoiPresent = false;
				if (st1.empty() || st2.empty() || (st1.top() > st3.top() || st2.top() > st3.top())) {
					selection = 3;
				}
			}

			// Moving discs from stand 2 to 1
			if (display.Is1Pressed() && selection == 2) {
				hanoiPresent = false;
				if (st1.empty() || st2.top() < st1.top()) {
					st1.push(st2.top());
					st2.pop();
					selection = 4;
					turnCount++;
				}
			}
			// Moving discs from stand 3 to 1
			if (display.Is1Pressed() && selection == 3) {
				hanoiPresent = false;
				if (st1.empty() || st3.top() < st1.top()) {
					st1.push(st3.top());
					st3.pop();
					selection = 4;
					turnCount++;
				}
			}
			// Moving discs from stand 1 to 2
			if (display.Is2Pressed() && selection == 1) {
				hanoiPresent = false;
				if (st2.empty() || st1.top() < st2.top()) {
					st2.push(st1.top());
					st1.pop();
					selection = 4;
					turnCount++;
				}
			}
			// Moving discs from stand 3 to 2
			if (display.Is2Pressed() && selection == 3) {
				hanoiPresent = false;
				if (st2.empty() || st3.top() < st2.top()) {
					st2.push(st3.top());
					st3.pop();
					selection = 4;
					turnCount++;
				}
			}
			// Moving discs from stand 2 to 3
			if (display.Is3Pressed() && selection == 2) {
				hanoiPresent = false;
				if (st3.empty() || st2.top() < st3.top()) {
					st3.push(st2.top());
					st2.pop();
					selection = 4;
					turnCount++;
				}
			}
			// Moving discs from stand 1 to 3
			if (display.Is3Pressed() && selection == 1) {
				hanoiPresent = false;
				if (st3.empty() || st1.top() < st3.top()) {
					st3.push(st1.top());
					st1.pop();
					selection = 4;
					turnCount++;
				}
			}
			// Cancel the stand selection
			if ((selection == 4 && display.isNewInput()) || display.isCPressed()) {
				selection = 0;
				display.isCPressed() = false;
			}

		}
#pragma endregion



		////////////// Skybox ///////////////////////////
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);
		skyboxShader.Bind();
		view = glm::mat4(glm::mat3(camera.GetSkyboxProjection()));	// Remove any translation component of the view matrix
		skyboxTex.Bind();
		skybox.Draw(skyboxShader.GetProg(), view, projection);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);

		////////////// TEXT AND MENU ////////////////////////////

		char text[256];
		sprintf_s(text, "Current Room:%d", currentPos);
		printText2D(text, 1, 1, 20); // Print text (text, x position, y position, size)
		if (menu) {
			drawMenu(WIDTH / 4, HEIGHT / 4, 400);
			glDisable(GL_DEPTH_TEST);
			cleanupText2D();
			sprintf_s(text, "%dx%d", width, height);
			printText2D(text, 530, 185, 10);
			double camSpeed = cameraSpeed * 100;
			int camSpeedInt = int(camSpeed);
			cleanupText2D();
			sprintf_s(text, "%d", camSpeedInt);
			printText2D(text, 550, 155, 15);
			glEnable(GL_DEPTH_TEST);
		}

#pragma region Camera Control
		
		//If not in mapview and demo mode
		if (!mapView) {
			if (!demoMode) {
				//Stops camera leaving first room backwards
				if (camera.GetCameraPos().z > 0.5) {
					//Keyboard control to translate camera
					if (display.IsUpPressed())
						camera.MoveForward(cameraSpeed);

					if (display.IsDownPressed())
						camera.MoveBackward(cameraSpeed);

					if (display.IsLeftPressed())
						camera.MoveLeft(cameraSpeed);

					if (display.IsRightPressed())
						camera.MoveRight(cameraSpeed);
				}
				else {
					camera.GetCameraPos() = glm::vec3(0, 1.5, roomHalfLength);
				}
			}
		}

		//Changes camera speed
		if (display.IsPlusPressed() && !display.IsKeydown()) {
			if (cameraSpeed < 0.5) {
				cameraSpeed += 0.01f;
			}
			display.IsKeydown() = true;
		}

		//Changes camera speed
		if (display.IsMinusPressed() && !display.IsKeydown()) {
			if (cameraSpeed > 0.01) {
				cameraSpeed -= 0.01f;
			}
			if (cameraSpeed <= 0) {
				cameraSpeed = 0.01;
			}
			display.IsKeydown() = true;
		}

		//Restarts engine on pree
		if (display.IsRPressed() && !display.IsKeydown()) {
			restart = true;
			demoMode = false;
			display.IsKeydown() = true;
		}

		if (display.GetFirstMouse())
		{
			lastX = display.GetMouseX();
			lastY = display.GetMouseY();
			display.GetFirstMouse() = false;
		}

		if (display.IsEPressed()// && Overlap
			) {
			

			camera.GetCameraPos().x = 22;
			camera.GetCameraPos().y = 5;
			camera.GetCameraPos().z = 5;

			if (doorOpen) {
				doorOpen = false;
			}
			else
			{
				doorOpen = true;
			}
		}



		if (display.IsPageUpPressed() && !display.IsKeydown() && !display.IsFullScreen()) {
			if (width == 1280) { 
				width = 1920;
				height = 1080;
			}else 
			if (width == 800){ 
				width = 1280;
				height = 720;
			} 
#define WIDTH width
#define HEIGHT height
	
			display.setWindowSize(WIDTH, HEIGHT);
			camera.UpdatePerspective(70.0f, (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);
			view = camera.GetViewProjection();
			projection = glm::perspective(70.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

			display.IsKeydown() = true;
			display.IsPageUpPressed() = false;
		}

		if (display.IsPageDownPressed() && !display.IsKeydown() && !display.IsFullScreen()) {
			if (width == 1920) {
				width = 1280;
				height = 720;
			}
			else
				if (width == 1280) {
					width = 800;
					height = 600;
				}
#define WIDTH width
#define HEIGHT height
			display.setWindowSize(WIDTH, HEIGHT);
			camera.UpdatePerspective(70.0f, (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);
			view = camera.GetViewProjection();
			projection = glm::perspective(70.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

			display.IsKeydown() = true;
			display.IsPageDownPressed() = false;
		}

		if (display.IsTabPressed() && !display.IsKeydown()) {
			if (!menu) {
				menu = true;
			}
			else { menu = false; }

			display.IsKeydown() = true;
			display.IsTabPressed() = false;
		}

		//Changes to mapView
		if (display.IsMPressed() && !mapView && !display.IsKeydown()) {
				mapPos = currentPos;
				mapView = true;
				camera.GetCameraPos() = glm::vec3(roomdetails.at(mapPos).GetX(), 500, roomdetails.at(mapPos).GetZ());
				glm::vec3 front;
				front.x = 0.0f;
				front.y = -0.99f;
				front.z = 0.01f;
				camera.GetCameraForward() = glm::normalize(front);
				display.IsKeydown() = true;
				display.IsMPressed() = false;
			}

		//Changes out of mapView
		if (display.IsMPressed() && mapView && !display.IsKeydown()) {
				mapView = false;
				currentPos = mapPos;
				camera.GetCameraPos() = glm::vec3(roomdetails.at(mapPos).GetX(), roomdetails.at(mapPos).GetY() + 1.5, roomdetails.at(mapPos).GetZ());
				display.IsMPressed() = false;
				display.IsKeydown() = true;
			}
		
		//Changes to auto camera mode
		if (display.isF1Pressed() && !display.IsKeydown() && !mapView) {
			if (!demoMode) {
				demoMode = true;
				camera.GetCameraPos().x = roomdetails.at(currentPos).GetX();
				camera.GetCameraPos().y = roomdetails.at(currentPos).GetY() + 1.5;
				camera.GetCameraPos().z = roomdetails.at(currentPos).GetZ();
			}
			else {
				demoMode = false;
			}
			display.IsKeydown() = true;
		}

		if (!mapView) {
		GLfloat xoffset = display.GetMouseX() - lastX;
		GLfloat yoffset = lastY - display.GetMouseY(); // Reversed since y-coordinates go from bottom to left
		lastX = display.GetMouseX();
		lastY = display.GetMouseY();

		GLfloat sensitivity = 0.05;	// Change this value to your liking
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		
		camera.GetCameraForward() = glm::normalize(front);
	}
#pragma endregion

#pragma region Auto Camera
		//Changes direction of autocamera
		if (demoMode && display.isF2Pressed() && !display.IsKeydown()) {
			if (!autoBackward) {
				autoBackward = true;
			}
			else {
				autoBackward = false;
			}
			display.IsKeydown() = true;
		}

		//Makes sure user is not in mapview
		if (demoMode && !mapView) {
			if (!autoBackward) {

			//Based on the user's current Position, the camera works out where to be positioned using coordinates from each room.
			if (currentPos == 0) {
				camera.GetCameraPos().z += cameraSpeed;
			}
			else {
				switch (roomdetails.at(currentPos).GetDirection()) {
				case 'f':
					if (roomdetails.at(currentPos).isCorner()) {
						while (camera.GetCameraPos().z < roomdetails.at(currentPos).GetZ() + roomHalfLength) {
							camera.GetCameraPos().z += cameraSpeed;
						}
						if (roomdetails.at(currentPos + 1).GetDirection() == 'l') {
							camera.GetCameraPos().x += cameraSpeed;
						}
						else {
							camera.GetCameraPos().x -= cameraSpeed;
						}
					}
					else {
						camera.GetCameraPos().z += cameraSpeed;
						switch (roomdetails.at(currentPos).GetMeshNum()) {
						case 2:
							while (camera.GetCameraPos().z > roomdetails.at(currentPos).GetZ() + roomHalfLength && camera.GetCameraPos().y < roomdetails.at(currentPos + 1).GetY() + 1.5) {
								camera.GetCameraPos().y += cameraSpeed;
							}
							break;
						case 14:
							while (camera.GetCameraPos().z > roomdetails.at(currentPos).GetZ() + roomHalfLength && camera.GetCameraPos().y > roomdetails.at(currentPos + 1).GetY() + 1.5) {
								camera.GetCameraPos().y -= cameraSpeed;
							}
							break;
						}
					}
					break;
				case 'l':
					if (roomdetails.at(currentPos).isCorner()) {
						while (camera.GetCameraPos().x < roomdetails.at(currentPos).GetX() + roomHalfLength) {
							camera.GetCameraPos().x += cameraSpeed;
						}
						camera.GetCameraPos().z += cameraSpeed;
					}
					else {
						camera.GetCameraPos().x += cameraSpeed;
						switch (roomdetails.at(currentPos).GetMeshNum()) {
						case 2:
							while (camera.GetCameraPos().x > roomdetails.at(currentPos).GetX() + roomHalfLength && camera.GetCameraPos().y < roomdetails.at(currentPos + 1).GetY() + 1.5) {
								camera.GetCameraPos().y += cameraSpeed;
							}
							break;
						case 14:
							while (camera.GetCameraPos().x > roomdetails.at(currentPos).GetX() + roomHalfLength && camera.GetCameraPos().y > roomdetails.at(currentPos + 1).GetY() + 1.5) {
								camera.GetCameraPos().y -= cameraSpeed;
							}
							break;
						}
					}
					break;
				case 'r':
					if (roomdetails.at(currentPos).isCorner()) {
						while (camera.GetCameraPos().x > roomdetails.at(currentPos).GetX() - roomHalfLength) {
							camera.GetCameraPos().x -= cameraSpeed;
						}
						camera.GetCameraPos().z += cameraSpeed;
					}
					else {
						camera.GetCameraPos().x -= cameraSpeed;
						switch (roomdetails.at(currentPos).GetMeshNum()) {
						case 2:
							while (camera.GetCameraPos().x < roomdetails.at(currentPos).GetX() + roomHalfLength && camera.GetCameraPos().y < roomdetails.at(currentPos + 1).GetY() + 1.5) {
								camera.GetCameraPos().y += cameraSpeed;
							}
							break;
						case 14:
							while (camera.GetCameraPos().x < roomdetails.at(currentPos).GetX() + roomHalfLength && camera.GetCameraPos().y > roomdetails.at(currentPos + 1).GetY() + 1.5) {
								camera.GetCameraPos().y -= cameraSpeed;
							}
							break;
							}
						}
						break;
					}
				}
			}
			else if (autoBackward) {
				if (currentPos == 0) {
					if (camera.GetCameraPos().z > 0.5) {
						camera.GetCameraPos().z -= cameraSpeed;
					}
					else {
						camera.GetCameraPos() = glm::vec3(0, 1.5, roomHalfLength);
					}
				}
				else {
					switch (roomdetails.at(currentPos).GetDirection()) {
					case 'f':
						if (roomdetails.at(currentPos).isCorner()) {
							switch (roomdetails.at(currentPos + 1).GetDirection()) {
							case 'r':
								while (camera.GetCameraPos().x < roomdetails.at(currentPos).GetX()) {
									camera.GetCameraPos().x += cameraSpeed;
								}
								break;
							case 'l':
								while (camera.GetCameraPos().x > roomdetails.at(currentPos).GetX()) {
									camera.GetCameraPos().x -= cameraSpeed;
								}
								break;
							}
								camera.GetCameraPos().z -= cameraSpeed;						
						}
						else {
							camera.GetCameraPos().z -= cameraSpeed;
							switch (roomdetails.at(currentPos).GetMeshNum()) {
							case 2:
								while (camera.GetCameraPos().z > roomdetails.at(currentPos).GetZ() && camera.GetCameraPos().y > roomdetails.at(currentPos - 1).GetY() + 1.5) {
									camera.GetCameraPos().y -= cameraSpeed;
								}
								break;
							case 14:
								while (camera.GetCameraPos().z > roomdetails.at(currentPos).GetZ() && camera.GetCameraPos().y < roomdetails.at(currentPos - 1).GetY() + 1.5) {
									camera.GetCameraPos().y += cameraSpeed;
						}
								break;
					}
						}
					break;
					case 'l':
						if (roomdetails.at(currentPos).isCorner()) {
						while (camera.GetCameraPos().z > roomdetails.at(currentPos).GetZ()) {
							camera.GetCameraPos().z -= cameraSpeed;
						}
						camera.GetCameraPos().x -= cameraSpeed;
						}
						else {
							camera.GetCameraPos().x -= cameraSpeed;
							switch (roomdetails.at(currentPos).GetMeshNum()) {
							case 2:
								while (camera.GetCameraPos().x > roomdetails.at(currentPos).GetX() && camera.GetCameraPos().y > roomdetails.at(currentPos - 1).GetY() + 1.5) {
									camera.GetCameraPos().y -= cameraSpeed;
								}
								break;
							case 14:
								while (camera.GetCameraPos().x > roomdetails.at(currentPos).GetX() && camera.GetCameraPos().y < roomdetails.at(currentPos - 1).GetY() + 1.5) {
									camera.GetCameraPos().y += cameraSpeed;
								}
								break;
							}
						}
						break;
					case 'r':
						if (roomdetails.at(currentPos).isCorner()) {
							while (camera.GetCameraPos().z > roomdetails.at(currentPos).GetZ()) {
								camera.GetCameraPos().z -= cameraSpeed;
							}
							camera.GetCameraPos().x += cameraSpeed;
						}
						else {
							camera.GetCameraPos().x += cameraSpeed;
							switch (roomdetails.at(currentPos).GetMeshNum()) {
							case 2:
								while (camera.GetCameraPos().x < roomdetails.at(currentPos).GetX() && camera.GetCameraPos().y > roomdetails.at(currentPos - 1).GetY() + 1.5) {
									camera.GetCameraPos().y -= cameraSpeed;
								}
								break;
							case 14:
								while (camera.GetCameraPos().x < roomdetails.at(currentPos).GetX() && camera.GetCameraPos().y < roomdetails.at(currentPos - 1).GetY() + 1.5) {
									camera.GetCameraPos().y += cameraSpeed;
								}
								break;
							}
						}
						break;
					}
				}
			}
		}

#pragma endregion

		display.Update();
		counter += 0.0003f ;
	}



	return 0;
}
