#pragma once

class RiderEnemy;

//行動処理基底クラス
class ActionBase
{
public:
	ActionBase(RiderEnemy* enemy) :owner(enemy) {}
	//実行情報
	enum class State
	{
		Run,		//実行中
		Failed,		//実行失敗
		Complete,	//実行成功
	};

	//実行処理(純粋仮想関数)
	virtual ActionBase::State Run(float elapsedTime) = 0;
protected:
	RiderEnemy* owner;
	int step = 0;
};
