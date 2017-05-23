#include "stdafx.h"
using namespace std;


vector<Node> results;
long limit = -1;
FILE* output = nullptr;

int contains(char c, char* buffer) {
	unsigned int i = 0;
	while (*buffer != NULL)
	{
		if (c == *buffer)
			return i;
		i++;
	}
	return INVAILD;
}

int contains(char* c, vector<Member*> members) {
	unsigned int i = 0;
	unsigned int size = members.size();
	for (i = 0; i < size; i++)
		if (STR_EQUEL(members[i]->name, c))
			return i;
	return INVAILD;
}
void setLimit(long value) {
	limit = value;
}
void setOutput(FILE* op) {
	output = op;
}
void pushDuties(Node node, list<Node>* statusQueue,
	unsigned short mPos, unsigned short  dPos) {
	if (node.duties[node.members[mPos].dutiesAvailable[dPos]].currentRest > 0)
	{
		node.mPos = mPos;
		node.duties[node.members[mPos].dutiesAvailable[dPos]].currentRest--;
		node.duties[node.members[mPos].dutiesAvailable[dPos]].memberNames.push_back(node.members[mPos].name);
		statusQueue->push_back(node);
	}
}
void printfResult(FILE* fp, Node node) {
	fprintf(fp, "\n--------------------\n");
	for each (Duty e in node.duties)
	{
		fprintf(fp, "\n<---%s--->", e.name);
		for each (char* ce in e.memberNames)
			fprintf(fp, "%s ", ce);
	}
	fprintf(fp, "\n--------------------\n");
}
void printfResult(Node node) {
	printf("\n--------------------\n");
	for each (Duty e in node.duties)
	{
		printf("\n<---%s--->", e.name);
		for each (char* ce in e.memberNames)
			printf("%s ", ce);
	}
	printf("\n--------------------\n");
}
void arrangeCore(const Node* node) {
	list<Node> statusQueue;
	unsigned short mPos = 0;
	long count = 0;
	unsigned int size = node->members[mPos].dutiesAvailable.size();
	for (int i = 0; i < size; i++)
		pushDuties(*node, &statusQueue, mPos, i);
	while (!statusQueue.empty())
	{
		Node temp = statusQueue.back();
		statusQueue.pop_back();
		if (temp.mPos + 1 < temp.members.size())
		{
			mPos = temp.mPos + 1;
			size = temp.members[mPos].dutiesAvailable.size();
			for (int i = 0; i < size; i++)
				pushDuties(temp, &statusQueue, mPos, i);
		}
		else {
			results.push_back(temp);
			if (output != nullptr)
				printfResult(output, temp);
			else
				printfResult(temp);
			if (limit > 0 && ++count > limit)
				break;
		}
	}
	printf("搜索结束！");
	getchar();
}
bool metRules(vector<Rule> rules, vector<Course*> courses) {
	for each (Rule e in rules)
	{
		bool flag = false;
		for each (Course* e1 in courses)
			if (e1->date.ClassOfDay == e.src->date.ClassOfDay&&
				e1->date.DayOfWeek == e.src->date.DayOfWeek)
			{
				flag = true;
				break;
			}
		if (flag^e.isPermit)
		{
			return false;
		}
	}
	return true;
}
void checkDuty(Member* member,vector<Duty> duties) {
	vector<Course*> courses = member->noClassList;
	unsigned int size = duties.size();
	for (int i = 0; i < size; i++)
		if (metRules(duties[i].rules, courses))
			member->dutiesAvailable.push_back(i);
}
void trimData(vector<Member*> members,vector<Duty> duties) {
	unsigned short total = 0;
	for each (Duty e in duties)
		total += e.capactiy;
	if (total != members.size())
	{
		printf("人数不匹配，找到队员%d人，需要分配岗位数%d个!", members.size(), total);
		getchar();
		return;
	}
	for each (Member* e in members)
		checkDuty(e, duties);
	bool flag = true;
	for each (Member* e in members)
	{
		if (e->dutiesAvailable.size() == 0)
		{
			flag = false;
			printf("成员%s不满足任何一个值班时间！\n此人的无课表为", e->name);
			for each (Course* ce in e->noClassList)
				printf("第%d天,第%d节无课\n",
					ce->date.DayOfWeek, ce->date.ClassOfDay);
			printf("---------------------");
		}
	}
	if (!flag)
	{
		getchar();
		return;
	}
	Node* node = new Node();
	for each (Member* e in members)
	{
		node->members.push_back(*e);
	}
	for (int i = node->members.size() - 1; i > 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			if (node->members[j].dutiesAvailable.size() >
				node->members[j + 1].dutiesAvailable.size())
			{
				Member temp = node->members[j];
				node->members[j] = node->members[j + 1];
				node->members[j + 1] = temp;
			}
		}
	}
	for each (Duty e in duties)
	{
		node->duties.push_back(e);
	}
	arrangeCore(node);
}
void getData(FILE* fp) {
	ParseStatus status;
	char buffer[1024];
	char temp[20];
	char* p, *tp;
	vector<Member*> members;
	vector<Course*> courses;
	vector<Duty> duties;
	while (READLINE != NULL)
	{
		if (STR_EQUEL(buffer, "#无课表#\n"))
			status = NO_CLASS;
		else if (STR_EQUEL(buffer, "#值班计划#\n"))
			status = DUTY;
		else if (STR_EQUEL(buffer, "#结束#\n") ||
			STR_EQUEL(buffer, "#结束#"))
			break;
		else
		{
			switch (status)
			{
			case NO_CLASS: {
				p = buffer;
				while (*p == ' ')
					p++;
				unsigned short dow = atoi(p);
				while (*p != ' ')
					p++;
				while (*p == ' ')
					p++;
				unsigned short cod = atoi(p);
				Course* course = new Course();
				course->date.ClassOfDay = cod;
				course->date.DayOfWeek = dow;
				courses.push_back(course);
				READLINE;
				p = buffer;
				tp = temp;
				bool flag = true;
				while (*p != 0)
				{
					if ((*p == ' ' || *p == '\n') && !flag)
					{
						flag = true;
						*tp = 0;
						tp = temp;
						int pos = contains(temp, members);
						if (pos == INVAILD)
						{
							Member* member = new Member();
							member->name = (char*)malloc(sizeof(char)*strlen(temp));
							strcpy(member->name, temp);
							member->noClassList.push_back(course);
							members.push_back(member);
						}
						else
							members[pos]->noClassList.push_back(course);
					}
					else if (*p != ' '&&*p != '\n')
					{
						flag = false;
						*(tp++) = *p;
					}
					p++;
				}
				break;
			}
			case DUTY: {
				p = buffer;
				tp = temp;
				while (*p != ' ')
					*(tp)++ = *(p++);
				*tp = 0;
				Duty* duty = new Duty();
				duty->name = (char*)malloc(sizeof(char)*strlen(temp));
				strcpy(duty->name, temp);
				while (*p == ' ')
					p++;
				unsigned short capacity = atoi(p);
				duty->capactiy = capacity;
				duty->currentRest = capacity;
				while (*p != ' ')
					p++;
				while (*p != 0 && *p != '\n')
				{
					Rule* rule = new Rule();
					while (*p == ' ')
						p++;
					unsigned short dow = atoi(p);
					while (*p != ' ')
						p++;
					while (*p == ' ')
						p++;
					unsigned short cod = atoi(p);
					while (*p != ' ')
						p++;
					while (*p == ' ')
						p++;
					bool isPermit = atoi(p);
					rule->isPermit = isPermit;
					unsigned short size = courses.size();
					for (int i = 0; i < size; i++)
						if (courses[i]->date.ClassOfDay == cod&&
							courses[i]->date.DayOfWeek == dow)
						{
							rule->src = courses[i];
							break;
						}
					duty->rules.push_back(*rule);
					p++;
				}
				duties.push_back(*duty);
				break;
			}
			default:
				break;
			}
		}
	}
	trimData(members, duties);
}
