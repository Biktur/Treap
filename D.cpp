#include<iostream>
#include<vector>
#include<random>
#include<ctime>

using namespace std;

mt19937 rnd(time(0));

struct Node {
	int x, y, z;
	Node* parent;
	Node* left;
	Node* right;
	Node(int x1 = 0, int y1 = 0, int z1 = 0, Node* parent1 = nullptr, Node* left1 = nullptr, Node* right1 = nullptr) {
		x = x1, y = y1, z = z1, parent = parent1, left = left1, right = right1;
	}
};

struct Treap {
	Node* root = nullptr;
	void rotate (Node*& a, Node*& b) {
		a->parent = b->parent;
		b->z -= a->z;
		a->z += b->z;
		if (b != root) {
			if (b->parent->left == b)
				b->parent->left = a;
			else
				b->parent->right = a;
		}
		else
			root = a;
		if (a == b->left) {
			b->left = a->right;
			if (b->left != nullptr) {
				b->left->parent = b;
				b->z += b->left->z;
			}
			a->right = b;
		}
		else {
			b->right = a->left;
			if (b->right != nullptr) {
				b->right->parent = b;
				b->z += b->right->z;
			}
			a->left = b;
		}
		b->parent = a;
	}
	Node* find (int x) {
		if (root == nullptr) 
			return nullptr;
		Node* p = root;
		while (1) {
			if (x == p->x)
				return p;
			else if (x < p->x) {
				if (p->left == nullptr)
					return p;
				p = p->left;
			}
			else {
				if (p->right == nullptr)
					return p;
				p = p->right;
			}
		}
	}	
	void insert (int x) {
		int y = rnd();
		Node* a = new Node(x, y, 1);
		if (root == nullptr) {
			root = a;
		}
		Node* p = find(x);
		if (p->x == x)
			return;
		a->parent = p;
		if (x < p->x)
			p->left = a;
		else
			p->right = a;
		while (a->parent != nullptr) {
			if (a->parent->y < a->y) {
				p = a->parent;
				p->z++;
				rotate(a, p);
			}
			else
				while (a->parent != nullptr) {
					a = a->parent;
					a->z++;
				}
		}
	}
	void remove (int x) {
		Node* a = find(x);
		Node* temp = a;
		while (temp->parent != nullptr) {
			temp = temp->parent;
			temp->z--;
		}
		while (a->right != nullptr) {
			temp = a->right;
			rotate(temp, a);
			temp->z--;
		}
		if (a->left != nullptr)
			a->left->parent = a->parent;
		if (a != root) {
			if (a == a->parent->right)
				a->parent->right = a->left;
			else
				a->parent->left = a->left;
		}
		else
			root = a->left;
		delete a;
	}
	int kth_element (int k) {
		Node* a = root;
		int s = 0;
		while (1) {
			if (a->left != nullptr) {
				if (s + a->left->z + 1 == k)
					return a->x;
				if (s + a->left->z + 1 < k)
					s += a->left->z + 1, a = a->right;
				else 
					a = a->left;
			}
			else {
				if (s + 1 == k)
					return a->x;
				s += 1, a = a->right;
			}
		}
	}
};

int main() {
	int n;
	cin >> n;
	Treap T;
	for (int i = 0; i < n; ++i) {
		int s, k;
		cin >> s >> k;
		if (s == 1)
			T.insert(-k);
		else if (s == 0)
			cout << -T.kth_element(k) << endl;
		else
			T.remove(-k);
	}
}


