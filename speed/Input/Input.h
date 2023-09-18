#pragma once

#include<string>
#include<unordered_map>

enum class InputType {
	keybord,
	gamepad,
	mouse
};

//入力情報(一つ当たり)
struct InputInfo {
	InputType type;//入力種別
	//実際に調べる値(PADやマウスならビットを表し
	//キーボードバラキーコードを表す)
	int inputID;
};

struct InputState
{
	char keycode[256];
};


class Input
{
private:
	//ゲーム中の"意味"と実際のキーコードのペア
	using InptTable_ = std::unordered_map<std::string, std::vector<InputInfo>>;
	InptTable_  inputTable_;

	InputState lastState_ = {};
	InputState currentState_ = {};

	//値の代入とコピーの禁止
	Input(const Input&) = delete;
	void operator = (const Input&) = delete;
public:
	Input();
	void Update();
	bool IsTriggerd(const std::string& str)const;
	
	/// <summary>
	/// ボタンが押されている間
	/// </summary>
	/// <param name="str">どのボタン</param>
	/// <returns></returns>
	bool IsTrigger(const std::string& str)const;


};

