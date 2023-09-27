#include "Input.h"
#include<DxLib.h>

Input::Input()
{
	inputTable_["next"] = {
						{ InputType::keybord, KEY_INPUT_RETURN} ,
						{InputType::gamepad,PAD_INPUT_R } };

	inputTable_["pause"] = {
							{InputType::keybord, KEY_INPUT_P},
							{ InputType::gamepad,PAD_INPUT_B } };
	
	inputTable_["right"] = {
							{InputType::keybord, KEY_INPUT_RIGHT},
							{ InputType::gamepad,PAD_INPUT_B } };
	inputTable_["left"] = {
							{InputType::keybord, KEY_INPUT_LEFT},
							{ InputType::gamepad,PAD_INPUT_B } };
	inputTable_["down"] = {
							{InputType::keybord, KEY_INPUT_B},
							{ InputType::gamepad,PAD_INPUT_B } };
	

}

void Input::Update()
{
	lastState_ = currentState_;
	GetHitKeyStateAll(currentState_.keycode);
}

bool Input::IsTriggerd(const std::string& str) const
{
	auto it = inputTable_.find(str);
	if (it == inputTable_.end())
	{
		return false;
	}
	auto inputInfoes = it->second;

	for (const auto& info : inputInfoes)
	{
		if (info.type == InputType::keybord)
		{
			if (!lastState_.keycode[info.inputID])
			{
				if (currentState_.keycode[info.inputID])
				{
					return true;
				}
			}
		}
	}
    return false;
}

bool Input::IsTrigger(const std::string& str) const
{
	auto it = inputTable_.find(str);
	if (it == inputTable_.end())
	{
		return false;
	}
	auto inputInfoes = it->second;
	for (const auto& info : inputInfoes)
	{
		if (info.type == InputType::keybord)
		{
			if (currentState_.keycode[info.inputID])
			{
				return true;
			}
		}
	}
	return false;
}
