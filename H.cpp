#include<iostream>
#include<vector>
#include<random>
#include<ctime>

using namespace std;

mt19937 rnd(time(0));

struct Node {
	char x;
	int y, z, mod;
	Node* left;
	Node* right;
	Node(char x1 = 0, int y1 = rnd(), int z1 = 1, Node* left1 = nullptr, Node* right1 = nullptr) {
		x = x1, y = y1, z = z1, mod = 0, left = left1, right = right1;
	}
};

void push(Node* p) {
	if (p == nullptr)
		return;
	if (p->mod == 1) {
       	        Node* temp = p->left;
              	p->left = p->right, p->right = temp;
        }
      	if (p->left != nullptr)
                p->left->mod ^= p->mod;
      	if (p->right != nullptr)
              	p->right->mod ^= p->mod;
	p->mod = 0;
}

int getsize(Node* a) {
	return a == nullptr ? 0 : a->z;
}

void print_n (Node* a) {
	push(a);
        if (a != nullptr) {
    	        print_n (a->left);
             	cout << a->x;
                print_n (a->right);
       	}
}

void update (Node* p) {
   	p->z = getsize(p->left) + getsize(p->right) + 1;
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
	void flip() {
		root->mod ^= 1;
	}
};

Treap merge (Treap T1, Treap T2) {
        Treap T(merge_n(T1.root, T2.root));
        return T;
}

int main() {
	int n, q;
	cin >> n >> q;
	Treap T;
	for (int i = 0; i < n; ++i) {
		char x;
		cin >> x;
		T.push_back(x);
	}
	int s = 0;
	for (int i = 0; i < q; ++i) {
		int l, r;
		cin >> l >> r;
		l = (l - 1 + n - s) % n + 1, r = (r - 1 + n - s) % n + 1;
		if (l <= r) {
			auto [T3, T4] = T.split(r);
			auto [T1, T2] = T3.split(l - 1);
                        T2.flip();
			T3 = merge(T1, T2);
			T = merge (T3, T4);
		}
		else {
			auto [T3, T4] = T.split(l - 1);
                        auto [T1, T2] = T3.split(r);
			T3 = merge(T4, T1);
			T3.flip();
                        T = merge (T2, T3);
			s = (s + r) % n;
		}
	}
	auto [T1, T2] = T.split(n - s);
	T2.print();
	T1.print();
	cout << endl;
}
