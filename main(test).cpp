#include <iostream>
#include <ctime>
#include <string>

class ListRand
{
	struct ListNode
	{
		ListNode* prev_;
		ListNode* next_;
		ListNode* rand_;
		std::string data_;
		unsigned int index_; //for external access to random element
	};

public:
	explicit ListRand() : head_(nullptr), tail_(nullptr), count_(0u) {};
	~ListRand()
	{
		while (head_)
		{
			tail_ = head_->next_;
			delete head_;
			head_ = tail_;
		}
	};

	void pushBack(const std::string& val, unsigned int index = NULL)
	{
		ListNode *tmp = new ListNode;
		tmp->next_ = nullptr;
		tmp->data_ = val;
		tmp->index_ = count_++;

		//set random index
		std::srand(unsigned(std::time(0)));
		if (index == NULL)
		{
			index = std::rand() % count_;
		}

		if (head_ != nullptr)
		{
			tmp->prev_ = tail_;
			tail_->next_ = tmp;
			tail_ = tmp;
		}
		else
		{
			tmp->prev_ = nullptr;
			head_ = tail_ = tmp;
		}
		SetRandElementPtr(tmp, index);
	};
	void SetRandElementPtr(ListNode *src, unsigned int index)
	{
		if (index <= count_ / (index + 1)) //go to random index from from head
		{
			ListNode *tmp = head_;
			while (tmp->index_ != index)
			{
				tmp = tmp->next_;
			}
			src->rand_ = tmp;
		}
		else //go to random index from from tail
		{
			ListNode *tmp = tail_;
			while (tmp->index_ != index)
			{
				tmp = tmp->prev_;
			}
			src->rand_ = tmp;
		}
	}
	void Serialize(std::FILE* s) const
	{
		unsigned int length = 0u;
		std::fwrite(&count_, sizeof(unsigned int), 1, s);

		for (ListNode* node = head_; node != nullptr; node = node->next_)
		{
			length = node->data_.size();
			std::fwrite(&length, sizeof(unsigned int), 1, s);
			std::fwrite(&node->rand_->index_, sizeof(unsigned int), 1, s);
			std::fwrite(&node->data_[0], sizeof(char), length, s);
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
			data.resize(length + 1);
			std::fread(&index, sizeof(unsigned int), 1, s);
			std::fread(&data[0], sizeof(char), length, s);
			data[length + 1] = '\0';
			pushBack(data, index);
		}
	}

private:
	ListNode* head_;
	ListNode* tail_;
	unsigned int count_;
};


int main(int argc, char** argv)
{
	ListRand serLst;
	serLst.pushBack("12");
	serLst.pushBack("34");
	serLst.pushBack("56");
	serLst.pushBack("78");
	serLst.pushBack("910");

	std::FILE *f = std::fopen("ListRand.txt", "wb");
	serLst.Serialize(f);
	std::fclose(f);

	ListRand desLst;
	f = std::fopen("ListRand.txt", "rb");
	desLst.Deserialize(f);
	std::fclose(f);

	system("pause");
	return 0;
} 