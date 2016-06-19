#ifndef ROOMDETAILS_H
#define ROOMDETAILS_H

#include <iostream>
#include "time.h"
#include <glm\glm.hpp>
using namespace std;

class RoomDetails {
public:
	//Used to keep a record of a room's components
	RoomDetails(glm::vec3 coords, char direct, int turnCheck, bool nextToCorner) {
		srand(time(NULL));
			meshNum = rand() % 15 + 1;
			//MeshNum is 4: RightRoom
			//MeshNum is 9: LeftRoom
			//MeshNum is 2: UpRoom
			//MeshNum is 13: DownRoom
			//Other is straight room
			contains = rand() % 6 + 1;
		if (meshNum == 4 && turnCheck == 1) {
			while (meshNum == 4) {
				meshNum = rand() % 15 + 1;
			}
		}
		if (meshNum == 9 && turnCheck == -1) {
			while (meshNum == 9) {
				meshNum = rand() % 15 + 1;
			}
		}
		if ((meshNum == 4 || meshNum == 9) && nextToCorner == true) {
			while (meshNum == 4 || meshNum == 9) {
				meshNum = rand() % 15 + 1;
			}
		}
		if (meshNum == 4 || meshNum == 9) {
			cornerRoom = true;
			contains = 0;
		}	
		if (meshNum == 2 || meshNum == 14) {
			contains = 0;
		}
		roomCoords = coords;
		direction = direct;
			switch (contains) {
			case 1:
				cout << "Room created: Mesh " << meshNum << ", Contains: Collision Meteors " << endl;
				break;
			case 2:
				cout << "Room created: Mesh " << meshNum << ", Contains: Console " << endl;
				break;
			case 3:
				cout << "Room created: Mesh " << meshNum << ", Contains: Music Tiles " << endl;
				break;
			case 4:
				cout << "Room created: Mesh " << meshNum << ", Contains: Towers of Hanoi Game " << endl;
				break;
			case 5:
				cout << "Room created: Mesh " << meshNum << ", Contains: Colour Particles" << endl;
				break;
			case 6:
				cout << "Room created: Mesh " << meshNum << ", Contains: Falling Cubes" << endl;
				break;
            
			default:
				cout << "Room created: Mesh " << meshNum << endl;
				break;
			}
	}

	inline int &GetMeshNum() { return meshNum; }
	inline int &GetContents() { return contains; }
	inline char &GetDirection() { return direction; }
	inline float &GetX() { return roomCoords.x; }
	inline float &GetY() { return roomCoords.y; }
	inline float &GetZ() { return roomCoords.z; }
	inline glm::vec3 &GetPos() { return roomCoords; }
	inline bool isCorner() { return cornerRoom;  }
	inline float GetDoorPos() { return doorPos; }
	inline void OpenDoor() { 
		doorOpen = true;	
	}

protected:
private:
	
	int meshNum, contains;
	glm::vec3 roomCoords;
	char direction;
	bool cornerRoom = false;
	bool doorOpen = false;
	float doorPos = 0;
};

#endif // !ROOMDETAILS_H

