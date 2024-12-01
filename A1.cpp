#include<iostream>
#include<vector>
#include<random>
#include<ctime>

using namespace std;

mt19937 rnd(time(0));

struct Node {
	int x, y, z, y0;
	Node* parent;
	Node* left;
	Node* right;
	Node(int x1 = 0, int y1 = 0, int z1 = 0, int y01 = 0 Node* parent1 = nullptr, Node* left1 = nullptr, Node* right1 = nullptr) {
		x = x1, y = y1, z = z1, y0 = y01, parent = parent1, left = left1, right = right1;
	}
};

struct Treap {
	Node* root = nullptr;
	void rotate (Node*& a, Node*& b) {
		a->parent = b->parent;
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
			if (b->left != nullptr)
				b->left->parent = b;
			a->right = b;
		}
		else {
			b->right = a->left;
			if (b->right != nullptr)
				b->right->parent = b;
			a->left = b;
		}
		b->parent = a;
	}
	Node* find (int& x) {
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
	void insert (Node& x) {
		Node* a = &x;
		if (root == nullptr) {
			root = a;
		}
		Node* p = find(a->x);
		if (p->x == a->x)
			return;
		a->parent = p;
		if (a->x < p->x)
			p->left = a;
		else
			p->right = a;
		while (p != nullptr) {
			if (p->y < a->y) {
				Node* temp = p;
				p = p->parent;
				rotate(a, temp);
			}
			else
				return;
		}
	}
};

int main() {
	int n;
	cin >> n;
	Treap T;
	vector<Node> v(n);
	for (int i = 0; i < n; ++i) {
		cin >> v[i].x >> v[i].y;
		v[i].z = i + 1, v[i].y *= -1;
		T.insert(v[i]);
	}
	cout << "YES" << endl;
	for (int i = 0; i < n; ++i) {
		cout << (v[i].parent == nullptr ? 0 : v[i].parent->z) << " " << (v[i].left == nullptr ? 0 : v[i].left->z) << " " << (v[i].right == nullptr ? 0 : v[i].right->z) << endl;
	}
}
