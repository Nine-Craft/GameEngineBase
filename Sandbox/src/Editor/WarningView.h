#pragma once
#include <string>
class WarningView
{
public:
	void Show();
public:
	/**
	 * \brief allow the user to display a warning message on the cursor
	 * 
	 * \param str
	 *		the warning message
	 */
	static void DisplayWarning(const std::string& str);

private:
	static int s_counter;
	static const int s_max_counter;

	static bool s_ShowWarning;
	static std::string s_WarningMessage;
	static float s_position[2];
};

