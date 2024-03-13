#ifndef HASHMAP_H
#define HASHMAP_H
#include <string>

template <typename T>
class HashMap
{
public:
	HashMap(double max_load = 0.75) : m_hash(new std::vector<Node*>), m_buckets(10), m_maxload(max_load)
	{
		for (int i = 0; i < m_buckets; i++)
			m_hash->push_back(nullptr);
	} // constructor; initializes a new empty hash table with 10 buckets

	~HashMap()
	{
		for (int i = 0; i < size(); i++)
			delete m_nodes[i];
		delete m_hash;
	} // destructor; deletes all of the items in the hashmap

	int size() const { return m_nodes.size(); } // return the number of associations in the hashmap

	void insert(const std::string& key, const T& value)
	{
		if (find(key) != nullptr) // if key exists, updates the value of key
		{
			*find(key) = value;
			return;
		}
		if ((size() + 1) * 1.0 / m_buckets > m_maxload) // rehash into bigger table if max load is exceeded
			rehash();
		int i = getIndex(key); // ensures that index is always for the new, rehashed table 
		Node* temp = m_hash->at(i);
		m_hash->at(i) = new Node(key, value);
		m_hash->at(i)->next = temp;
		m_nodes.push_back(m_hash->at(i));
	}

	T* find(const std::string& key) const
	{
		int i = getIndex(key);
		if (m_hash->at(i) != nullptr) // searches bucket for Node with matching key
		{
			Node* temp = m_hash->at(i);
			while (temp != nullptr)
			{
				if (temp->m_key == key)
					return &(temp->m_value);
				temp = temp->next;
			}
		}
		return nullptr; // returns nullptr if no matching key is found
	}

	T& operator[](const std::string& key)
	{
		if (find(key) == nullptr) // if key does not currently exist in table, create new default entry
			insert(key, T());
		return *find(key);
	}

private:
	HashMap(const HashMap&);
	HashMap& operator=(const HashMap&);

	struct Node
	{
		Node(std::string key, T value) : m_key(key), m_value(value), next(nullptr) {};
		std::string m_key;
		T m_value;
		Node* next;
	};

	std::vector<Node*> *m_hash; // pointer to current hash table
	std::vector<Node*> m_nodes; // always contains all Nodes
	int m_buckets;
	double m_maxload;

	int getIndex(std::string key) const // function to turn string into an index via built-in string hasher
	{
		size_t h = std::hash<std::string>()(key);
		return h % m_buckets;
	}

	void rehash() // creates a new vector 2x the size of current and rehashes all Nodes into it
	{
		m_buckets *= 2;
		std::vector<Node*> *temp = new std::vector<Node*>;
		for (int i = 0; i < m_buckets; i++) // fill vector with nullptrs(as default empty buckets)
			temp->push_back(nullptr);
		for (int i = 0; i < size(); i++) // fill hash table with rehashed Nodes
			temp->at(getIndex(m_nodes[i]->m_key)) = m_nodes[i];
		delete m_hash;
		m_hash = temp; // replace current hash table
	}
};

#endif
