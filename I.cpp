#include<iostream>
#include<vector>
#include<random>
#include<ctime>

using namespace std;

mt19937 rnd(time(0));

struct Node {
	int x, y, z;
	long long t;
	Node* left;
	Node* right;
	Node(int x1 = 0, int y1 = rnd(), int z1 = 1, Node* left1 = nullptr, Node* right1 = nullptr) {
		x = x1, y = y1, z = z1, t = x1, left = left1, right = right1;
	}
};

int getsize(Node* a) {
	return a == nullptr ? 0 : a->z;
}

long long getsum(Node* a) {
	return a == nullptr ? 0 : a->t;
}

void print_n (Node* a) {
        if (a != nullptr) {
    	        print_n (a->left);
             	cout << a->x << " ";
                print_n (a->right);
       	}
}

void update (Node* p) {
   	p->z = getsize(p->left) + getsize(p->right) + 1;
	p->t = getsum(p->left) + getsum(p->right) + p->x;
}

pair<Node*, Node*> split_n(Node*& p, int x) {
	if (p == nullptr)
		return {nullptr, nullptr};
	if (p->x > x) {
		auto [a, b] = split_n(p->left, x);
		p->left = b;
		update(p);
		return {a, p};
	}
	auto [a, b] = split_n(p->right, x);
      	p->right = a;
        update(p);
        return {p, b};
}

Node* merge_n(Node* a, Node* b) {
	if (b == nullptr)
		return a;
	if (a == nullptr)
		return b;
	if (a->y > b->y) {
		a->right = merge_n(a->right, b);
		update(a);
		return a;
	}
	b->left = merge_n(a, b->left);
	update(b);
	return b;
}

struct Treap {
        Node* root = nullptr;
        Treap(Node* root1 = nullptr) {
                root = root1;
        }
	void print() {
	        Node* p = root;
        	print_n(p);
	}
	pair<Treap, Treap> split(int x) {
        	auto [a, b] = split_n(root, x);
        	Treap T1(a), T2(b);
        	return {T1, T2};
	}
	void insert(int x) {
	        auto [T1, T2] = split_n(root, x);
		auto [T3, T4] = split_n(T1, x - 1);
		if (getsize(T4) != 0) {
			T1 = merge_n(T3, T4);
			root = merge_n(T1, T2);
		}
		else {
			Node* a = new Node(x);
        		T1 = merge_n(T3, a);
        		root = merge_n(T1, T2);
		}
	}
	long long sum() {
		return getsum(root);
	}
};

Treap merge (Treap T1, Treap T2) {
	Treap T(merge_n(T1.root, T2.root));
	return T;
}		

int main() {
	int n;
	cin >> n;
	long long y = 0;
	Treap T;
	for (int i = 0; i < n; ++i) {
		char c;
		cin >> c;
		if (c == '+') {
			int j;
			cin >> j;
			T.insert((j + y) % 1000000000);
			y = 0;
		}
		else {
			int l, r;
			cin >> l >> r;
			auto [T1, T2] = T.split(r);
			auto [T3, T4] = T1.split(l - 1);
			y = T4.sum();
			T1 = merge(T3, T4);
			T = merge(T1, T2);
			cout << y << endl;
		}
	}
}

