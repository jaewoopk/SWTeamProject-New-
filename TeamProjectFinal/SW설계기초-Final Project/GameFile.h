﻿#pragma once
#include "CursorFunctions.h"
#include "ResizeConsole.h"
#include "BackGround.h"
#include "StartMenu.h"
#include "CharacterSelectionMenu.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Item.h"
#include "ExpItem.h"
#include "WeaponItem.h"
#include "UI.h"
#include "Timer.h"
#include "StageMenu.h"
#include "GameOverMenu.h"
#include "SoundPlay.h"
#ifndef GAMEFILE_H
#define GAMEFILE_H

//스토리 모드
void StoryMode() { 
	while (StageNumber < 4) {
		InitStageEnemyHealth();
		InitStageNpcHealth();
		ShowCurrentNSubSkill();
		ShowEnemyHealthBar();
		ShowStoryModeTime();
		ShownHideStageNumber();
		TimeCheckerStart();
		while (1) {
			InvalidateStoryModeTime(); InvalidateEnemyHealthBar(); InvalidateItem(); InvalidateExp(); InvalidateWeaponItem(); InvalidatePlayerLevel(); InvalidateP_Bullet(); InvalidateNpc(); InvalidatePlayer();
			if (NpcKillCount > 30 && !NpcKilledOver) {
				NpcKilledOver = 1; // npc를 30마리 잡았을 경우 InvalidBullet과 InvalidEnemy를 계속 적용하기 위한 변수 NpcKilledOver
				NpcCountOverTimeStart = TimeCheckerEnd() - PausingTime;
			}
			if (NpcKilledOver) { // npc를 30마리 잡았을 경우 발동
				if (TimeCheckerEnd() - PausingTime - NpcCountOverTimeStart > BOSSDURATIONTIME) {
					if (Warningsignflag) { Warningsignflag = 0; HideWarningSign(); mciSendCommandW(dwIDSE_W, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL); }
					InvalidateBullet();
					InvalidateEnemy();
				}
				else { 
					if (!Warningsignflag) { Warningsignflag = 1;  PlayWARNINGSound(); }
					ShowWarningSign();
				}
			}
			if (GameOver()) return; if (StageOver()) break;
		}
	}
}

//무한 모드
/*void InfiniteMode() {		//UI 수정필요
	while (1) {
		InitInfiniteModeEnemyHealth();
		ShowCurrentNSubSkill();
		ShowEnemyHealthBar();
		ShowInfiniteModeTime();
		TimeCheckerStart();
		while (1) { InvalidateInfiniteModeTime(); InvalidateItem(); InvalidateExp(); InvalidateBullet(); InvalidateP_Bullet(); InvalidateEnemy(); InvalidatePlayer(); if (GameOver()) return; }
	}
}*/

//게임 실행 함수 ----게임을 실행할 때 initialize 해야할 것들만 넣기
void RunGame() {
	ResizeConsole(); ResizeConsoleBuffer(); RemoveCursor(); InitUniBoard(); InitializePrisonInfo(); 
	srand((unsigned)time(NULL));
	while (1) {
		mciSendCommandW(dwID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
		mciSendCommandW(dwID_MC, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
		mciSendCommandW(dwID_MF, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
		PlayBGMSound();
		if (StartMenu()) {
			ShowCharacterSelectionMenu();
			ShowBackGround();
			InitializeLifeGauge();
			switch (GameMode) {
			case 0: StoryMode(); break;
			//case 1: InfiniteMode(); break;
			}
			if (GameOverMenu()) return;
		}
		else return;
	}
}
#endif // !GAMEFILE_H
