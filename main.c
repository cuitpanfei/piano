#include <stdio.h>
#include <Windows.h>
#include <malloc.h>
#include <stdlib.h>
#include <conio.h>
#pragma comment(lib,"winmm.lib")
int shift = 5;
enum Scale {
	C=0,CS,D,DS,E,F,FS,G,GS,A,AS,B
};
int key2scale(char key, int shift) {
	if (key >= 'a'&&key <= 'z')key += 'A' - 'a';
	switch (key)
	{
	case'A' : return C+12*shift;
	case 'S':return D + 12 * shift;
	case 'D':return E + 12 * shift;
	case 'F':return F + 12 * shift;
	case 'G':return G + 12 * shift;
	case 'H':return A + 12 * shift;
	case 'J':return B + 12 * shift;

	case'Q': return C + 12 * shift +12;
	case 'W':return D + 12 * shift + 12;
	case 'E':return E + 12 * shift + 12;
	case 'R':return F + 12 * shift + 12;
	case 'T':return G + 12 * shift + 12;
	case 'Y':return A + 12 * shift + 12;
	case 'U':return B + 12 * shift + 12;

	case'Z': return C + 12 * shift - 12;
	case 'X':return D + 12 * shift - 12;
	case 'C':return E + 12 * shift - 12;
	case 'V':return F + 12 * shift - 12;
	case 'B':return G + 12 * shift - 12;
	case 'N':return A + 12 * shift - 12;
	case 'M':return B + 12 * shift - 12;
	default:
		return 0;
	}

}
void shift_change(char key,int state_code,int offset) {
	static char last_code = 0x0;
	static char last_key = 0x0;
	if (state_code == last_code&&last_key==key)return;
	last_code = state_code;
	last_key = key;
	shift += offset;

}
void Piano() {
	HMIDIOUT handle;
	int key_state = 0;
	midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
	system("cls");
	printf("钢琴已开启，敲击键盘Q-U,A-J,Z-M\n当前音阶C%,dSHIFT键提高一个8度，ctrl键降低一个8度,请按L键退出\n", shift);
	while (1) {
		for (char i = 0x00; i <= 0xff; i++) {
			key_state = GetKeyState(i);
			if (key_state < 0) {
				if (i == 'L') {
					midiOutClose(handle);
					return;
				}
				midiOutShortMsg(handle, (0x007f << 16) + (key2scale(i,shift) << 8) + 0x90);
				if (i == VK_SHIFT)
					shift_change(i, key_state, 1);

				if (i == VK_CONTROL)
					shift_change(i, key_state, -1);
				system("cls");
				printf("当前音阶C%d,SHIFT键提高一个8度，ctrl键降低一个8度,请按L键退出\n",shift);
				printf("已按下%c键\n", i);
				while (GetKeyState(i) < 0)Sleep(1);

			}
		}
	}
}
void str2scale(const char* str) {
	HMIDIOUT handle;
	midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
	char* p = str,byte;
	int scale;
	while (*p) {
		if (*p >= '0'&& *p <= '9')
		{
			scale += ((*p) - '0')*12;
			printf("%p ", scale);
			midiOutShortMsg(handle, (0x007f << 16) + (scale << 8) + 0x90);
			Sleep(200);
		}
		else {
			byte = *p;
			if (byte >= 'a'&&byte <= 'z')
				byte += 'A' - 'a';
			switch (*p)
			{
			case 'C':scale = C; break;
			case 'D':scale = D; break;
			case 'E':scale = E; break;
			case 'F':scale = F; break;
			case 'G':scale = G; break;
			case 'A':scale = A; break;
			case 'B':scale = B; break;
			case '#':scale++; break;
			case 'S':scale++; break;
			case ' ':Sleep(200); break;
			case '_':Sleep(300); break;
			case '-':Sleep(100); break;
			case '\n':Sleep(600); break;
			default:
				break;
			}
		}
		p++;
	}
	Sleep(1000);
	midiOutClose(handle);
	
}

void load_midi() {
	char path[1024],*data;
	int lenth;
	FILE *fp;
	system("cls");
	printf("请输入文件路径:");
	gets(path);
	fp= fopen(path, "rb");
	if (fp == NULL) {
		puts("文件路径错误");
		system("pause");
		return;
	}
	fseek(fp, 0, SEEK_END);
	lenth = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	data = (char*)malloc(lenth);
	fread(data, lenth, 1, fp);
	puts("音符数据为:");
	puts(data);
	str2scale(data);
	fclose(fp);
	system("pause");
}
void play_midi() {
	char data[1024];
	system("cls");
	puts("请输入曲谱数据:");
	gets(data);
	str2scale(data);
	system("pause");

}
void menu() {
	char cmd=0;
start:
	system("cls");
	puts("\t\t\t******************************************************");
	puts("\t\t\t*----------------欢迎使用雪风钢琴系统----------------*");
	puts("\t\t\t*----------------1.自由弹奏--------------------------*");
	puts("\t\t\t*----------------2.曲谱演奏--------------------------*");
	puts("\t\t\t*----------------3.导入曲谱--------------------------*");
	puts("\t\t\t*----------------4.退出程序--------------------------*");
	puts("\t\t\t*----------------请输入命令编号操作------------------*");
	fflush(stdin);
	while (!kbhit());
	cmd = getch();
	switch (cmd)
	{
	case '1':Piano(); break;
	case '2':play_midi(); break;
	case '3':load_midi(); break;
	case '4':return;
	default:
		printf("指令错误！");
		system("pause"); 
		break;
	}
	goto start;

}
int main() {
	menu();
	return 0;
}