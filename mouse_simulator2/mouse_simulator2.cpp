// mouse_simulator.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <conio.h>

//#include "map_2017_higashinihon.h"
//#include "map_2017_gakusei.h"
#include "map_2019_dentsu.h"
#include "drive.h"
#include "macro.h"
#include "global.h"
#include "search.h"


//プロトタイプ宣言
int real_m_step_cnt;
int real_m_step_map[16][16];
int mode;
int route_dir;

void print_real_m_step_map(int);


//+++++++++++++++++++++++++
//既存の壁情報を動作パターンに反映
//引数：なし
//戻り値：なし
//+++++++++++++++++++++++++
void get_wall_info(void) {
	unsigned char m_temp = 0;
	if (real_wall_data_vertical[PRELOC.AXIS.X][PRELOC.AXIS.Y] == '1') {					//西方向に壁がある場合
		m_temp |= 0X11;
	}
	if (real_wall_data_horizontal[PRELOC.AXIS.Y][PRELOC.AXIS.X] == '1') {				//南方向に壁がある場合
		m_temp |= 0X22;
	}
	if (real_wall_data_vertical[PRELOC.AXIS.X + 1][PRELOC.AXIS.Y] == '1') {				//東方向に壁がある場合
		m_temp |= 0X44;
	}
	if (real_wall_data_horizontal[PRELOC.AXIS.Y + 1][PRELOC.AXIS.X] == '1') {			//北方向に壁がある場合
		m_temp |= 0X88;
	}

	//m_dir ... N : 0, E : 1, S : 2, W : 3
	
	//m_tempに絶対的な方角NESWの順で壁が格納されているものを機体前方を北とした相対的な方角の順に変換
	m_temp = 0X0F & ((m_temp << m_dir) | ((0X0F & m_temp) >> (4 - m_dir)));	//センサ壁情報をm_dirで向きを補正させて下位4bit分を残す
	m_temp |= (m_temp << 4);																				//上位4bitに下位4bitをコピー。この作業でm_tempに相対的な方角の順で壁が格納
	wall_info = m_temp;																							//変換した方角を壁情報変数に代入
	//歩数・X,Y座標・機体の向き・壁情報(機体前を北とした場合)を出力
	//printf("step:%2d, x:%2d, y:%2d, dir:%1d, wall_info:%2X,\n", real_m_step_cnt, PRELOC.AXIS.X, PRELOC.AXIS.Y, m_dir, wall_info);
}


//+++++++++++++++++++++++++
//歩数マップ配列のクリア
//引数：なし
//戻り値：なし
//+++++++++++++++++++++++++
void reset_step_map(void) {
	int i;
	real_m_step_cnt = 0;
	for (i = 0; i < 16; i++) {
		int j;
		for (j = 0; j < 16; j++) {
			real_m_step_map[i][j] = 810;
		}
	}
}


//+++++++++++++++++++++++++
//任意の時点での座標・マウスの方向・壁情報を出力
//引数：なし
//戻り値：なし
//+++++++++++++++++++++++++
void print_m_location(void) {
	real_m_step_map[PRELOC.AXIS.Y][PRELOC.AXIS.X] = real_m_step_cnt;
	real_m_step_cnt++;
	if (real_m_step_cnt > 2 && real_m_step_cnt < 300) {					//歩数・X,Y座標・機体の向き・壁情報(機体前を北とした場合)を出力
		//printf("step:%2d, x:%2d, y:%2d, dir:%1d, wall_info:%2X,\n", real_m_step_cnt-1, PRELOC.AXIS.X, PRELOC.AXIS.Y, m_dir, wall_info);
		//print_real_m_step_map(1);
	}
}


