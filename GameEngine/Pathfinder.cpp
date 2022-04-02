/******************************************************************************/
/* !
/* File:   Pathfinder.cpp
/* Author: Sreyash Raychaudhuri
/* Email: srey.raychaudhuri@digipen.edu
/* Date:   03/26/2022
/* Pathfinder component implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "Pathfinder.h"
#include "GameObjectManager.h"


#include "Collider.h"
#include <iostream>


Pathfinder::Pathfinder() : Component("PATHFINDER"), p_owner_transform(nullptr), p_owner_movement(nullptr),
    player_transform(nullptr), player_pos(glm::vec4(0)), result(PathResult::ISNULL), grid_width(800.0f), grid_height(800.0f),
    grid_dimension_in_cells(20), curr_point(glm::vec4(0)), next_point(glm::vec4(0)), path_vec_index(0), 
    grid_center_pos(glm::vec4(0)), go_back_to_center(false), marked_pos(glm::vec4(0)) {
}

// Stores a reference to the player character
void Pathfinder::StorePlayerRef()
{
    auto go_list = p_game_obj_manager->game_object_list;
    for (auto g : go_list) {
        if (g->GetName() == "player_character") {
            player_transform = static_cast<Transform*>(g->HasComponent("TRANSFORM"));
            player_pos = player_transform->GetPosition();
        }
    }
}

// AABB collision check. Check if there's any object in the grid cell
bool Pathfinder::GridSpaceIsWall(glm::vec4 pos_0, glm::vec4 pos_1)
{
	if ((pos_0.x - pos_0.z) > (pos_1.x + pos_1.z)) {
		return false;
	}

	if ((pos_0.x + pos_0.z) < (pos_1.x - pos_1.z)) {
		return false;
	}

	if ((pos_0.y + pos_0.w) < (pos_1.y - pos_1.w)) {
		return false;
	}

	if ((pos_0.y - pos_0.w) > (pos_1.y + pos_1.w)) {
		return false;
	}

	return true;
}

void Pathfinder::CreateGrid()
{
    // Resize the grid to 20 x 20
	grid.resize(grid_dimension_in_cells);
    for (auto it = grid.begin(); it != grid.end(); it++) {
        it->resize(grid_dimension_in_cells);
	}

    auto position = p_owner_transform->GetPosition();
    grid_center_pos = glm::vec4(position.x, position.y, position.z / 2.0f, position.w / 2.0f);

	auto go_list = p_game_obj_manager->game_object_list;
	
	//glm::vec4 current_grid_cell_pos = pos - glm::vec4(10 * 10, 10 * 10, 0, 0);
    glm::vec4 current_grid_cell_pos = grid_center_pos - glm::vec4(grid_width/2.0f, grid_height/2.0f, 0, 0);

	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].size(); j++) {

			// Assume grid cell is accessible
			grid[i][j] = { current_grid_cell_pos, false };

			// Check if grid cell is a wall or not
			// Must iterate through every game object
			for each (auto go in go_list)
			{
                if (go->HasComponent("COLLIDER")) {
                    glm::vec4 go_col_pos = static_cast<Collider*>(go->HasComponent("COLLIDER"))->GetColliderPosition();

                    // Grid cell is a wall if grid cell collides with an obj that is collideable with
                    if (GridSpaceIsWall(current_grid_cell_pos, go_col_pos) && go->GetName() != "player_character") {
                        grid[i][j] = { current_grid_cell_pos, true }; break;
                    }
                }
			}

			// next pos rightward
			current_grid_cell_pos.x += grid_width / grid_dimension_in_cells;
		}
		// Row complete. Go one row down and all the way to the left again
		current_grid_cell_pos.x -= grid_width;
        current_grid_cell_pos.y += grid_height / grid_dimension_in_cells;
	}
}

// Heuristic Calculation. Using Octile Distance
float hx(GridPos& node_pos, GridPos& goal_pos)
{
    int xDiff = abs(goal_pos.row - node_pos.row);
    int yDiff = abs(goal_pos.col - node_pos.col);

    return std::min(xDiff, yDiff) * sqrtf(2) + std::max(xDiff, yDiff) - std::min(xDiff, yDiff);
}


// Helper function to find square of distance between two positions
float DistanceSquared(glm::vec4 pos_0, glm::vec4 pos_1) {
    return pow(pos_0.x - pos_1.x, 2) + pow(pos_0.y - pos_1.y, 2);
}


// Helper function to see if a node is present in a list
int NodePresent(std::vector<Node>& nodeList, Node& node)
{
    for (int i = 0; i < nodeList.size(); i++) {
        if (node.pos == nodeList[i].pos) {
            return i;
        }
    }
    return INT_MAX;
}

// Comparator to sort nodes
bool NodeSort(Node x, Node y)
{
    return (x.fx < y.fx);
}

bool IsValidGridPos(GridPos pos) {
    if (pos.row < 0 || pos.row > 19 || pos.col < 0 || pos.col > 19) {
        return false;
    }
    return true;
}

// A* Pathfinding (fuckn hope it works)
void Pathfinder::FindPath()
{
    // Get start and goal grid positions (indices)
    glm::vec4 pos = p_owner_transform->GetPosition();
    GridPos start_grid_pos = { INT_MAX, INT_MAX }, goal_grid_pos = { INT_MAX, INT_MAX };
    float min_dist_to_owner_sqrd = FLT_MAX;
    float min_dist_to_player_sqrd = FLT_MAX;

    // finding grid positions of start and end nodes
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            float dist_sqrd = DistanceSquared(grid[i][j].pos, pos);
            if (dist_sqrd < min_dist_to_owner_sqrd) {
                min_dist_to_owner_sqrd = dist_sqrd;
                start_grid_pos = { i, j };
            }
            dist_sqrd = DistanceSquared(grid[i][j].pos, player_pos);
            if (dist_sqrd < min_dist_to_player_sqrd) {
                min_dist_to_player_sqrd = dist_sqrd;
                goal_grid_pos = { i, j };
            }
        }
    }

    std::vector<Node> open_list;
    std::vector<Node> closed_list;

    // Clear all lists
    open_list.clear();
    closed_list.clear();

    // Fill starting node details and push it onto the open list
    Node start_node;
    start_node.pos = start_grid_pos;
    start_node.parent_pos = { INT_MAX, INT_MAX };
    start_node.gx = 0;
    start_node.fx = start_node.gx + hx(start_node.pos, goal_grid_pos);

    open_list.push_back(start_node);
    

    while (!open_list.empty()) {

        // Popping cheapest node off the open list
        // This is the first element as nodes will be sorted
        // in increasing order of total cost
        Node node_popped = open_list[0];
        open_list.erase(open_list.begin());

        // Found goal node!
        if (node_popped.pos == goal_grid_pos) {

            // Computing path here
            std::vector<Node> temp;
            Node temp_node = node_popped;
            temp.push_back(temp_node);

            while (temp_node.pos != start_grid_pos) {
                for (int i = 0; i < closed_list.size(); i++) {

                    if (closed_list[i].pos == temp_node.parent_pos) {
                        temp.push_back(closed_list[i]);
                        temp_node = closed_list[i];
                        break;
                    }
                }
            }

            //temp = RubberBanding(temp);

            // Set final path
            for (int i = temp.size() - 1; i >= 0; i--) {
                path.push_back(grid[temp[i].pos.row][temp[i].pos.col].pos);
            }

            // There is a path to the goal
            result = PathResult::COMPLETE;
            return;
        }
        else {
            int i = node_popped.pos.row;
            int j = node_popped.pos.col;

            // Loading all neighbors
            // from these we'll find possible children
            std::vector<GridPos> neighbor_grid_pos_list = {
                {i + 1, j},
                {i + 1, j - 1},
                {i    , j - 1},
                {i - 1, j - 1},
                {i - 1, j},
                {i - 1, j + 1},
                {i    , j + 1},
                {i + 1, j + 1}
            };

            //int a = i, b = j;

            // Iterate over all neighbors
            for (int k = 0; k < neighbor_grid_pos_list.size(); k++) {

                    // First check if the neighbouring cell is valid
                if (IsValidGridPos(neighbor_grid_pos_list[k])) {
                    if (!grid[neighbor_grid_pos_list[k].row][neighbor_grid_pos_list[k].col].isWall) {

                        Node new_node;

                        // Checking for a diagonal neighbor - diagonals are in odd number indices
                        if (k % 2 != 0) {

                            if (IsValidGridPos(neighbor_grid_pos_list[(k + 1) % 8]) &&
                                IsValidGridPos(neighbor_grid_pos_list[(k + 7) % 8])) 
                            {
                                if (!grid[neighbor_grid_pos_list[(k + 7) % 8].row][neighbor_grid_pos_list[(k + 7) % 8].col].isWall &&
                                    !grid[neighbor_grid_pos_list[(k + 1) % 8].row][neighbor_grid_pos_list[(k + 1) % 8].col].isWall)
                                {
                                    new_node.parent_pos = { i, j };
                                    new_node.pos = neighbor_grid_pos_list[k];
                                    new_node.gx = node_popped.gx + 1.414f; // add root(2) = diagonal distance between grid nodes
                                    new_node.fx = new_node.gx + hx(new_node.pos, goal_grid_pos);
                                }
                                else {
                                    continue;
                                }
                            }
                        }

                        // Neighbors directly above or below the current node 
                        else {

                            new_node.parent_pos = { i, j };
                            new_node.pos = neighbor_grid_pos_list[k];
                            new_node.gx = node_popped.gx + 1;
                            new_node.fx = new_node.gx + hx(new_node.pos, goal_grid_pos);
                        }

                        int found_in_open_list = NodePresent(open_list, new_node);
                        int found_in_closed_list = NodePresent(closed_list, new_node);

                        // If new node is not on open list or closed list
                        if (found_in_open_list == INT_MAX && found_in_closed_list == INT_MAX) {
                            open_list.push_back(new_node);
                        }

                        // If new node is on either open or closed list or both
                        else {

                            bool new_is_cheaper = false;

                            if (found_in_open_list != INT_MAX) {

                                // If cost of new node is cheaper
                                // remove the old one from the open list and add the new one
                                if (new_node.fx < open_list[found_in_open_list].fx) {
                                    open_list.erase(open_list.begin() + found_in_open_list);
                                    new_is_cheaper = true;
                                }
                            }

                            // Same here but remove from the close list
                            if (found_in_closed_list != INT_MAX) {
                                if (new_node.fx < closed_list[found_in_closed_list].fx) {
                                    closed_list.erase(closed_list.begin() + found_in_closed_list);
                                    new_is_cheaper = true;
                                }
                            }

                            if (new_is_cheaper) {
                                open_list.push_back(new_node);
                            }
                        }
                    }
                }
            }

        }

        // Sorting the nodes in the open list
        std::sort(open_list.begin(), open_list.end(), NodeSort);


        // Add the current node (node just popped) to the closed list
        // Done exploring it
        closed_list.push_back(node_popped);
    }

    result = PathResult::IMPOSSIBLE;
}

// Reset some variables (before pathfinding is restarted)
void Pathfinder::ResetVariables()
{
    path.clear();
    curr_point = glm::vec4(0);
    next_point = glm::vec4(0);
    path_vec_index = 0;
    result = PathResult::ISNULL;
}


void Pathfinder::Update()
{
    // If player transform isn't kept -- basically the beginning of the game
    if (player_transform == nullptr) {
        
        StorePlayerRef();

        // Create the grid. Do this one time only
        CreateGrid();
    }

    // Owner goes back to center if it must 
    if (go_back_to_center) {
        marked_pos = p_owner_transform->GetPosition();
        p_owner_movement->SetVelocity(glm::normalize(grid_center_pos - marked_pos) * 50.0f);
        go_back_to_center = false;
    }

    // If player leaves grid bounds - owner is told to go back to center
    if ((fabs(grid_center_pos.x - player_transform->GetPosition().x) > (grid_width / 2.0f) ||
        fabs(grid_center_pos.y - player_transform->GetPosition().y) > (grid_height / 2.0f) &&
         !go_back_to_center)) {

        ResetVariables();
        go_back_to_center = true;
    }

    // If player is in grid bounds and player has moved then only recalculate path
    if (fabs(grid_center_pos.x - player_transform->GetPosition().x) < (grid_width / 2.0f) &&
        fabs(grid_center_pos.y - player_transform->GetPosition().y) < (grid_height / 2.0f) &&
        DistanceSquared(player_transform->GetPosition(), player_pos) > 200.0f) {

        if (go_back_to_center) {
            go_back_to_center = false;
        }

        ResetVariables();

        // Mark new player position
        player_pos = player_transform->GetPosition();

        // Calculate path
        FindPath();
    }

    // Only if a path is calculated
    if (result == PathResult::COMPLETE) {

        // if not initialized or reset
        if (curr_point == glm::vec4(0)) {

            if (path.size() > 1) {
                curr_point = path[path_vec_index++];
                next_point = path[path_vec_index];
            }
        }

         // -------- Movement from one waypoint to another -------- 
        
        // If reached next point
        if (DistanceSquared(p_owner_transform->GetPosition(), next_point) <= 0.01f) {

            // If path comeplete - reset
            if (path_vec_index == path.size() - 1) {
                ResetVariables();
            }
            else { // Move to next point
                curr_point = path[path_vec_index++];
                next_point = path[path_vec_index];
            }
        }
        else {
            // Get going!
            if (next_point != curr_point) {
                p_owner_movement->SetVelocity(glm::normalize(next_point - curr_point) * 50.0f);
            }
            else {
                p_owner_movement->SetVelocity(glm::vec4(0));
            }
        }
    }
}

// Does nothing for thid component
void Pathfinder::Serialize(json json_object) {
}

// Store references to other components
void Pathfinder::Link() {

	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
	p_owner_movement = static_cast<Movement*>(GetOwner()->HasComponent("MOVEMENT"));
}

// Wanna do GridPos a == GridPos b
bool GridPos::operator==(GridPos& other) {

    if (row == other.row && col == other.col) {
        return true;
    }
    return false;
}

// Wanna do GridPos a != GridPos b
bool GridPos::operator!=(GridPos& other) {

    if (row == other.row && col == other.col) {
        return false;
    }
    return true;
}
