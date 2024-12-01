#include<iostream>
#include<vector>
#include<random>
#include<ctime>

using namespace std;

mt19937 rnd(time(0));

struct Node {
	int x, y, sz, sum, seg, mod;
	int lb, rb;
	Node* left;
	Node* right;
	Node(int x1 = 0, int y1 = rnd(), int sz1 = 1, Node* left1 = nullptr, Node* right1 = nullptr) {
		x = x1, y = y1, sz = sz1, sum = x1, seg = 0, mod = 0, lb = 0, rb = 0, left = left1, right = right1;
	}
};

void push(Node* p) {
	if (p == nullptr)
		return;
	if (p->mod == 1) {
		p->x = 1;
		p->sum = p->sz;
		p->seg = 1;
		p->lb = 1, p->rb = 1;
	}
	else if (p->mod == -1) {
		p->x = 0;
		p->sum = 0;
		p->seg = 0;
		p->lb = 0, p->rb = 0;
	}
	if (p->mod != 0) {
      		if (p->left != nullptr)
                	p->left->mod = p->mod;
      		if (p->right != nullptr)
              		p->right->mod = p->mod;
	}
	p->mod = 0;
}

int getsum(Node* a) {
	return a == nullptr ? 0 : a->sum;
}

int getseg(Node* a) {
	return a == nullptr ? 0 : a->seg;
}

int getsize(Node* a) {
	return a == nullptr ? 0 : a->sz;
}

bool getlb(Node* a) {
	return a == nullptr ? 0 : a->lb;
}

bool getrb(Node* a) {
	return a == nullptr ? 0 : a->rb;
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
	p->sum = getsum(p->left) + getsum(p->right) + p->x;
	if (p->x == 0)
		p->seg = getseg(p->left) + getseg(p->right);
	else
		p->seg = p->x + getseg(p->left) + getseg(p->right) - getrb(p->left) - getlb(p->right);
	if (p->left != nullptr)
		p->lb = getlb(p->left);
	else
		p->lb = p->x;
	if (p->right != nullptr)
		p->rb = getrb(p->right);
	else
		p->rb = p->x;
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

int main() {
	int n;
	cin >> n;
	Treap T;
	for (int i = 0; i < 1000001; ++i) {
		T.push_back(0);
	}
	for (int i = 0; i < n; ++i) {
		char c;
		int x, l;
		cin >> c >> x >> l;
		auto [T3, T4] = T.split(500000 + x + l);
		auto [T1, T2] = T3.split(500000 + x);
		T2.color(c);
		T3 = merge(T1, T2);
		T = merge(T3, T4);
		cout << T.seg() << " " << T.sum() << endl;
	}
}