//+++++++++++++++++++++++++
//壁＋歩数マップの出力
//引数：0...壁マップのみ、1...壁＋歩数マップ
//戻り値：なし
//+++++++++++++++++++++++++
void print_real_m_step_map(int show_step_map) {
	int i;
	int k = 1;

	for (i = 15; i >= 0; i--) {
		int j;
		for (j = 0; j < 16; j++) {													//水平方向の壁生成
			printf("+");
			if (real_wall_data_horizontal[i + 1][j] == '1') {
				printf("---");																//水平方向壁情報格納関数内で壁がある状態(=1)であれば"---"を出力する
			}
			else {
				printf("   ");																//水平方向壁情報格納関数内で壁がない状態(=0)であれば"   "を出力する
			}
		}
		printf("+\n");
		for (j = 0; j < 16; j++) {													//鉛直方向の壁生成
			if (real_wall_data_vertical[j][i] == '1') {
				printf("|");																	//鉛直方向壁情報格納関数内で壁がある状態(=1)であれば"|"を出力する
			}
			else {
				printf(" ");																	//鉛直方向壁情報格納関数内で壁がある状態(=0)であれば" "を出力する
			}
			if (show_step_map && real_m_step_map[i][j] != 810) {
				printf("%3d", real_m_step_map[i][j]);							//走行時通過する場合、マップの歩数を区画中央に出力
			}
			else if (show_step_map && mode == 3) {
				make_smap();
				printf("%3d", smap[i][j]);											//歩数マップを全区画中央に出力
			}
			else if (show_step_map && mode == 4) {
				make_smap();
				char t;
				t = _getch();
				printf("%3d", smap[i][j]);											//歩数マップを全区画中央に順番に出力
			}
			else {
				printf("   ");																//通過しない場合、"   "を出力
			}
		}
		printf("|\n");																		//最右壁生成
	}
	for (i = 0; i < 16; i++) {
		printf("+");
		printf("---");																		//最下壁生成
	}
	printf("+\n");
	printf("\n\n");
}


#include "search_functions.h"


int main(void) {

	MF.FLAG.SCND = 0;

	reset_step_map();					//歩数マップ配列のクリア
	goal_x = GOAL_X;        			//GOAL_Xはglobal.hにマクロ定義あり
	goal_y = GOAL_Y;        			//GOAL_Yはglobal.hにマクロ定義あり
	map_Init();							//マップの初期化
	PRELOC.PLANE = 0x00;			//現在地の初期化
	m_dir = 0;								//マウス方向の初期化

	printf("modeを入力してください　");
	scanf_s("%d", &mode);
	
	printf("優先探索方向を入力してください　0 =>N, 1 =>E, 2 =>S, 3 =>W　");
	scanf_s("%d", &route_dir);

	if (mode == 0) {
		print_real_m_step_map(0);	//壁マップのみの出力
		return 0;
	}
	if (mode == 1) {
		print_m_location();				//スタートポジションに0を代入＋全般情報出力
		searchA();
	}
	else if (mode == 2){
		print_m_location();				//スタートポジションに0を代入＋全般情報出力
		searchL();
	}
	else if (mode == 3) {
		print_real_m_step_map(1);	//歩数マップを表示
		return 0;
	}
	else if (mode == 4) {
		print_real_m_step_map(1);	//歩数マップを1つずつ表示
		return 0;
	}


	print_real_m_step_map(1);	//壁＋歩数マップの出力

	reset_step_map();					//歩数マップ配列のクリア
	goal_x = goal_y = 0;

	printf("modeを入力してください　");
	scanf_s("%d", &mode);
	
	if (mode == 1) {
		print_m_location();				//スタートポジションに0を代入＋全般情報出力
		searchA();
	}
	else if (mode == 2) {
		print_m_location();				//スタートポジションに0を代入＋全般情報出力
		searchL();
	}
	else if (mode == 5) {
		print_m_location();				//スタートポジションに0を代入＋全般情報出力
		searchB();
	}


	print_real_m_step_map(1);	//壁＋歩数マップの出力

	MF.FLAG.SCND = 1;

	reset_step_map();					//歩数マップ配列のクリア
	goal_x = GOAL_X;
	goal_y = GOAL_Y;
	PRELOC.PLANE = 0x00;			//現在地の初期化
	m_dir = 0;								//マウス方向の初期化

	printf("modeを再度入力してください　");
	scanf_s("%d", &mode);

	if (mode == 1) {
		print_m_location();				//スタートポジションに0を代入＋全般情報出力
		searchA();
	}
	else if (mode == 2) {
		print_m_location();				//スタートポジションに0を代入＋全般情報出力
		searchL();
	}
	else if (mode == 3) {
		print_real_m_step_map(1);
		return 0;
	}
	else if (mode == 4) {
		print_real_m_step_map(1);
		return 0;
	}
	
	print_real_m_step_map(1);	//壁＋歩数マップの出力
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
