#include "stdafx.h"

#include <iostream>
#include <cstdio>
#include <string>
using namespace std;

/* This is the node of our linked list */
template <class T>
class ListItem
{
private:
	T* item;
	ListItem<T>* prev;
	ListItem<T>* next;
public:
	ListItem<T>(T* item);
	ListItem<T>* getPrev();
	void setPrev(ListItem<T>* prev);
	ListItem<T>* getNext();
	void setNext(ListItem<T>* next);
	T* getItem();
	void setItem(T* prev);
};

/* This is our doubly linked list */
template <class T>
class LinkedList
{
private:
	ListItem<T>* first; // point to first item
	ListItem<T>* last; // point to last item
public:
	LinkedList<T>();
	// functions to find data in the list
	// please note that T must properly implement/override operator = (equal)
	// in order for these functions  to work
	ListItem<T>* Find(T*);
	ListItem<T>* Find(T);
	void Insert(T*);
	void Insert(T);
	// Delete() can only work if its input is an actual node of this list, or null.
	// So we should only pass result of Find() into this function.
	void Delete(ListItem<T>*);
	int IsEmpty();
	T Maximum();
	T Minimum();
	// 
	ListItem<T>* getFirst();
	// print the entire list, for testing purpose
	void Print(ListItem<T>*);
};

/* used to store data in HashTable */
template <class T>
struct KeyValuePair
{
	string key;
	T value;
};

/* This is our HashTable. T is type of value, type of key is always std::string */
template <class T>
class HashTable
{
private:
	static const int SIZE_MULTIPLIER = 3; // when the number of stored item reached 3 times current bank slot, resize the table
	static const int FULL_SLOT_PERCENT = 80; // when 80% of current slots are used, resize the table
	LinkedList<KeyValuePair<T>>** dataBank; // our list of data slot
	int bankSize;
	int dataCount; // number of values stored in this table
	int slotCount; // number of slots that have data
	// helper functions
	unsigned long HashValueOf(string);
	void IncreaseDataBankSize(); // create bigger data bank and move all stored values into new data bank
	int NeedResize(); // check if the table need to be resized
	LinkedList<KeyValuePair<T>>** CreateEmptyBank(int);
	int Insert(string, T, int); // insert data with the option to enable/disable auto sizing

public:
	static const int DEFAULT_SIZE = 113; // default data bank size, just pick a prime number
	// constructors
	HashTable<T>();
	HashTable<T>(int);
	// function to retrieve data from the table
	KeyValuePair<T>* Retrieve(string);
	// function to insert data to the table, the key is always of type string
	int Insert(string, T);
	// print info of the table, for testing purpose
	void Print();
};

/* Main function to test our HashTable */
int main()
{
	HashTable<string>* hashTable;
	cout << "Create a hashtable with:" << endl;
	cout << " 1. Pre-defined size (" << HashTable<string>::DEFAULT_SIZE << ")" << endl;
	cout << " 2. Custom size" << endl;
	char select = '0';
	while ('1' > select || select > '2')
	{
		select = getchar();
	}
	switch (select)
	{
	case '1':
		hashTable = new HashTable<string>;
		break;
	case '2':
		{
			int size = 0;
			while (size < 1)
			{
				cout << "Enter table size: ";
				cin >> size;
			}
			hashTable = new HashTable<string>(size);
		}
		break;
	}

	cout << endl << endl;
	int quit = 0;
	while (!quit)
	{
		cout << "Select operation:" << endl;
		cout << " 1. Insert contact" << endl;
		cout << " 2. Retrieve number" << endl;
		cout << " 3. Quit" << endl;
		select = '0';
		while ('1' > select || select > '3')
		{
			select = getchar();
		}
		switch (select)
		{
		case '1':
			{
				cin.ignore(INT_MAX, '\n');
				string name, phone;
				cout << "Enter contact name: ";
				getline(cin, name);
				cout << "Enter phone number: ";
				getline(cin, phone);

				hashTable->Insert(name, phone);
				cout << endl;
				hashTable->Print();
			}
			break;
		case '2':
			{
				cin.ignore(INT_MAX, '\n');
				string name;
				cout << "Enter contact name: ";
				getline(cin, name);

				KeyValuePair<string>* result = hashTable->Retrieve(name);
				if (result != nullptr)
				{
					cout << "Phone number: " << result->value;
				}
				else
				{
					cout << "Result not found";
				}
				cout << endl << endl;
			}
			break;
		default:
			quit = 1;
			break;
		}
	}

	getchar();
	return 1;
}

