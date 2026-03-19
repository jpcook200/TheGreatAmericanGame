#pragma once
#include <string>

class Action
{
	std::string m_name;			///< The name of the action, e.g., "MOVE_LEFT", "JUMP", etc.
	std::string m_type;			///< The type of the action, "START" or "END"

public:
	/*
		@brief Default constructor for Action. Initializes name and type to empty strings.
	*/
	Action();

	/*
		@brief Parameterized constructor for Action.
		@param name The name of the action.
		@param type The type of the action, "START" or "END".
	*/
	Action(const std::string& name, const std::string& type);

	/*
		@brief Getter for the action's name.
		@return The name of the action.
	*/
	const std::string& getName() const;

	/*
		@brief Getter for the action's type.
		@return The type of the action, "START" or "END".
	*/
	const std::string& getType() const;

};