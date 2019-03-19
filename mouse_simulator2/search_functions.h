#pragma once
/*
 Name        : search_functions.h
 Description : �W���v���O������search.c�̂����CsearchA()�CsearchB()�֐����ɂ���C
			   �W���v���O�����ł����Ƃ����drive.h�ɋL�q����Ă���֐��̌Ăяo����u��������K�v����B
			   get_wall_info()�֐���main�֐����ɋL�q������B
*/

#include "drive.h"
#include "macro.h"
#include "global.h"
#include "search.h"


/*===========================================================
		�T���n�֐�
===========================================================*/
/*-----------------------------------------------------------
		�����@�T�����s��(���摖�s)
-----------------------------------------------------------*/
//+++++++++++++++++++++++++++++++++++++++++++++++
//searchA
//	�����@��goal���W�ɐi��
// �����F�Ȃ�
// �߂�l�F�Ȃ�
//+++++++++++++++++++++++++++++++++++++++++++++++
void searchA() {																		//�ꎟ���s�@��Ԋ�{�I�ȏ�������

	//====�}�b�v�f�[�^������====
	//map_Init();																		//�}�b�v�f�[�^������������

	//====������������====
	m_step = r_cnt = 0;															//�����ƌo�H�J�E���^�̏�����
	get_wall_info();																	//�Ǐ��̏�����, ��ǂ͂Ȃ��Ȃ�
	write_map();																		//�n�}�̏�����
	make_smap();																	//�����}�̏�����
	make_route();																	//�ŒZ�o�H�T��(route�z��ɓ��삪�i�[�����)
	//====�T�����s====
	do {
		//----�i�s----
		switch (route[r_cnt++]) {												//route�z��ɂ���Đi�s������B�o�H�J�E���^��i�߂�
			//----�O�i----
		case 0x88:
			break;
			//----�E��----
		case 0x44:
			//turn_R90();										//�E��]
			turn_dir(DIR_TURN_R90);											//�}�C�N���}�E�X�����ʒu���ł��E��]����
			//WAIT;												//���肷��܂őҋ@
			break;
			//----180��]----
		case 0x22:
			//turn_180();										//180�x��]
			turn_dir(DIR_TURN_180);											//�}�C�N���}�E�X�����ʒu���ł�180�x��]����
			//WAIT;												//���肷��܂őҋ@
			break;
			//----����----
		case 0x11:
			//turn_L90();										//����]
			turn_dir(DIR_TURN_L90);											//�}�C�N���}�E�X�����ʒu���ł�����]����
			//WAIT;												//���肷��܂őҋ@
			break;
		}

		//a_section();										//�O�i����
		adv_pos();																		//�}�C�N���}�E�X�����ʒu���ł��O�i����
		get_wall_info();				// �T���V���~���[�^�p�ɒǉ�
		print_m_location();		// �T���V���~���[�^�p�ɒǉ�
		conf_route();																	//�ŒZ�o�H�Ői�s�\������

	} while ((PRELOC.AXIS.X != goal_x) || (PRELOC.AXIS.Y != goal_y));
	//���ݍ��W��goal���W���������Ȃ�܂Ŏ��s

//ms_wait(2000);											//�X�^�[�g�ł�***2�b�ȏ�***��~���Ȃ��Ă͂Ȃ�Ȃ�
//turn_180();													//180�x��]
	turn_dir(DIR_TURN_180);													//�}�C�N���}�E�X�����ʒu���ł�180�x��]����
}


