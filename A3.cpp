#include<iostream>
#include<fstream>	// in out streams
#include<ctime>	// for seed for random node movement

using namespace std;

// tree struct
// contains children pointers, data, count
typedef struct tree *tree_ptr;
struct tree
{
	int data;
	int count = 0;
	tree_ptr left_child = NULL;
	tree_ptr right_child = NULL;
};

// stack struct for print function
// contains pointer data, next pointer
typedef struct astack *stack_ptr;
struct astack
{
	tree_ptr data;
	stack_ptr next;
};

//	inserts new node into tree that stems from root
//	returns whether function call inserted node, allows for accurate debugging
//	Pre: outfile opened, value assigned to store, root initialized
//	Post: node inserted into tree or existing node incremented
bool insert_node(ofstream &outfile, int value, tree_ptr &root)
{
	if (root == NULL)
	{	// if empty tree stemming from current node "root"
		//	allocates space, increments count, assigns value
		root = new tree;
		root->data = value;
		root->count++;
		return true;	//	yes, this call allocated new node
	}
	else if (value == root->data)
	{	//	if current node data equals value
		root->count++;	//	increment 1
		cout << value << " is already in the tree, count incremented\n";
		outfile << value << " is already in the tree, count incremented\n";
		return false;	//	no new allocation
	}
	//	not equal
	else
	{
		if (value < root->data)
		{	//	if value less than current data
			//	insert value into left subtree
			if (insert_node(outfile, value, root->left_child))
			{	//	if call allocated new node
				//	display
				cout << value << " inserted in the tree as a left child, ";
				cout << " parent node is " << root->data << endl;
				outfile << value << " inserted in the tree as a left child, ";
				outfile << " parent node is " << root->data << endl;
			}
		}
		else
		{	//	if value more than current data
			//	insert value into right subtree
			if (insert_node(outfile, value, root->right_child))
			{	//	if call allocated new node
				//	display
				cout << value << " inserted in the tree as a right child, ";
				cout << " parent node is " << root->data << endl;
				outfile << value << " inserted in the tree as a right child, ";
				outfile << " parent node is " << root->data << endl;
			}
		}
	}
	return false;	//	no new allocation
}

//	deletes node associated with given data from tree that stems from root
//	returns pointer to node to keep linked or null if deleted leaf
//	Pre: outfile opened, value assigned to delete, root initialized
//	Post: node deleted, existing node count decremented, or no change if empty tree
tree_ptr delete_node(ofstream &outfile, int value, tree_ptr &root)
{
	if (root == NULL)
	{	//	if empty node, not found
		cout << value << " not found in tree\n";
		outfile << value << " not found in tree\n";
		return root;
	}
	if (value < root->data)	//	if value is less
		root->left_child = delete_node(outfile, value, root->left_child);	//	delete from left subtree
	else if (value > root->data)	//	if value is more
		root->right_child = delete_node(outfile, value, root->right_child);	//	delete from right subtree
	else if (value == root->data)
	{	//	if value found
		if (root->count > 1)
		{	//	if count more than 1
			root->count--;	//	decrement count
			cout << value << " deleted by decrementing count\n";
			outfile << value << " deleted by decrementing count\n";
			return root;
		}
		else if (root->left_child && root->right_child)
		{	//	if node has both children
			//	randomly select to replace node highest of left subtree
			//	or lowest of right subtree
			if (rand() % 2)
			{	//	highest of right subtree
				tree_ptr oldptr = root->left_child;	//	start on left child
				while (oldptr->right_child)	//	find most right leaf
					oldptr = oldptr->right_child;
				int temp = root->data;	//	store data to delete
				root->data = oldptr->data;	//	replace node data with left's rightmost child
				oldptr->data = temp;	//	data to delete becomes new left's rightmost
				root->left_child = delete_node(outfile, oldptr->data, root->left_child);
				//	delete data from left subtree, return as new left child
			}
			else
			{	//	lowest of right subtree
				tree_ptr oldptr = root->right_child;	//	start on right child
				while (oldptr->left_child)	//	find most left leaf
					oldptr = oldptr->left_child;
				int temp = root->data;	//	store data to delete
				root->data = oldptr->data;	//	replace node data with right's leftmost child
				oldptr->data = temp;	//	data to delete becomes new right's leftmost
				root->right_child = delete_node(outfile, oldptr->data, root->right_child);
				//	delete data from right subtree, return as new right child
			}
		}
		else if (root->left_child)
		{	//	if node has left child only
			root = root->left_child;	//	replace root data with child's
			cout << value << " deleted from the tree\n";
			outfile << value << " deleted from the tree\n";
			return root;
		}
		else if (root->right_child)
		{	//	if node has right child only
			root = root->right_child;	//	replace root data with child's
			cout << value << " deleted from the tree\n";
			outfile << value << " deleted from the tree\n";
			return root;
		}
		else
		{	//	if root has no children
			delete root;	//	delete node
			cout << value << " deleted from the tree\n";
			outfile << value << " deleted from the tree\n";
			return NULL;	//	node is now null
		}
	}
	return root;
}

