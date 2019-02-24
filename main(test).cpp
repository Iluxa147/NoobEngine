#include <iostream>
#include <ctime>
#include <string>
#include <unordered_map>

//PushBack;			//O(n) = k			(k - doublelinked list push back)
//SetRandPtr;		//O(n)=n/2 = n		(only half: from head or from tail)
//PushBack rand;	//O(n)=k+n/2 = n	(k - push back, n/2 - set pointer to random element)
//Serialize;		//O(n)=n(k+k) = n	(n - write n elements, k - emplace to hashtable, k - find in hashtable, no collisions)
//Deserialize;		//O(n)=n(k+k+k) = n (n - add n elements, k - push back, k - emplace to hashtable, k - find in hashtable, no collisions)

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

	void pushBack(const std::string& val, bool needRand = true, unsigned int index = NULL)
	{
		ListNode *tmp = new ListNode;
		tmp->Next = nullptr;
		tmp->Data = val;
		++Count;

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
		
		if (needRand) //set random index
		{
			std::srand(unsigned(std::time(0)));
			if (index == NULL)
			{
				index = std::rand() % Count;
			}
			SetRandElementPtr(tmp, index);
		}
	};
	void Serialize(std::FILE* s) const
	{
		std::unordered_map<ListNode*, unsigned int> addresses;
		unsigned int step = 0u;
		unsigned int length = 0u;
		std::fwrite(&Count, sizeof(unsigned int), 1, s);
		for (ListNode* node = Head; node != nullptr; node = node->Next)
		{
			length = node->Data.size();
			std::fwrite(&length, sizeof(unsigned int), 1, s);
			std::fwrite(&node->Data[0], sizeof(char), length, s);
			addresses.emplace(node, step); //fill hashtable with current address and index
			std::fwrite(&addresses.find(node->Rand)->second, sizeof(unsigned int), 1, s); //write an index, which node->Rand points to
			++step;
		}
	}
	void Deserialize(std::FILE* s)
	{
		std::unordered_map<unsigned int, ListNode*> addresses;
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
			
			pushBack(data, false);
			addresses.emplace(i, this->Tail);
			this->Tail->Rand = addresses.find(index)->second;
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
	for (size_t i = 0; i < 10000; ++i)
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