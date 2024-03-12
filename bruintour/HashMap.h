#ifndef HASHMAP_H
#define HASHMAP_H
#include <string>

template <typename T>
class HashMap
{
public:
	HashMap(double max_load = 0.75) : m_size(0), m_buckets(10), m_maxload(max_load)
	{
		m_hash = new std::vector<Node*> v(m_buckets, nullptr);
	} // constructor

	~HashMap()
	{
		// delete everything in m_hash
	} // destructor; deletes all of the items in the hashmap

	int size() const { return m_nodes.size(); } // return the number of associations in the hashmap

	// The insert method associates one item (key) with another (value).
	// If no association currently exists with that key, this method inserts
	// a new association into the hashmap with that key/value pair. If there is
	// already an association with that key in the hashmap, then the item
	// associated with that key is replaced by the second parameter (value).
	// Thus, the hashmap must contain no duplicate keys.
	void insert(const std::string& key, const T& value)
	{
		int i = getIndex(key);
		if (find(key) != nullptr)
		{
			*find(key) = value;
			return;
		}
		if ((size() + 1) * 1.0 / m_buckets > m_maxload)
			rehash();
		if ((*m_hash)[i] != nullptr)
		{
			Node* temp = (*m_hash)[i];
			while (temp->next != nullptr)
				temp = temp->next;
			temp->next = new Node(key, value);
			m_nodes.push_back(temp->next);
		}
		else
		{
			(*m_hash)[i] = new Node(key, value);
			m_nodes.push_back(temp->next);
		}
	}

	// If no association exists with the given key, return nullptr; otherwise,
	// return a pointer to the value associated with that key. This pointer can be
	// used to examine that value or modify it directly within the map.
	T* find(const std::string& key) const
	{
		int i = getIndex(key);
		if ((*m_hash)[i] != nullptr)
		{
			Node* temp = (*m_hash)[i];
			while (temp != nullptr)
			{
				if (temp->m_key == key)
					return &temp->m_value;
				temp = temp->next;
			}
		}
		return nullptr;
	}

	// Defines the bracket operator for HashMap, so you can use your map like this:
	// your_map["david"] = 2.99;
	// If the key does not exist in the hashmap, this will create a new entry in
	// the hashmap and map it to the default value of type T. Then it will return a
	// reference to the newly created value in the map.
	T& operator[](const std::string& key);

private:
	Node* m_hash;
	vector<Node*> m_nodes; // points to head nodes inside the hash map
	int m_buckets;
	double m_maxload;

	int getIndex(std::string key)
	{
		size_t h = std::hash<std::string>()(key);
		int index = h % m_buckets;
	}

	void rehash()
	{
		m_buckets *= 2;
		Node* temp = new std::vector<Node*> v(m_buckets*2, nullptr);
		for (int i = 0; i < size(); i++)
		{
			getIndex(m_nodes[i]->m_key)
			
		}
	}

	struct Node
	{
		Node(std::string key, T value) : m_key(key), m_value(value), next(nullptr) {};
		std::string m_key;
		T m_value;
		Node* next;
	};
};

#endif