//	prints tree from root using stack
//	Pre: tree pointer is initialized
//	Post: contents of tree, if any, are printed
void print_tree(ofstream &outfile, const tree_ptr &root)
{
	if (root != NULL)
	{	//	if not empty
		cout << "Number\t\tCount\n";
		outfile << "Number\t\tCount\n";
		tree_ptr current = root;	//	start from root
		stack_ptr print_stack = NULL;	//	initialize stack pointer to null to indicate empty

		while (current != NULL || print_stack != NULL)
		{	//	while current is a node and stack isn't empty
			while (current != NULL)
			{	//	while current is a node
				stack_ptr newptr = new astack;	//	new stack node
				if (print_stack == NULL)
				{	//	if empty, initialize stack
					print_stack = new astack;
					newptr->data = current;	//	push current data into stack
					newptr->next = NULL;
					print_stack = newptr;
				}
				else
				{	//	stack as values
					newptr->data = current;	//	push current data into stack
					newptr->next = print_stack;
					print_stack = newptr;
				}
				current = current->left_child;	//	current = next
			}
			current = print_stack->data;	//	current = top of stack
			stack_ptr old = print_stack;	//	pop stack
			print_stack = print_stack->next;
			delete old;
			cout << current->data << "\t\t" << current->count << endl;	//	print previous top of stack
			outfile << current->data << "\t\t\t" << current->count << endl;
			current = current->right_child;	//	current = right
		}
	}
	else
	{	//	empty
		cout << "Cannot print: tree is empty.\n";
		outfile << "Cannot print: tree is empty.\n";
	}
}

int main()
{
	srand(time(0));	//	provides seed for randomness
	ifstream infile;	//	open A3.DAT
	infile.open("A3.DAT");
	if(infile.fail())
	{
		cout << "Infile opening failed!\n";
		system("pause");
		exit(1);
	}
	ofstream outfile;	//	open A3.txt
	outfile.open("A3.txt");
	if(outfile.fail())
	{
		cout << "Outfile opening failed!\n";
		system("pause");
		exit(1);
	}
	tree_ptr root = NULL;	//	initialize root pointer to null
	//	variables for file read
	char opcode;
	int value;
	while(infile >> opcode >> value)
	{	//	while input exists
		switch (opcode)
		{
		case 'I':	//	if I, insert
			if (insert_node(outfile, value, root))
			{	//	if inserting root value
				cout << value << " inserted in the tree as a root\n";
				outfile << value << " inserted in the tree as a root\n";
			}
			break;
		case 'D':	//	if D, delete
			delete_node(outfile, value, root);
			break;
		default:	//	not an op
			cout << "Invalid operation!\n";
			outfile << "Invalid operation!\n";
		}
	}
	print_tree(outfile, root);	//	print tree from root
	//	close files
	infile.close();
	outfile.close();
	system("pause");
	return 0;
}