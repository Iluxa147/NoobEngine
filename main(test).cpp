#include <iostream>
#include <ctime>
#include <string>

#include "vld.h" //Visual Leak Detector
#include <vector>

#define MyDebug

#ifdef MyDebug
int OPushBack;	//O(n)=k+n/2=n (k - push back, n/2 - set pointer to random element)
int OSetRandPtr;//O(n)=n/2=n
int OSer;		//O(n)=n
int ODes;		//O(n)=n+n/2 = n^2 (n - add n elements, n/2 - set pointer to random element)
#endif //MyDebug

class ListNode
{
public:
	ListNode* Prev;
	ListNode* Next;
	ListNode* Rand;
	std::string Data;
	
	unsigned int index_;
};

class ListRand
{
public:
	explicit ListRand() : Head(nullptr), Tail(nullptr), Count(0u) {};
	~ListRand()
	{
		while (Head)
		{
			Tail = Head->Next;
			delete Head;
			Head = Tail;
		}
	};

	void pushBack(const std::string& val, unsigned int index = NULL)
	{
#ifdef MyDebug
		++OPushBack;
#endif //MyDebug

		ListNode *tmp = new ListNode;
		tmp->Next = nullptr;
		tmp->Data = val;
		tmp->index_ = Count++;

		//set random index
		std::srand(unsigned(std::time(0)));
		if (index == NULL)
		{
			index = std::rand() % Count;
		}

		if (Head != nullptr)
		{
			tmp->Prev = Tail;
			Tail->Next = tmp;
			Tail = tmp;
		}
		else
		{
			tmp->Prev = nullptr;
			Head = Tail = tmp;
		}
#ifdef MyDebug
		std::cout << "OPushBack " << OPushBack << " ";
		OPushBack = 0;
#endif //MyDebug

		SetRandElementPtr(tmp, index);
#ifdef MyDebug
		std::cout << "randidx_= " << Tail->Rand->index_ << ' ';
		std::cout << "count_= " << Count;
		std::cout << "\n";
#endif //MyDebug
	};
	void Serialize(std::FILE* s) const
	{
#ifdef MyDebug
		std::cout << "\n";
		std::cout << " ser  ";
#endif //MyDebug
		unsigned int offset = 0u;
		unsigned int length = 0u;
		std::fwrite(&Count, sizeof(unsigned int), 1, s);
		//std::fwrite(this, sizeof(*this), 1, s);
		for (ListNode* node = Head; node != nullptr; node = node->Next)
		{
#ifdef MyDebug
			++OSer;
#endif //MyDebug
			length = node->Data.size();
			std::fwrite(&length, sizeof(unsigned int), 1, s);
			std::fwrite(&node->Data[0], sizeof(char), length, s);
			std::fwrite(&node->Rand->index_, sizeof(unsigned int), 1, s);
			//std::cout << node->Data << " ";
		}
#ifdef MyDebug
		std::cout << "\n";
		std::cout << "OSer " << OSer << " ";
		OSer = 0;
		std::cout << "\n";
#endif //MyDebug

	}
	void Deserialize(std::FILE* s)
	{
		//ListRand* tmpLN = new ListRand;
		/*for (size_t i = 0; i < 10; ++i)
		{
			tmpLN->pushBack("");
		}*/
		//tmpLN->Show();
#ifdef MyDebug
		std::cout << "\n";
		std::cout << " des " << "\n";
#endif //MyDebug
		
		unsigned int index = 0u;
		unsigned int length = 0u;
		unsigned int count = 0u;
		std::string data;
		/*
		unsigned int pos = 0u;

		//std::fread(&count, sizeof(unsigned int), 1, s);
		pos += sizeof(unsigned int);
		std::string Data;
		auto ghddddg = sizeof(Data);// Data;
		auto ghg = sizeof(*this);
		auto ghsdsdsg = sizeof(ListNode);

		tmpLN->Show();
				
		std::fread(&length, sizeof(unsigned int), 1, s);
		//fseek(s, sizeof(unsigned int), SEEK_CUR);
		std::fread(this, 1, sizeof(this), s);
		//fseek(s, sizeof(this), SEEK_CUR);
		std::fread(this->Tail, 1, sizeof(ListNode)*count, s);
		*/
		std::fread(&count, sizeof(unsigned int), 1, s);
		for (unsigned int i = 0; i < count; ++i)
		{
#ifdef MyDebug
			++ODes;
#endif //MyDebug
			std::fread(&length, sizeof(unsigned int), 1, s);
			data.resize(length);
			std::fread(&data[0], sizeof(char), length, s);
			std::fread(&index, sizeof(unsigned int), 1, s);
			pushBack(data, index);
		}
#ifdef MyDebug
		std::cout << "\n";
		std::cout << "ODes " << ODes << " ";
		ODes = 0;
		std::cout << "\n";
#endif //MyDebug
	}
	void Show() const
	{
#ifdef MyDebug
		std::cout << "\n";
		std::cout << " show ";
#endif //MyDebug
		/*ListNode *temp = tail_;

		while (temp != NULL)
		{
			std::cout << temp->data_ << " ";
			temp = temp->prev_;
		}
		std::cout << "\n";*/
		ListNode *temp = Head;
#ifdef MyDebug
		std::cout << "Count= " << Count << " " << "Head= " << Head << " Tail= " << Tail << " ";
#endif //MyDebug
		while (temp != NULL)
		{
#ifdef MyDebug
			std::cout << "\n temp= " << temp << " randidx= " << temp->Rand->index_;
#endif //MyDebug
			std::cout << " data= " << temp->Data << " ";
			temp = temp->Next;
		}
		std::cout << "\n";
	}
private:
	void SetRandElementPtr(ListNode *src, unsigned int index) const
	{

		if (index <= Count / 2) //go to random index from head
		{
			ListNode *tmp = Head;
			while (tmp->index_ != index)
			{
#ifdef MyDebug
				++OSetRandPtr;
#endif //MyDebug

				tmp = tmp->Next;
			}
			src->Rand = tmp;
		}
		else //go to random index from tail
		{
			ListNode *tmp = Tail;
			while (tmp->index_ != index)
			{
#ifdef MyDebug
				++OSetRandPtr;
#endif //MyDebug

				tmp = tmp->Prev;
			}
			src->Rand = tmp;
		}
#ifdef MyDebug
		std::cout << " OSetRandPtr " << OSetRandPtr << " ";
		OSetRandPtr = 0;
#endif //MyDebug

	}

public:
	ListNode* Head;
	ListNode* Tail;
	unsigned int Count;
};


int main(int argc, char** argv)
{
	ListRand serLst;
	for (size_t i = 0; i < 10; ++i)
	{
		std::string data = std::to_string(i);
		serLst.pushBack(data);
	}

	std::FILE *f = std::fopen("ListRand.txt", "wb");
	serLst.Serialize(f);
	std::fclose(f);
	serLst.Show();

	ListRand desLst;
	f = std::fopen("ListRand.txt", "rb");
	desLst.Deserialize(f);
	std::fclose(f);
	desLst.Show();

	system("pause");
	return 0;
}