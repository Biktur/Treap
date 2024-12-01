#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

struct Node {
	int x, y, z;
	Node* parent;
	Node* left;
	Node* right;
	Node(int x1 = 0, int y1 = 0, int z1 = 0, Node* parent1 = nullptr, Node* left1 = nullptr, Node* right1 = nullptr) {
		x = x1, y = y1, z = z1, parent = parent1, left = left1, right = right1;
	}
};

struct Ans {
	int p, l, r;
	Ans(int p1 = 0, int l1 = 0, int r1 = 0) {
		p = p1, l = l1, r = r1;
	}
};

bool operator < (Node a, Node b) {
	return a.x < b.x;
}

bool comp (Node a, Node b) {
	return a.z < b.z;
}

struct Treap {
	Node* root = nullptr;
	Node* max = nullptr;
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
	void insert (Node& x) {
		Node* a = &x;
		if (root == nullptr) {
			root = a;
			max = a;
			return;
		}
		Node* p = max;
		a->parent = p;
		p->right = a;
		max = a;
		while (a->parent != nullptr) {
			if (a->parent->y < a->y) {
				p = a->parent;	
				rotate(a, p);
			}
			else {
				return;
			}
		}
	}
};

int main() {
	int n;
	cin >> n;
	Treap T;
	vector<Node> v(n);
	vector<Ans> ans(n);
	for (int i = 0; i < n; ++i) {
		cin >> v[i].x >> v[i].y;
		v[i].z = i + 1, v[i].y *= -1;
	}
	sort(v.begin(), v.end());
	for (int i = 0; i < n; ++i) {
		T.insert(v[i]);
	}
	cout << "YES" << endl;
	for (int i = 0; i < n; ++i) {
		(v[i].parent == nullptr ? ans[v[i].z - 1].p = 0 : ans[v[i].z - 1].p = v[i].parent->z), (v[i].left == nullptr ? ans[v[i].z - 1].l = 0 : ans[v[i].z - 1].l = v[i].left->z), (v[i].right == nullptr ? ans[v[i].z - 1].r = 0 : ans[v[i].z - 1].r = v[i].right->z);
	}
	for (int i = 0; i < n; ++i) {
		cout << ans[i].p << " " << ans[i].l << " " << ans[i].r << endl;
	}
}