//+++++++++++++++++++++++++++++++++++++++++++++++
//searchL
//	����g���@��goal���W�ɐi��
// �����F�Ȃ�
// �߂�l�F�Ȃ�
//+++++++++++++++++++++++++++++++++++++++++++++++
/*
smap[16][16] ... �����@���p���͕����}�b�v�����C����g���@���p���͖K��ς݃��X�g�ŁC
				 0:���T��, 1:�T���� ��\���B
*/
void searchL() {

	//====�K��ς݃��X�g��������====
	UCHAR i;
	for (i = 0; i < 16; i++) {
		UCHAR j;
		for (j = 0; j < 16; j++) {
			smap[i][j] = 0;
		}
	}

	//====�������W�̕Ǐ��擾�ƕǏ��L��====
	get_wall_info();																//�Ǐ��̏�����, ��ǂ͂Ȃ��Ȃ�
	write_map();																	//�n�}�̏�����

	//====�T�����s====
	do {
		smap[PRELOC.AXIS.Y][PRELOC.AXIS.X] = 1;
		UCHAR smap_temp[4];												// �K��ςݏ����i�[�B
							// 3:Front, 2:Right, 1:Back, 0:Left
#define set_smap_temp(A,B,C,D) do{smap_temp[0]=A;smap_temp[1]=B;smap_temp[2]=C;smap_temp[3]=D;}while(0)

// m_dir ... N : 0, W : 1, S : 2, E : 3
		switch (m_dir) {
		case 0:
			set_smap_temp(
				smap[PRELOC.AXIS.Y][PRELOC.AXIS.X - 1],
				smap[PRELOC.AXIS.Y - 1][PRELOC.AXIS.X],
				smap[PRELOC.AXIS.Y][PRELOC.AXIS.X + 1],
				smap[PRELOC.AXIS.Y + 1][PRELOC.AXIS.X]);
			break;
		case 3:
			set_smap_temp(
				smap[PRELOC.AXIS.Y - 1][PRELOC.AXIS.X],
				smap[PRELOC.AXIS.Y][PRELOC.AXIS.X + 1],
				smap[PRELOC.AXIS.Y + 1][PRELOC.AXIS.X],
				smap[PRELOC.AXIS.Y][PRELOC.AXIS.X - 1]);
			break;
		case 2:
			set_smap_temp(
				smap[PRELOC.AXIS.Y][PRELOC.AXIS.X + 1],
				smap[PRELOC.AXIS.Y + 1][PRELOC.AXIS.X],
				smap[PRELOC.AXIS.Y][PRELOC.AXIS.X - 1],
				smap[PRELOC.AXIS.Y - 1][PRELOC.AXIS.X]);
			break;
		case 1:
			set_smap_temp(
				smap[PRELOC.AXIS.Y + 1][PRELOC.AXIS.X],
				smap[PRELOC.AXIS.Y][PRELOC.AXIS.X - 1],
				smap[PRELOC.AXIS.Y - 1][PRELOC.AXIS.X],
				smap[PRELOC.AXIS.Y][PRELOC.AXIS.X + 1]);
			break;

		}

		UCHAR dir;
		//wall_info ... 3bit:Front, 2bit:Right, 1bit:Back, 0bit:Left
		if (!(wall_info & 0X1) && smap_temp[0] == 0) {							//====���ɕǂȂ� && ����斢�K��====
			dir = 0X11;		// ����
		}
		else if (!(wall_info & 0X8) && smap_temp[3] == 0) {					//====�O�ɕǂȂ� && �O��斢�K��====
			dir = 0X88;		// �O�i
		}
		else if (!(wall_info & 0X4) && smap_temp[2] == 0) {					//====�E�ɕǂȂ� && �O��斢�K��====
			dir = 0X44;		// �E��
		}
		else if (!(wall_info & 0X1)) {														//====���ɕǂȂ�====
			dir = 0X11;		// ����
		}
		else if (!(wall_info & 0X8)) {														//====�O�ɕǂȂ�====
			dir = 0X88;		// �O�i
		}
		else if (!(wall_info & 0X4)) {														//====�E�ɕǂȂ�====
			dir = 0X44;		// �E��
		}
		else {																						//====����ȊO====
			dir = 0X22;		// 180�x��]
		}

		//----�i�s----
		switch (dir) {
			//----�O�i----
		case 0x88:

			break;
			//----�E��----
		case 0x44:

			turn_dir(DIR_TURN_R90);
			break;
			//----180��]----
		case 0x22:

			turn_dir(DIR_TURN_180);
			break;
			//----����----
		case 0x11:

			turn_dir(DIR_TURN_L90);
			break;
		}

		adv_pos();
		get_wall_info();				// �T���V���~���[�^�p�ɒǉ�
		write_map();
		print_m_location();		// �T���V���~���[�^�p�ɒǉ�

	} while ((PRELOC.AXIS.X != goal_x) || (PRELOC.AXIS.Y != goal_y));

	turn_dir(DIR_TURN_180);															//�}�C�N���}�E�X�����ʒu���ł�180�x��]����	
}

