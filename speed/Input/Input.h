#pragma once

#include<string>
#include<unordered_map>

enum class InputType {
	keybord,
	gamepad,
	mouse
};

//���͏��(�������)
struct InputInfo {
	InputType type;//���͎��
	//���ۂɒ��ׂ�l(PAD��}�E�X�Ȃ�r�b�g��\��
	//�L�[�{�[�h�o���L�[�R�[�h��\��)
	int inputID;
};

struct InputState
{
	char keycode[256];
};


class Input
{
private:
	//�Q�[������"�Ӗ�"�Ǝ��ۂ̃L�[�R�[�h�̃y�A
	using InptTable_ = std::unordered_map<std::string, std::vector<InputInfo>>;
	InptTable_  inputTable_;

	InputState lastState_ = {};
	InputState currentState_ = {};

	//�l�̑���ƃR�s�[�̋֎~
	Input(const Input&) = delete;
	void operator = (const Input&) = delete;
public:
	Input();
	void Update();
	bool IsTriggerd(const std::string& str)const;
	
	/// <summary>
	/// �{�^����������Ă����
	/// </summary>
	/// <param name="str">�ǂ̃{�^��</param>
	/// <returns></returns>
	bool IsTrigger(const std::string& str)const;


};

