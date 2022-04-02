/******************************************************************************/
/* !
/* File   Pathfinder.h
/* Author Sreyash Raychaudhuri
/* Email: srey.raychaudhuri@digipen.edu
/* Date   03/26/2022
/* Pathfinder Component encapsulation header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include "Component.h"
#include "Transform.h"
#include "Movement.h"

enum class PathResult {
	ISNULL,
	COMPLETE,
	IMPOSSIBLE,
};

struct GridCellInfo {
	glm::vec4 pos;
	bool isWall;
};

struct GridPos {
	int row, col;
	bool operator==(GridPos& other);
	bool operator!=(GridPos& other);
};

struct Node {
	GridPos parent_pos;
	GridPos pos;
	float fx;
	float gx;
};

class Pathfinder : public Component {
	
private:
	std::vector<std::vector<GridCellInfo>> grid;
	std::vector<glm::vec4> path;

	Transform* p_owner_transform;
	Movement* p_owner_movement;

	/* two points to travel betweeen. 
	 * Taken two at a time from the path vector once a path is calculated
	 */
	glm::vec4 curr_point, next_point;

	/* Position of the owner marked at the point when it decides
	 * to go back to the center
	 */
	glm::vec4 marked_pos;
	
	// grid properties
	glm::vec4 grid_center_pos;
	float grid_width;
	float grid_height;
	
	// Size of the grid
	int grid_dimension_in_cells;
	
	// Index of the path vector 
	int path_vec_index;
	
	// A reference to the player transform
	Transform* player_transform;
	
	// Found a path? Not possible to go somewhere? Haven't found one yet? Tells you that
	PathResult result;
	
	// Position of the player marked when pathfinding is performed
	glm::vec4 player_pos;

	// To tell the owner if it should go back to the center
	bool go_back_to_center;

	void ResetVariables();

public:
	Pathfinder();
	~Pathfinder() {};

	// Stores a reference to the player game object
	void StorePlayerRef();

	/* AABB collision checker to check if a grid cell collides with another collideable GO
	 If true: Grid space is a wall else not */
	bool GridSpaceIsWall(glm::vec4 pos_0, glm::vec4 pos_1);
	
	// Create a grid from the world space. A* will use this to pathfind
	void CreateGrid();

	/* Use A* algorithm to find the most optimal path from
	 * owner game object to the player game object */
	void FindPath();

	void Update();

	// Does nothing for this component
	void Serialize(json json_object);

	// Store references to other game objects
	void Link();
};

