#include <iostream>
#include <ctime>
#include <string>
#include <map>

//PushBack;		//O(n)=k+n/2=n (k - push back, n/2 - set pointer to random element)
//SetRandPtr;	//O(n)=n/2=n
//Serialize;	//O(n)=n(logn+logn) = 2nlogn (n - write n elements, logn - emplace to map, logn - find in map)
//Deserialize;	//O(n)=n+n/2 = n^2 (n - add n elements, n/2 - set pointer to element with known index)

class ListNode
{
public:
	ListNode* Prev;
	ListNode* Next;
	ListNode* Rand;
	std::string Data;
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
		ListNode *tmp = new ListNode;
		tmp->Next = nullptr;
		tmp->Data = val;
		++Count;

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
		SetRandElementPtr(tmp, index);
	};
	void Serialize(std::FILE* s) const
	{
		std::map<ListNode*, unsigned int> addresses;
		unsigned int step = 0u;
		unsigned int length = 0u;
		std::fwrite(&Count, sizeof(unsigned int), 1, s);
		for (ListNode* node = Head; node != nullptr; node = node->Next)
		{
			length = node->Data.size();
			std::fwrite(&length, sizeof(unsigned int), 1, s);
			std::fwrite(&node->Data[0], sizeof(char), length, s);

			addresses.emplace(node, step); //fill map with current addresses
			auto element = addresses.find(node->Rand); //find an index for pointer to random element

			std::fwrite(&element->second, sizeof(unsigned int), 1, s); //write an index, on which node->Rand points to
			++step;
		}
	}
	void Deserialize(std::FILE* s)
	{
		unsigned int index = 0u;
		unsigned int length = 0u;
		unsigned int count = 0u;
		std::string data;
		std::fread(&count, sizeof(unsigned int), 1, s);
		for (unsigned int i = 0; i < count; ++i)
		{
			std::fread(&length, sizeof(unsigned int), 1, s);
			data.resize(length);
			std::fread(&data[0], sizeof(char), length, s);
			std::fread(&index, sizeof(unsigned int), 1, s);
			pushBack(data, index);
		}
	}

private:
	void SetRandElementPtr(ListNode *src, unsigned int index) const
	{
		unsigned int offset = 0u;
		if (index <= Count / 2) //go to random index from head
		{
			ListNode *tmp = Head;
			while (offset != index)
			{
				++offset;
				tmp = tmp->Next;
			}
			src->Rand = tmp;
		}
		else //go to random index from tail
		{
			ListNode *tmp = Tail;
			while (offset != index)
			{
				++offset;
				tmp = tmp->Prev;
			}
			src->Rand = tmp;
		}
	}

public:
	ListNode* Head;
	ListNode* Tail;
	unsigned int Count;
};

int main(int argc, char** argv)
{
	ListRand serLst;
	for (size_t i = 0; i < 100; ++i)
	{
		std::string data = std::to_string(i);
		serLst.pushBack(data);
	}

	std::FILE *f = std::fopen("ListRand.dat", "wb");
	serLst.Serialize(f);
	std::fclose(f);

	ListRand desLst;
	f = std::fopen("ListRand.dat", "rb");
	desLst.Deserialize(f);
	std::fclose(f);

	system("pause");
	return 0;
}