//+++++++++++++++++++++++++++++++++++++++++++++++
//adv_pos
//	�}�C�N���}�E�X�����ʒu���őO�i������
// �����F�Ȃ�
// �߂�l�F�Ȃ�
//+++++++++++++++++++++++++++++++++++++++++++++++
void adv_pos()
{
	switch (m_dir) {																		//�}�C�N���}�E�X�����݌����Ă�������Ŕ���
	case 0x00:																				//�k�����Ɍ����Ă���ꍇ
		(PRELOC.AXIS.Y)++;															//Y���W���C���N�������g
		break;
	case 0x01:																				//�������Ɍ����Ă���ꍇ
		(PRELOC.AXIS.X)++;															//X���W���C���N�������g
		break;
	case 0x02:																				//������Ɍ����Ă���ꍇ
		(PRELOC.AXIS.Y)--;															//Y���W���f�N�������g
		break;
	case 0x03:																				//�������Ɍ����Ă���ꍇ
		(PRELOC.AXIS.X)--;															//X���W���f�N�������g
		break;
	}
}


//+++++++++++++++++++++++++++++++++++++++++++++++
//conf_route
//	�i�H�𔻒肷��
// �����F�Ȃ�
// �߂�l�F�Ȃ�
//+++++++++++++++++++++++++++++++++++++++++++++++
void conf_route()
{
	//----�Ǐ�񏑂�����----
	write_map();

	//----�ŒZ�o�H��ɕǂ�����ΐi�H�ύX----
	if (wall_info & route[r_cnt]) {
		make_smap();																//�����}�b�v���X�V
		make_route();																//�ŒZ�o�H���X�V
		r_cnt = 0;																		//�o�H�J�E���^��0��
	}
}


//+++++++++++++++++++++++++++++++++++++++++++++++
//map_Init
//	�}�b�v�i�[�z��map[][]�̏�����������
// �����F�Ȃ�
// �߂�l�F�Ȃ�
//+++++++++++++++++++++++++++++++++++++++++++++++
void map_Init()
{
	//====�ϐ��錾====
	UCHAR x, y;																		//for���p�ϐ�

	//====�������J�n====
	//�}�b�v�̃N���A
	for (y = 0; y <= 0x0f; y++) {												//�eY���W�Ŏ��s
		for (x = 0; x <= 0x0f; x++) {											//�eX���W�Ŏ��s
			map[y][x] = 0xf0;														//���(2�����s��)��ǂ���A����(1�����s��)��ǂȂ��Ƃ���B
		}
	}

	//�m��ǂ̔z�u
	for (y = 0; y <= 0x0f; y++) {												//�eY���W�Ŏ��s
		map[y][0] |= 0xf1;														//�Ő��ɕǂ�z�u
		map[y][15] |= 0xf4;														//�œ��ɕǂ�z�u
	}
	for (x = 0; x <= 0x0f; x++) {												//�eX���W�Ŏ��s
		map[0][x] |= 0xf2;														//�œ�ɕǂ�z�u
		map[15][x] |= 0xf8;														//�Ŗk�ɕǂ�z�u
	}
}


