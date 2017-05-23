// PlanDutyDaily.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <string.h>

int main(int argc, char* argv[])
{
	FILE* fp = nullptr;
	FILE* out = nullptr;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(*(argv + i), "-s") == 0)
			fopen_s(&fp, *(argv + i + 1), "r");
		else if (strcmp(*(argv + i), "-o") == 0)
			fopen_s(&out, *(argv + i + 1), "w");
		else if (strcmp(*(argv + i), "-l") == 0)
			setLimit(atoi(*(argv + i + 1)));
		else if (strcmp(*(argv + i), "-h") == 0 ||
			strcmp(*(argv + i), "-help") == 0) {
			printfHelp();
			if (fp != nullptr)
				fclose(fp);
			if (out != nullptr)
				fclose(out);
			return 0;
		}
	}
	if (out != nullptr)
		setOutput(out);
	if (fp != nullptr)
	{
		getData(fp);
		fclose(fp);
	}
	else {
		printf("�޷��������ļ���\n");
		getchar();
	}
	if (out != nullptr)
		fclose(out);
	return 0;
}
void printfHelp() {
	printf("-s [�ļ�·��] ���������ļ���\n");
	printf("-l [����] ���������Ľ����������С��0ʱΪ����������\n");
	printf("-o [�ļ�·��] ѡ�������������ļ�����û�иò�����Ĭ���������Ļ��\n");
}
