
#include<iostream>
#include<vector>
#include<random>
#include<ctime>

using namespace std;

mt19937 rnd(time(0));

struct Node {
	int x, y, sz;
	Node* left;
	Node* right;
	Node(int x1 = 0, int y1 = rnd(), int sz1 = 1, Node* left1 = nullptr, Node* right1 = nullptr) {
		x = x1, y = y1, sz = sz1, left = left1, right = right1;
	}
};

int getsize(Node* a) {
	return a == nullptr ? 0 : a->sz;
}

void print_n (Node* a) {
        if (a != nullptr) {
    	        print_n (a->left);
             	cout << a->x << " ";
             	print_n (a->right);
       	}
}

void update (Node* p) {
   	p->sz = getsize(p->left) + getsize(p->right) + 1;
}

pair<Node*, Node*> split_n(Node*& p, int k) {
	if (p == nullptr) {
		return {nullptr, nullptr};
	}
	push(p);
	if (getsize(p->left) >= k) {
		auto [a, b] = split_n(p->left, k);
		p->left = b;
		push(p->right);
		update(p);
		return {a, p};
	}
	auto [a, b] = split_n(p->right, k - getsize(p->left) - 1);
      	p->right = a;
	push(p->left);
        update(p);
        return {p, b};
}

Node* merge_n(Node* a, Node* b) {
	if (b == nullptr)
		return a;
	if (a == nullptr)
		return b;
	push(a), push(b);
	if (a->y > b->y) {
		a->right = merge_n(a->right, b);
		push(a->left);
		update(a);
		return a;
	}
	b->left = merge_n(a, b->left);
	push(b->right);
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
        pair<Treap, Treap> split(int k) {
                auto [a, b] = split_n(root, k);
                Treap T1(a), T2(b);
                return {T1, T2};
        }
        void push_back(int x) {
                Node* a = new Node(x);
                auto [T1, T2] = split_n(root, getsize(root));
                T1 = merge_n(T1, a);
                root = merge_n(T1, T2);
        }
	int size() {
		return getsize(root);
	}
	int sum() {
		return getsum(root);
	}
	int seg() {
		return getseg(root);
	}
	void color(char c) {
		if (c == 'W')
			root->mod = -1;
		else
			root->mod = 1;
	}
};

Treap merge (Treap T1, Treap T2) {
        Treap T(merge_n(T1.root, T2.root));
        return T;
}
