#pragma once

#include "JudgmentBase.h"
#include "RiderEnemy.h"

//TODO:Behavior 行動判定追加

// StartPerfNodeに遷移できるか判定
class StartPerfJudgment :public JudgmentBase
{
public:
	StartPerfJudgment(RiderEnemy* enemy) :JudgmentBase(enemy) {};
	//判定
	bool Judgment();
};

// DamageNodeに遷移できるか判定
class DamageJudgment:public JudgmentBase
{
public:
	DamageJudgment(RiderEnemy* enemy) :JudgmentBase(enemy) {};
	//判定
	bool Judgment();
};

// ChangeNodeに遷移できるか判定
class ChangeJudgment :public JudgmentBase
{
public:
	ChangeJudgment(RiderEnemy* enemy) :JudgmentBase(enemy) {};
	//判定
	bool Judgment();
};

// BattleNodeに遷移できるか判定
class BattleJudgment :public JudgmentBase
{
public:
	BattleJudgment(RiderEnemy* enemy) :JudgmentBase(enemy) {};
	//判定
	bool Judgment();
};

// SpecialNodeに遷移できるか判定
class SpecialJudgment :public JudgmentBase
{
public:
	SpecialJudgment(RiderEnemy* enemy) :JudgmentBase(enemy) {};
	//判定
	bool Judgment();
};

// TerrainNodeに遷移できるか判定
class TerrainJudgment :public JudgmentBase
{
public:
	TerrainJudgment(RiderEnemy* enemy) :JudgmentBase(enemy) {};
	//判定
	bool Judgment();
};

// PursuitNodeに遷移できるか判定
class PursuitJudgment :public JudgmentBase
{
public:
	PursuitJudgment(RiderEnemy* enemy) :JudgmentBase(enemy) {};
	//判定
	bool Judgment();
};

// AttackNodeに遷移できるか判定
class AttackJudgment :public JudgmentBase
{
public:
	AttackJudgment(RiderEnemy* enemy) :JudgmentBase(enemy) {};
	//判定
	bool Judgment();
};

// NormalNodeに遷移できるか判定
class NormalJudgment :public JudgmentBase
{
public:
	NormalJudgment(RiderEnemy* enemy) :JudgmentBase(enemy) {};
	//判定
	bool Judgment();
};

// ChargeNodeに遷移できるか判定
class ChargeJudgment :public JudgmentBase
{
public:
	ChargeJudgment(RiderEnemy* enemy) :JudgmentBase(enemy) {};
	//判定
	bool Judgment();
};

// SummonNodeに遷移できるか判定
class SummonJudgment :public JudgmentBase
{
public:
	SummonJudgment(RiderEnemy* enemy) :JudgmentBase(enemy) {};
	//判定
	bool Judgment();
};

class CoolIdleJudgment :public JudgmentBase
{
public:
	CoolIdleJudgment(RiderEnemy* enemy) :JudgmentBase(enemy) {};
	//判定
	bool Judgment();
};

class CoolPursuitJudgment :public JudgmentBase
{
public:
	CoolPursuitJudgment(RiderEnemy* enemy) :JudgmentBase(enemy) {};
	//判定
	bool Judgment();
};
