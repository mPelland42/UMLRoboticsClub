#ifndef ROBOT_H
#define ROBOT_H

#include "Point.h"
#include "MazeMapper.h"
#include "GameState.h"
#include "Constants.h"
#include "Drive.h"

using namespace std;

class Robot {

public:
	Robot();

	void start(void);

	void robotLoop(void);

private:
	float		robotPos;
	float		robotAngle;

	MazeMapper	mazeMapper;
	Drive		drive;

	GameState	gameState;
	Point		safeZoneLocation;

	ColorSensor colorSensor
	IRSensor	IRsensor
	Camera		camera;
};


#endif