/************************************/
/* ListItem function implementation */
/************************************/
template <class T>
ListItem<T>::ListItem(T* item)
{
	this->item = item;
	this->next = nullptr;
	this->prev = nullptr;
}

template <class T>
ListItem<T>* ListItem<T>::getPrev()
{
	return this->prev;
}

template <class T>
void ListItem<T>::setPrev(ListItem<T>* prev)
{
	this->prev = prev;
}

template <class T>
ListItem<T>* ListItem<T>::getNext()
{
	return this->next;
}

template <class T>
void ListItem<T>::setNext(ListItem<T>* next)
{
	this->next = next;
}

template <class T>
T* ListItem<T>::getItem()
{
	return this->item;
}

template <class T>
void ListItem<T>::setItem(T* item)
{
	this->item = item;
}

/**************************************/
/* LinkedList function implementation */
/**************************************/
template <class T>
LinkedList<T>::LinkedList()
{
	// remember to initialize our pointer
	this->first = nullptr;
	this->last = nullptr;
}

template <class T>
ListItem<T>* LinkedList<T>::Find(T* data)
{
	// start with the first item in the list
	// please note that it can be null
	ListItem<T>* temp = this->first;
	// loop through the list using next pointer
	while (temp != nullptr && *(temp->getItem()) != *data)
	{
		temp = temp->getNext();
	}
	// if temp is null here, the data is not in the list
	return temp;
}

template <class T>
ListItem<T>* LinkedList<T>::Find(T data)
{
	return this->Find(&data);
}

template <class T>
void LinkedList<T>::Insert(T* data)
{
	if (this->first == nullptr)
	{
		// the list is empty
		this->first = new ListItem<T>(data);
		this->last = this->first;
	}
	else
	{
		// create new item
		ListItem<T>* newItem = new ListItem<T>(data);
		// point next pointer of current last item to the new item
		this->last->setNext(newItem);
		// point prev pointer of the new item to current last item
		newItem->setPrev(this->last);
		// move last pointer to the new item
		this->last = newItem;
	}
}

template <class T>
void LinkedList<T>::Insert(T data)
{
	T* dataPtr = new T(data);
	this->Insert(dataPtr);
}

template <class T>
void LinkedList<T>::Delete(ListItem<T>* node)
{
	// the node returned from Find() can be null
	if (node != nullptr)
	{
		ListItem<T>* prev = node->getPrev();
		ListItem<T>* next = node->getNext();

		if (prev == nullptr && next == nullptr)
		{
			// the node is the only one in the list
			this->first = this->last = nullptr;
		}
		else if (prev == nullptr)
		{
			// the node is the first one
			// point first pointer to next node
			this->first = this->first->getNext();
			// clear prev pointer of the next node (which is now the first one)
			this->first->setPrev(nullptr);
		}
		else if (next == nullptr)
		{
			// the node is the last one
			// point last pointer to previous node
			this->last = this->last->getPrev();
			// clear next pointer of the previous node (which is now the last one)
			this->last->setNext(nullptr);
		}
		else
		{
			// the node is in the middle of the list
			// point the next node and the previous node toward each other
			next->setPrev(prev);
			prev->setNext(next);
		}
	}
}

template <class T>
int LinkedList<T>::IsEmpty()
{
	return this->first == nullptr;
}

template <class T>
T LinkedList<T>::Minimum()
{
	T min;
	ListItem<T>* temp = this->first;
	if (temp != nullptr)
	{
		min = *(temp->getItem());
		temp = temp->getNext();
	}
	// loop through the list using next pointer
	while (temp != nullptr)
	{
		if (min > *(temp->getItem()))
		{
			min = *(temp->getItem());
		}
		// move to next node
		temp = temp->getNext();
	}
	return min;
}

template <class T>
T LinkedList<T>::Maximum()
{
	T max;
	ListItem<T>* temp = this->first;
	if (temp != nullptr)
	{
		max = *(temp->getItem());
		temp = temp->getNext();
	}
	// loop through the list using next pointer
	while (temp != nullptr)
	{
		if (max < *(temp->getItem()))
		{
			max = *(temp->getItem());
		}
		// move to next node
		temp = temp->getNext();
	}
	return max;
}

template <class T>
ListItem<T>* LinkedList<T>::getFirst()
{
	return this->first;
}

template <class T>
void LinkedList<T>::Print(ListItem<T>* anchor)
{
	cout << "< ";
	// start with the first item in the list
	// please note that it can be null
	ListItem<T>* temp = this->first;
	// loop through the list using next pointer
	while (temp != nullptr)
	{
		if (temp == anchor)
		{
			// hightlight the anchor
			cout << "(" << *(temp->getItem()) << ") ";
		}
		else
		{
			cout << *(temp->getItem()) << " ";
		}
		// move to next node
		temp = temp->getNext();
	}
	cout << ">" << endl;
}

