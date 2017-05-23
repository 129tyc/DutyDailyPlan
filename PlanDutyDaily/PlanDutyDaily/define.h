#pragma once
#include <vector>
#include <list>
#define INVAILD -1
#define EXIST(c,src) contains(c,src) != INVAILD
#define STR_EQUEL(str1,str2) strcmp(str1,str2) == NULL
#define READLINE fgets(buffer, sizeof(buffer), fp)
using namespace std;

struct Date {
	unsigned short DayOfWeek;
	unsigned short ClassOfDay;
};

struct Course {
	Date date;
	char* name;
};

struct Rule {
	Course* src;
	bool isPermit;
};

struct Duty {
	char* name;
	vector<Rule> rules;
	vector<char*> memberNames;
	unsigned short capactiy;
	unsigned short currentRest;
};

struct Member {
	char* name;
	vector<Course*> noClassList;
	vector<int> dutiesAvailable;
};

struct Node
{
	vector<Member> members;
	vector<Duty> duties;
	unsigned short mPos;
};

enum ParseStatus
{
	NO_CLASS,
	DUTY
};
void getData(FILE* fp);
int contains(char c, char* buffer);
int contains(char* c, vector<Member*> members);
void setLimit(long value);
void setOutput(FILE* op);
void getData(FILE* fp);
void trimData(vector<Member*> members, vector<Duty> duties);
void checkDuty(Member* member, vector<Duty> duties);
bool metRules(vector<Rule> rules, vector<Course*> courses);
void arrangeCore(const Node* node);
void printfResult(Node node);
void printfResult(FILE* fp, Node node);
void pushDuties(Node node, list<Node>* statusQueue,
	unsigned short mPos, unsigned short  dPos);

void printfHelp();