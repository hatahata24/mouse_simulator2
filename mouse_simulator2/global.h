#pragma once
#pragma once
/*
 Name        : global.h
 Description : �W���v���O������global.h�̂����T���Ɋ֌W�̂�����̂����𔲐������B
*/

#ifndef GLOBAL_H_
#define GLOBAL_H_

/*============================================================
		�e��萔�E�ϐ��錾
============================================================*/
/*------------------------------------------------------------
		�T���n
------------------------------------------------------------*/
//----�S�[�����W----
#define GOAL_X 6	//7
#define GOAL_Y 14	//7

/*------------------------------------------------------------
		���p�E�\���̂̒�`
------------------------------------------------------------*/
//���p�E�\���̂Ƃ́A���p�̂ƍ\���̂�g�ݍ��킹�����̂ŁA
//�����̈ꊇ������A�����o�P�ʂł̑�����\�Ȃ��̂ł���B
//�Ⴆ�΁A�ȉ���mouse_flags���p�E�\���̂�MF�ł́A
//MF.FLAGS = 0x10;�ƑS�̂��ꊇ�ύX�ł���ق��A
//MF.FLAG.DECL = 1;�ƃ����o���w�肵�ĒP�̂ŕύX���邱�Ƃ��o����B
//�Q�l�F
//https://www.grapecity.com/tools/support/powernews/column/clang/033/page02.htm

//----�t���O���p�E�\����----
//���Ԃ͊Ԉ���Ă܂���
#ifndef __MOUSE_FLAGS__						//�Ή��t�@�C���ň�x���ǂݍ��܂�Ă��Ȃ��ꍇ�ȉ����`
#define __MOUSE_FLAGS__						//�ǂݍ��񂾂��Ƃ�\��
typedef union {											//���p�̂̐錾
	unsigned char FLAGS;
	struct ms_flags {									//�\���̂̐錾
		unsigned char RSV : 1;						//�\���r�b�g(B0)		(:1��1�r�b�g���̈Ӗ��A�r�b�g�t�B�[���h)
		unsigned char SCND : 1;						//�񎟃t���O(B1)
		unsigned char SLAL : 1;						//����t���O(B2)
		unsigned char CTRL : 1;						//����t���O(B3)
		unsigned char ACCL : 1;						//�����t���O(B4)
		unsigned char DECL : 1;						//�����t���O(B5)
		unsigned char DEF : 1;						//�葬�t���O(B6)
		unsigned char STOP : 1;						//��~�t���O(B7)
	}FLAG;
} mouse_flags;
#endif

/*�O���[�o���ϐ��̒�`*/
volatile mouse_flags MF;								//���[�^�X�e�[�^�X���܂Ƃ߂����p�E�\����



//----���ݒn�i�[���p�E�\����----
#ifndef __MAP_COOR__								//�Ή��t�@�C���ň�x���ǂݍ��܂�Ă��Ȃ��ꍇ�ȉ����`
#define __MAP_COOR__								//�ǂݍ��񂾂��Ƃ�\��
union map_coor {										//���p�̂̐錾
	UCHAR PLANE;										//YX���W
	struct coor_axis {									//�\���̂̐錾
		UCHAR X : 4;										//X���W
		UCHAR Y : 4;										//Y���W
	}AXIS;
};
#endif

/*�O���[�o���ϐ��̒�`*/
volatile union map_coor PRELOC;				//���ݒn�̍��W���i�[���鋤�p�E�\����


#endif