/*************************************/
/* HashTable function implementation */
/*************************************/
// constructors
template <class T>
HashTable<T>::HashTable()
{
	this->bankSize = DEFAULT_SIZE;
	this->dataCount = 0;
	this->slotCount = 0;
	this->dataBank = this->CreateEmptyBank(this->bankSize);
}

template <class T>
HashTable<T>::HashTable(int capacity)
{
	this->bankSize = capacity;
	this->dataCount = 0;
	this->slotCount = 0;
	this->dataBank = this->CreateEmptyBank(this->bankSize);
}

template <class T>
KeyValuePair<T>* HashTable<T>::Retrieve(string key)
{
	unsigned long hash = this->HashValueOf(key) % this->bankSize;
	if (this->dataBank[hash] == nullptr)
	{
		// there is no data slot matching the above hash value
		return nullptr;
	}
	else
	{
		// we've found the corresponding data slot
		// now we need to iterate through it
		ListItem<KeyValuePair<T>>* current = this->dataBank[hash]->getFirst();
		KeyValuePair<T>* result = nullptr;
		while(current != nullptr)
		{
			if(key.compare(current->getItem()->key) == 0)
			{
				result = current->getItem();
				break;
			}
			current = current->getNext();
		}
		return result;
	}
}

// functions to insert data to the table
template <class T>
int HashTable<T>::Insert(string key, T value)
{
	return this->Insert(key, value, 1);
}

template <class T>
int HashTable<T>::Insert(string key, T value, int autoResize)
{
	unsigned long hash = this->HashValueOf(key) % this->bankSize;
	if (this->dataBank[hash] == nullptr)
	{
		// this slot is empty
		this->slotCount++;
		this->dataBank[hash] = new LinkedList<KeyValuePair<T>>;

	}

	// check if key exists
	ListItem<KeyValuePair<T>>* current = this->dataBank[hash]->getFirst();
	KeyValuePair<T>* result = nullptr;
	while(current != nullptr)
	{
		if(key.compare(current->getItem()->key) == 0)
		{
			result = current->getItem();
			break;
		}
		current = current->getNext();
	}

	if(result == nullptr)
	{
		// insert data
		KeyValuePair<T>* newData = new KeyValuePair<T>;
		newData->key = key;
		newData->value = value;
		this->dataBank[hash]->Insert(newData);
		this->dataCount++;

		// resize
		if (autoResize && this->NeedResize())
		{
			this->IncreaseDataBankSize();
			return 1;
		}
	}
	else
	{
		// update data
		result->value = value;
	}
	return 0;
}

// helper functions
template <class T>
unsigned long HashTable<T>::HashValueOf(string key)
{
	unsigned long hash = 5381;
	for(string::size_type i = 0; i < key.size(); ++i)
	{
		hash = ((hash << 5) + hash) + key[i]; /* hash * 33 + c */
	}
	return hash;
}

template <class T>
void HashTable<T>::IncreaseDataBankSize()
{
	// store old bank
	int oldSize = this->bankSize;
	LinkedList<KeyValuePair<T>>** oldBank = this->dataBank;

	// create bigger data bank
	this->bankSize = this->bankSize * 2 + 1;
	this->dataBank = this->CreateEmptyBank(this->bankSize);
	this->dataCount = 0;
	this->slotCount = 0;

	// move data to newly created bank
	for (int i = 0; i < oldSize; i++)
	{
		if (oldBank[i] != nullptr)
		{
			ListItem<KeyValuePair<T>>* current = oldBank[i]->getFirst();
			while(current != nullptr)
			{
				// insert to new data bank
				this->Insert(current->getItem()->key, current->getItem()->value, 0); // do not resize
				current = current->getNext();
			}
		}
	}
}

template <class T>
int HashTable<T>::NeedResize()
{
	return (this->dataCount >= this->bankSize * SIZE_MULTIPLIER) || (this->slotCount > (this->bankSize * FULL_SLOT_PERCENT / 100));
}

template <class T>
LinkedList<KeyValuePair<T>>** HashTable<T>::CreateEmptyBank(int size)
{
	LinkedList<KeyValuePair<T>>** result = new LinkedList<KeyValuePair<T>>*[size];
	// clear list
	for (int i = 0; i < size; i++)
	{
		result[i] = nullptr;
	}

	return result;
}

template <class T>
void HashTable<T>::Print()
{
	cout << "Used slot: " << this->slotCount << "/" << this->bankSize << " Item: " << this->dataCount << endl << endl;
}