//+++++++++++++++++++++++++++++++++++++++++++++++
//write_map
//	�}�b�v�f�[�^����������
// �����F�Ȃ�
// �߂�l�F�Ȃ�
//+++++++++++++++++++++++++++++++++++++++++++++++
void write_map()
{
	//====�ϐ��錾====
	UCHAR m_temp;																	//������␳�����Ǐ��

	//====�Ǐ��̕␳�i�[====
	m_temp = (wall_info >> m_dir) & 0x0f;									//�Z���T�Ǐ���m_dir�Ō�����␳�����ĉ���4bit�����c��
	m_temp |= (m_temp << 4);													//���4bit�ɉ���4bit���R�s�[�B���̍�Ƃ�m_temp��NESW���ŕǂ��i�[

	//====�f�[�^�̏�������====
	map[PRELOC.AXIS.Y][PRELOC.AXIS.X] = m_temp; 					//���ݒn�ɏ�������
	//----���ӂɏ�������----
	//�k���ɂ���
	if (PRELOC.AXIS.Y != 15) {														//���ݍŖk�[�łȂ��Ƃ�
		if (m_temp & 0x88) {															//�k�ǂ�����ꍇ
			map[PRELOC.AXIS.Y + 1][PRELOC.AXIS.X] |= 0x22;		//�k���̋�悩�猩�ē�ǂ������������
		}
		else {																				//�k�ǂ��Ȃ��ꍇ
			map[PRELOC.AXIS.Y + 1][PRELOC.AXIS.X] &= 0xDD;		//�k���̋�悩�猩�ē�ǂȂ�����������
		}
	}
	//�����ɂ���
	if (PRELOC.AXIS.X != 15) {														//���ݍœ��[�łȂ��Ƃ�
		if (m_temp & 0x44) {															//���ǂ�����ꍇ
			map[PRELOC.AXIS.Y][PRELOC.AXIS.X + 1] |= 0x11;		//�����̋�悩�猩�Đ��ǂ������������
		}
		else {																				//�k�ǂ��Ȃ��ꍇ
			map[PRELOC.AXIS.Y][PRELOC.AXIS.X + 1] &= 0xEE;		//�����̋�悩�猩�Đ��ǂȂ�����������
		}
	}
	//��ǂɂ���
	if (PRELOC.AXIS.Y != 0) {														//���ݍœ�[�łȂ��Ƃ�
		if (m_temp & 0x22) {															//��ǂ�����ꍇ
			map[PRELOC.AXIS.Y - 1][PRELOC.AXIS.X] |= 0x88;			//�쑤�̋�悩�猩�Ėk�ǂ������������
		}
		else {																				//��ǂ��Ȃ��ꍇ
			map[PRELOC.AXIS.Y - 1][PRELOC.AXIS.X] &= 0x77;		//�쑤�̋�悩�猩�Ėk�ǂȂ�����������
		}
	}
	//�����ɂ���
	if (PRELOC.AXIS.X != 0) {														//���ݍŐ��[�łȂ��Ƃ�
		if (m_temp & 0x11) {															//���ǂ�����ꍇ
			map[PRELOC.AXIS.Y][PRELOC.AXIS.X - 1] |= 0x44;			//�����̋�悩�猩�ē��ǂ������������
		}
		else {																				//���ǂ��Ȃ��ꍇ
			map[PRELOC.AXIS.Y][PRELOC.AXIS.X - 1] &= 0xBB;		//�����̋�悩�猩�ē����Ȃ�����������
		}
	}
}


//+++++++++++++++++++++++++++++++++++++++++++++++
//turn_dir
//	�}�E�X�̕�����ύX����
// ����1�Ft_pat�E�E�E��]����(drive.h�Ń}�N����`)
// �߂�l�F�Ȃ�
//+++++++++++++++++++++++++++++++++++++++++++++++
void turn_dir(UCHAR	t_pat)
{
	//====������ύX====
	m_dir = (m_dir + t_pat) & 0x03;											//�w�肳�ꂽ��m_dir����]������
}


