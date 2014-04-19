#ifndef PRECOMPILED_H
#define PRECOMPILED_H
#include <winsock2.h>
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
//#include <gl\glaux.h>
#include <mmsystem.h>
#include <cmath>
#include <stdio.h> 
#include <string.h>
#include <fstream>
#include <iostream>
#include <math.h>						// Header File For Windows Math Library		( ADD )
#include <stdio.h>						// Header File For Standard Input/Output	( ADD )
#include <stdarg.h>						// Header File For Variable Argument Routines	( ADD )
#include "GLwindow.h"
#include "texture.h"
#include "Vector2.h"
#include "Vector3.h"
#define PI 3.14159
#define BITMAP_ID 0x4D42				// the universal bitmap ID
#include <sstream>
#include "BasicCamera.h"
#include <list>
#include "Timing.h"
#include <time.h>
#include "BitPacker.h"
#include "Net.h"
#include <map>
using namespace std;

enum PROJECTILE_OWNER
{
		PROJ_PLAYER,
		PROJ_NPC
};

// different networking enum values
enum ENUM_message
{	
		BLANK,
		ID,
		STATE,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		FIRE,
		ENDOFMESSAGE
};

// the differen types of tiles in the maze
	enum ENUM_tileTypes
	{
		GRASS,
		WALL,
		SAND,
		SLAB
	};
#endif