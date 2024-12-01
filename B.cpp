#include<iostream>
#include<vector>
#include<random>
#include<ctime>

using namespace std;

mt19937 rnd(time(0));

struct Node {
	int x, y, sz, sum;
	bool nul;
	bool pos;
	Node* left;
	Node* right;
	Node(int x1 = 0, int y1 = rnd(), int sz1 = 1, Node* left1 = nullptr, Node* right1 = nullptr) {
		x = x1, y = y1, sz = sz1, sum = x1, nul = (x1 == 0), pos = (x1 != 0), left = left1, right = right1;
	}
};

int getsum(Node* a) {
	return a == nullptr ? 0 : a->sum;
}

int getsize(Node* a) {
	return a == nullptr ? 0 : a->sz;
}

bool getnul(Node* a) {
	return a == nullptr ? 0 : a->nul;
}

bool getpos(Node* a) {
        return a == nullptr ? 0 : a->pos;
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
	p->nul = getnul(p->left) || getnul(p->right) || (p->x == 0);
	p->pos = getpos(p->left) || getpos(p->right) || (p->x != 0);
}

pair<Node*, Node*> split_n(Node*& p, int k) {
	if (p == nullptr) {
		return {nullptr, nullptr};
	}
	if (getsize(p->left) >= k) {
		auto [a, b] = split_n(p->left, k);
		p->left = b;
		update(p);
		return {a, p};
	}
	auto [a, b] = split_n(p->right, k - getsize(p->left) - 1);
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

int findnul_n(Node* a, int k = 0) {
	if (getnul(a->left))
		return findnul_n(a->left, k);
	else if (a->x == 0)
		return getsize(a->left) + k + 1;
	else
		return findnul_n(a->right, k + getsize(a->left) + 1);
}

int findpos_n(Node* a, int k = 0) {
	if (getpos(a->right))
                return findpos_n(a->right, k + getsize(a->left) + 1);
        else if (a->x != 0)
                return getsize(a->left) + k + 1;
        else
                return findpos_n(a->left, k);
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
	int findnul() {
		return findnul_n(root);
	}
	int findpos() {
                return findpos_n(root);
        }
};

Treap merge (Treap T1, Treap T2) {
        Treap T(merge_n(T1.root, T2.root));
        return T;
}

int main() {
	int n, m;
	cin >> n >> m;
	Treap T;
	for (int i = 0; i < n + m + 2; ++i)
		T.push_back(0);
	for (int i = 0; i < n; ++i) {
		int l;
		cin >> l;
		Node* p = new Node(i + 1);
		auto [T3, T4] = T.split(l);
		auto [T1, T2] = T3.split(l - 1);
		if (T2.root->nul) {
			T2 = Treap(p);
			T3 = merge(T1, T2);
			T = merge(T3, T4);
		}
		else {
			T4 = merge(T2, T4);
			auto [T5, T6] = T4.split(T4.findnul());
			auto [T7, T8] = T5.split(T5.size() - 1);
			T5 = merge(Treap(p), T7);
			T4 = merge(T5, T6);
			T = merge(T1, T4);
		}
	}
	auto [T1, T2] = T.split(T.findpos());
	cout << T1.size() << endl;
	T1.print();
	cout << endl;
}