//+++++++++++++++++++++++++++++++++++++++++++++++
//make_smap
//	�����}�b�v���쐬����
// �����F�Ȃ�
// �߂�l�F�Ȃ�
//+++++++++++++++++++++++++++++++++++++++++++++++
void make_smap()
{
	//====�ϐ��錾====
	UCHAR x, y;																			//for���p�ϐ�
	UCHAR m_temp;																	//�}�b�v�f�[�^�ꎞ�ێ�

	//====�����}�b�v�̃N���A====
	for (y = 0; y <= 0x0f; y++) {													//�eY���W�Ŏ��s
		for (x = 0; x <= 0x0f; x++) {												//�eX���W�Ŏ��s
			smap[y][x] = 0xff;															//���L�������͕����ő�Ƃ���
		}
	}

	//====�S�[�����W��0�ɂ���====
	smap[goal_y][goal_x] = 0;

	//====�����J�E���^��0�ɂ���====
	m_step = 0;																			//���݋L�������ő�̕����ƂȂ�

	//====�����̍��W�ɂ��ǂ蒅���܂Ń��[�v====
	do {
		//----�}�b�v�S���{��----
		for (y = 0; y <= 0x0f; y++) {												//�eY���W�Ŏ��s
			for (x = 0; x <= 0x0f; x++) {											//�eX���W�Ŏ��s
				//----���ݍő�̕����𔭌������Ƃ�----
				if (smap[y][x] == m_step) {										//�����i�[�ϐ�m_step�̒l�����ݍő�̕���
					m_temp = map[y][x];											//map�z�񂩂�}�b�v�f�[�^�����o��
					if (MF.FLAG.SCND) {												//�񎟑��s�p�̃}�b�v���쐬����ꍇ(�񎟑��s����MF.FLAG.SCND��True�ɂ���)
						m_temp >>= 4;												//���4bit���g���̂�4bit���E�ɃV�t�g������
					}
					//----�k�ǂɂ��Ă̏���----
					if (!(m_temp & 0x08) && y != 0x0f) {						//�k�ǂ��Ȃ����ݍŖk�[�łȂ��Ƃ�
						if (smap[y + 1][x] == 0xff) {								//�k�������L���Ȃ�
							smap[y + 1][x] = m_step + 1;						//���̕�������������
						}
					}
					//----���ǂɂ��Ă̏���----
					if (!(m_temp & 0x04) && x != 0x0f) {						//���ǂ��Ȃ����ݍœ��[�łȂ��Ƃ�
						if (smap[y][x + 1] == 0xff) {								//���������L���Ȃ�
							smap[y][x + 1] = m_step + 1;						//���̕�������������
						}
					}
					//----��ǂɂ��Ă̏���----
					if (!(m_temp & 0x02) && y != 0) {							//��ǂ��Ȃ����ݍœ�[�łȂ��Ƃ�
						if (smap[y - 1][x] == 0xff) {								//�쑤�����L���Ȃ�
							smap[y - 1][x] = m_step + 1;							//���̕�������������
						}
					}
					//----���ǂɂ��Ă̏���----
					if (!(m_temp & 0x01) && x != 0) {							//���ǂ��Ȃ����ݍŐ��[�łȂ��Ƃ�
						if (smap[y][x - 1] == 0xff) {								//���������L���Ȃ�
							smap[y][x - 1] = m_step + 1;							//���̕�������������
						}
					}
				}
			}
		}
		//====�����J�E���^�̃C���N�������g====
		m_step++;
	} while (smap[PRELOC.AXIS.Y][PRELOC.AXIS.X] == 0xff);		//���ݍ��W�����L���ł͂Ȃ��Ȃ�܂Ŏ��s
}


