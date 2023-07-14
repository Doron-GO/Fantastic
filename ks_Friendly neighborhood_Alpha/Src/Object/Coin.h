#pragma once
#include "Common/Transform.h"

class ResourceManager;
class Player;

class Coin
{
public:
	//�Փ˔��苅�̔��a
	static constexpr float COL_RADIUS = 50.0f;

	Coin(Transform transform);
	~Coin();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//���擾
	VECTOR GetPos(void);
	bool GetHit(void);

	//���i�[
	void SetHit(bool flag);

private:
	//�f�o�b�O�\��
	void DrawDebug(void);

	//�G�t�F�N�g
	void PlayEffectCoinSmoke(void);

	//��]
	void ProcessRot(void);
	void Turn(double deg, VECTOR axis);

	ResourceManager& resourceManager_;

	//���Ǘ�
	Transform transform_;

	//��]�֘A
	Quaternion coinRotY_;
	float stepRot_;

	//�Փ˔���
	bool isHit_;

	//�G�t�F�N�g�֘A
	int coinSmoke_;
	float stepEffect_;
};

