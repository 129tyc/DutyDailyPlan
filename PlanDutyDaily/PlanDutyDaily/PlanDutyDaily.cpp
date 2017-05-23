// PlanDutyDaily.cpp : 定义控制台应用程序的入口点。
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
		printf("无法打开数据文件！\n");
		getchar();
	}
	if (out != nullptr)
		fclose(out);
	return 0;
}
void printfHelp() {
	printf("-s [文件路径] 载入数据文件。\n");
	printf("-l [整数] 限制搜索的结果数量，当小于0时为无限搜索。\n");
	printf("-o [文件路径] 选择输出结果集到文件，若没有该参数则默认输出到屏幕。\n");
}