//+++++++++++++++++++++++++++++++++++++++++++++++
//make_route
//	�ŒZ�o�H�𓱏o����
// �����F�Ȃ�
// �߂�l�F�Ȃ�
//+++++++++++++++++++++++++++++++++++++++++++++++
void make_route()
{
	//====�ϐ��錾====
	UCHAR i = 0;																			//�J�E���^
	UCHAR x, y;																			//X�AY���W
	UCHAR dir_temp = m_dir;														//�����̕ۊǗp�ϐ�
	UCHAR m_temp;																	//�}�b�v�f�[�^�ꎞ�ێ�

	//====�ŒZ�o�H��������====
	do {
		route[i++] = 0xff;																//route���������Ai���C���N�������g
	} while (i != 0);																		//i��0�łȂ��Ԏ��s(i���I�[�o�[�t���[����0�ɂȂ�܂Ŏ��s�H)

	//====�����J�E���^���Z�b�g====
	m_step = smap[PRELOC.AXIS.Y][PRELOC.AXIS.X];					//���ݍ��W�̕����}�b�v�l���擾

	//====x, y�Ɍ��ݍ��W����������====
	x = (UCHAR)PRELOC.AXIS.X;
	y = (UCHAR)PRELOC.AXIS.Y;

	//====�ŒZ�o�H�𓱏o====
	do {
		m_temp = map[y][x];														//��r�p�}�b�v���̊i�[
		if (MF.FLAG.SCND) {															//�񎟑��s�p�̃}�b�v���쐬����ꍇ(�񎟑��s����MF.FLAG.SCND��True�ɂ���)
			m_temp >>= 4;															//���4bit���g���̂�4bit���E�ɃV�t�g������
		}

		//m_dir ... N : 0, W : 1, S : 2, E : 3
		UCHAR priority[4];																// �T���D�揇���i�[����ϐ�
																								// ������Ԃ��ƁCN,E,S,W�̏��ɗD��x������
#define set_priority(A,B,C,D) do{priority[0]=A;priority[1]=B;priority[2]=C;priority[3]=D;}while(0)

		if (m_dir == 0) {			// m_dir = N : N,E,S,W
			set_priority(0, 1, 2, 3);
		}
		else if (m_dir == 1) {	// m_dir = E : E,S,W,N
			set_priority(1, 2, 3, 0);
		}
		else if (m_dir == 2) {	// m_dir = S : S,W,N,E
			set_priority(2, 3, 0, 1);
		}
		else if (m_dir == 3) {	// m_dir = W : W,N,E,S
			set_priority(3, 0, 1, 2);
		}
		//set_priority(1, 0, 2, 3);

		UCHAR j;
		for (j = 0; j < 4; j++) {
			if (priority[j] == 0) {			//----�k������----
				if (!(m_temp & 0x08) && (smap[y + 1][x] < m_step)) {	//�k���ɕǂ������A���ݒn��菬���������}�b�v�l�ł����
					route[i] = (0x00 - m_dir) & 0x03;								//route�z��ɐi�s�������L�^
					m_step = smap[y + 1][x];											//�ő�����}�b�v�l���X�V
					y++;																			//�k�ɐi�񂾂̂�Y���W���C���N�������g
					break;
				}
			}
			else if (priority[j] == 1) {		//----��������----
				if (!(m_temp & 0x04) && (smap[y][x + 1] < m_step)) {	//�����ɕǂ������A���ݒn��菬���������}�b�v�l�ł����
					route[i] = (0x01 - m_dir) & 0x03;								//route�z��ɐi�s�������L�^
					m_step = smap[y][x + 1];											//�ő�����}�b�v�l���X�V
					x++;																			//���ɐi�񂾂̂�X���W���C���N�������g
					break;
				}
			}
			else if (priority[j] == 2) {		//----�������----
				if (!(m_temp & 0x02) && (smap[y - 1][x] < m_step)) {	//�쑤�ɕǂ������A���ݒn��菬���������}�b�v�l�ł����
					route[i] = (0x02 - m_dir) & 0x03;								//route�z��ɐi�s�������L�^
					m_step = smap[y - 1][x];											//�ő�����}�b�v�l���X�V
					y--;																			//��ɐi�񂾂̂�Y���W���f�N�������g
					break;
				}
			}
			else if (priority[j] == 3) {		//----��������----
				if (!(m_temp & 0x01) && (smap[y][x - 1] < m_step)) {	//�����ɕǂ������A���ݒn��菬���������}�b�v�l�ł����
					route[i] = (0x03 - m_dir) & 0x03;								//route�z��ɐi�s�������L�^
					m_step = smap[y][x - 1];											//�ő�����}�b�v�l���X�V
					x--;																			//���ɐi�񂾂̂�X���W���f�N�������g
					break;
				}
			}
		}

		//----�i�[�f�[�^�`���ύX----
		switch (route[i]) {																	//route�z��Ɋi�[�����v�f�l�ŕ���
		case 0x00:																				//�O�i����ꍇ
			route[i] = 0x88;																	//�i�[�f�[�^�`����ύX
			break;
		case 0x01:																				//�E�܂���ꍇ
			turn_dir(DIR_TURN_R90);													//�������̕�����90�x�E��]
			route[i] = 0x44;																	//�i�[�f�[�^�`����ύX
			break;
		case 0x02:																				//U�^�[������ꍇ
			turn_dir(DIR_TURN_180);													//�������̕�����180�x��]
			route[i] = 0x22;																	//�i�[�f�[�^�`����ύX
			break;
		case 0x03:																				//���܂���ꍇ
			turn_dir(DIR_TURN_L90);													//�������̕�����90�x�E��]
			route[i] = 0x11;																	//�i�[�f�[�^�`����ύX
			break;
		default:																					//����ȊO�̏ꍇ
			route[i] = 0x00;																	//�i�[�f�[�^�`����ύX
			break;
		}
		i++;																						//�J�E���^���C���N�������g
	} while (smap[y][x] != 0);															//�i�񂾐�̕����}�b�v�l��0(=�S�[��)�ɂȂ�܂Ŏ��s
	m_dir = dir_temp;																		//�������n�߂̏�Ԃɖ߂�
}

