#include<iostream>
#include<algorithm>
#include<random>
#include<ctime>

using namespace std;

mt19937 rnd(time(0));

struct Node {
	int x, y, sz;
	long long sum;
	Node* left;
	Node* right;
	Node(int x1 = 0, int y1 = rnd(), int sz1 = 1, Node* left1 = nullptr, Node* right1 = nullptr) {
		x = x1, y = y1, sz = sz1, sum = x1, left = left1, right = right1;
	}
};

long long getsum(Node* a) {
	return a == nullptr ? 0 : a->sum;
}

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
	p->sum = getsum(p->left) + getsum(p->right) + p->x;
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
	long long sum() {
		return getsum(root);
	}
};

Treap merge (Treap T1, Treap T2) {
        Treap T(merge_n(T1.root, T2.root));
        return T;
}

int main() {
	for (int i = 0; i < 100001; ++i) {
		int n, m;
		cin >> n >> m;
		if (n == 0)
			return 0;
		cout << "Swapper " << i + 1 << ":" << endl;
		Treap T_odd, T_even;
		for (int i = 0; i < n; ++i) {
			int x;
			cin >> x;
			if (i % 2)
				T_even.push_back(x);
			else
				T_odd.push_back(x);
		}
		for (int i = 0; i < m; ++i) {
			int f, l, r;
			cin >> f >> l >> r;
			auto [T3_odd, T4_odd] = T_odd.split((r + 1) / 2);
			auto [T1_odd, T2_odd] = T3_odd.split(l / 2);
			auto [T3_even, T4_even] = T_even.split(r / 2);
                	auto [T1_even, T2_even] = T3_even.split((l - 1) / 2);
			if (f == 1) 
				swap(T2_odd, T2_even);
			else
				cout << T2_odd.sum() + T2_even.sum() << endl;
			T3_odd = merge(T1_odd, T2_odd);
			T_odd = merge(T3_odd, T4_odd);
			T3_even = merge(T1_even, T2_even);
			T_even = merge(T3_even, T4_even);
		}
		cout << endl;
	}
}



