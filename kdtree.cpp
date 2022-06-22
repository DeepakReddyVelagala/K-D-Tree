// K-d-Trees in Nearest Neighbour Searches.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#define dim 5

using namespace std;
typedef struct Node {
    float key[dim];
    struct Node* left;
    struct Node* right;
    struct Node* p;
    int depth;
    int side;
}node;

typedef struct {
    node* root;
}KDTree;

void initializePoint(node* z) {  
    for (int i = 0; i < dim; i++) {
        z->key[i] = ((float)pow(-1, rand()%10))*((float)(rand() % 100)/10);
    }
    z->depth = 0;
    z->side = 0;
    z->left = NULL;
    z->right = NULL;
    z->p = NULL;
}

float distanceFromHyperplane(node* z, node* q) {
    float d{ 0 };
    int index{ (z->depth) - 1 };
    if (z->depth != 0) {
        if (z->side == 1) {
            d = (z->p)->key[index % dim] - q->key[index % dim];
        }
        else if (z->side == -1) {
            d = q->key[index % dim] - (z->p)->key[index % dim];
        }
    }
    else {
        d = 0;
    }
    if (d <= 0) {
        return 0;
    }
    else {
        return d;
    }
}

double euclideanDist(node* z, node* q) {
    double sum{ 0 };
    for (int i = 0; i < dim; i++) {
        sum += pow((z->key[i] - (q->key[i])), 2);
    }
    return sqrt(sum);
}

double maxNormDist(node* z, node* q) {
    double max{ 0 };
    for (int i = 0; i < dim; i++) {
        if (abs((z->key[i] - (q->key[i]))) > max) {
            max = abs((z->key[i]) - (q->key[i]));
        }
    }
    return max;
}

double manhattanDist(node* z, node* q) {
    double sum{ 0 };
    for (int i = 0; i < dim; i++) {
        sum += abs((z->key[i]) - (q->key[i]));
    }
    return sum;
}

void printPoint(node* z) {
    cout << "(";
    for (int i = 0; i < dim; i++) {
        cout << z->key[i];
        if (i != dim - 1) {
            cout << ",";
        }
    }
    cout << ")" << " ";
}

void buildRandomKDt(KDTree* bst, int num) {
    while (num--) {
        node* z = new node;
        initializePoint(z);
        printPoint(z);
        if (num % 3 == 0)
        {
            cout << endl; 
        }
        node* y = NULL;
        node* x = bst->root;
        int index = 0;
        while (x != NULL) {
            y = x;
            if (z->key[index % dim] < x->key[index % dim]) {
                x = x->left;
            }
            else {
                x = x->right;
            }
            index++;
        }
        z->depth = index;
        index--;
        z->p = y;
        if (y == NULL) {
            bst->root = z;
        }
        else if (z->key[index % dim] < y->key[index % dim]) {
            y->left = z;
            z->side = -1;
        }
        else {
            y->right = z;
            z->side = 1;
        }
    }
}

void inOrderTreeWalk(node* x) {
    if (x != NULL) {
        inOrderTreeWalk(x->left);
        printPoint(x);
        inOrderTreeWalk(x->right);
    }
}

typedef struct {
    double min;
    node* point;
}nearestNeighbour;

void nearestNeighbourSearch(node* z, node* q, nearestNeighbour* near) {
    if (z != NULL && (distanceFromHyperplane(z, q) <= near->min)) {
        if (near->min >= euclideanDist(z, q)) {
            near->min = euclideanDist(z, q);
            near->point = z;
        }
        if ((q->key[z->depth % dim] < z->key[z->depth % dim])) {
            if (z->left != NULL) {
                nearestNeighbourSearch(z->left, q, near);
            }
            if (z->right != NULL) {
                nearestNeighbourSearch(z->right, q, near);
            }
        }
        else {
            if (z->right != NULL) {
                nearestNeighbourSearch(z->right, q, near);
            }
            if (z->left != NULL) {
                nearestNeighbourSearch(z->left, q, near);
            }
        }
    }
}

void neighboursUnderRadius(node* z, node* q, double radius,double (*distMetric)(node*, node*)){
    if (z != NULL && (distanceFromHyperplane(z, q) <= radius)) {
        if (distMetric(z, q) <= radius) {
            printPoint(z);
        }
        if ((q->key[z->depth % dim] < z->key[z->depth % dim])) {
            if (z->left != NULL) {
                neighboursUnderRadius(z->left, q, radius,distMetric);
            }
            if (z->right != NULL) {
                neighboursUnderRadius(z->right, q, radius,distMetric);
            }
        }
        else {
            if (z->right != NULL) {
                neighboursUnderRadius(z->right, q, radius, distMetric);
            }
            if (z->left != NULL) {
                neighboursUnderRadius(z->left, q, radius, distMetric);
            }
        }
    }
}

int main()
{   //srand((unsigned)time(0));

    cout << "Building a KD Tree\n";
    int num{ 200 };
    KDTree* tree = new KDTree;
    tree->root = NULL;
    buildRandomKDt(tree, num);

    //cout << endl << "Inorder traversal of the KD Tree" << endl;
    // inOrderTreeWalk(tree->root);
    node* q = new node;
    initializePoint(q);

    cout << "\nNearest neighbour of the point ";
    printPoint(q);
    cout << ":" << endl;
    nearestNeighbour* near = new nearestNeighbour;
    near->point = NULL;
    near->min = 40;
    nearestNeighbourSearch(tree->root, q, near);
    printPoint(near->point);

    double radius{40};
    cout << "\n\nNeighbours of";
    printPoint(q);
    cout<<"Under radius "<<radius<<":"<< endl;
    cout << "\nUsing the Euclidean Metric, the neighbors generated are : " << endl;
    neighboursUnderRadius(tree->root, q, radius, euclideanDist);
    
 
    cout<< "\n\nUsing the Max Norm metric, the neighbors generated are : " << endl;
    neighboursUnderRadius(tree->root, q, radius, maxNormDist);

    cout<<"\n\nUsing the Manhattan Metric, the neighbors generated are : " << endl;
    neighboursUnderRadius(tree->root, q, radius, manhattanDist);
    cout << endl;
    delete(q);
    delete(tree);
    delete(near);
}
