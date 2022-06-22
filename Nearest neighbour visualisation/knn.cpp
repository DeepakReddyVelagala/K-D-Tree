// program to visualise the implementation of the nearest neighbour search
// using kd tree data structure in a high dimensional data space.

// the result shows the search space along with the nearest neighbours
// and space reduced by the KD Tree's nearest neighbour algorithm

#include "knn.h"
#include "ui_knn.h"

#include <iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>

#include<fstream>
#include<sstream>

#define dim 5

using namespace std;
typedef struct Node
{
    float key[dim];
    struct Node* left;
    struct Node* right;
    struct Node* p;
    int depth;
    int side;
    int color;
}node;


typedef struct
{
    node* root;
}KDTree;



void initializepoint(node* z)
{
    for (int i = 0; i < dim; i++) {
        z->key[i] = 0;
    }
    z->depth = 0;
    z->side = 0;
    z->color = 0;
    z->left = NULL;
    z->right = NULL;
    z->p = NULL;
}


float distfbox(node* boxOwner, node* origin)
{
    float d{ 0 };
    int index{ (boxOwner->depth) - 1 };

    if (boxOwner->depth != 0)
    {
        if (boxOwner->side == 1)
        {
            d = (boxOwner->p)->key[index % dim] - origin->key[index % dim];
        }

        else if (boxOwner->side == -1)
        {
            d = origin->key[index % dim] - (boxOwner->p)->key[index % dim];
        }
    }

    // for root node
    else
    {
        d = 0;
    }

    // present in the box
    if (d <= 0)
    {
        return 0;
    }

    // At a distance d from the box
    else
    {
        return d;
    }
}



double eucDist(node* z, node* q)
{
    double sum{ 0 };

    for (int i = 0; i < dim; i++)
    {
        sum += pow((z->key[i] - (q->key[i])), 2);
    }

    return sqrt(sum);
}


double maxNormDist(node* z, node* q)
{
    double max{ 0 };

    for (int i = 0; i < dim; i++)
    {
        if (abs((z->key[i] - (q->key[i]))) > max)
        {
            max = abs((z->key[i]) - (q->key[i]));
        }
    }

    return max;
}


double manhattanDist(node* z, node* q)
{
    double sum{ 0 };

    for (int i = 0; i < dim; i++)
    {
        sum += abs((z->key[i]) - (q->key[i]));
    }

    return sum;
}



void knen(node* root, node* origin, double radius, double (*distMetric)(node*, node*))
{
    if (root != NULL && (distfbox(root, origin) <= radius))
    {
        root->color = 1;

        if (distMetric(root, origin) <= radius)
        {
            root->color = 2;
        }

        if ((origin->key[root->depth % dim] < root->key[root->depth % dim]))
        {
            if (root->left != NULL)
            {
                knen(root->left, origin, radius, distMetric);
            }

            if (root->right != NULL)
            {
                knen(root->right, origin, radius, distMetric);
            }
        }

        else
        {
            if (root->right != NULL)
            {
                knen(root->right, origin, radius, distMetric);
            }

            if (root->left != NULL)
            {
                knen(root->left, origin, radius, distMetric);
            }
        }
    }
}


knn::knn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::knn)
{
    ui->setupUi(this);
}


knn::~knn()
{
    delete ui;
}


void knn::paintEvent(QPaintEvent *final){

    QPainter painter(this);
    painter.translate(this->rect().center());
    painter.scale(1, -1);


    // loading the data from a file

    fstream datafile;
    datafile.open("D:/qtLearn/KDTree/values.csv",ios::in);
    if(!datafile){
        return;
    }

    // declaring and initialising a KD TREE

    KDTree* tree = new KDTree;
    tree->root = NULL;


    string line;
    int numData = 0;

    while (getline(datafile, line))
    {
         std::stringstream s(line);
         std::string word;
         numData++;

         node* currentNode = new node;
         initializepoint(currentNode);

         for(int i = 0 ; i < dim ; i++)
         {
             getline(s, word, ',');
             std::stringstream hostconv(word);

             float num{0};
             hostconv>>num;
             currentNode->key[i]= num;
         }

         // allocating where the node to be appended in the KD tree

         node* parent = NULL;
         node* iterator = tree->root;
         int index = 0;

         while (iterator != NULL)
         {
             parent = iterator;

             if (currentNode->key[index % dim] < iterator->key[index % dim])
             {
                 iterator = iterator->left;
             }

             else
             {
                 iterator = iterator->right;
             }

             // as only one of the component of a node is compared at
             // each level, increase index to compare next component in
             // next level.

             index++;
         }

         currentNode->depth = index;

         index--;

         currentNode->p = parent;

         if (parent == NULL)
         {
             tree->root = currentNode;
         }

         else if (currentNode->key[index % dim] < parent->key[index % dim])
         {
             parent->left = currentNode;
             currentNode->side = -1;
         }

         else
         {
             parent->right = currentNode;
             currentNode->side = 1;
         }
     }


    node* origin = new node;
    initializepoint(origin);
    origin->key[0] = 3;
    origin->key[1] = 7;
    origin->key[2] = 5;
    origin->key[3] = 0;
    origin->key[4] = 9;

    QPen paintpen(Qt::red);
    paintpen.setWidth(6);
    painter.setPen(paintpen);
    painter.drawEllipse(QRect(origin->key[0]*20,origin->key[1]*20, 5, 5));


    int radius{7};


    // knen(tree->root, origin, radius, eucDist);
    // knen(tree->root, origin, radius, maxNormDist);
    knen(tree->root, origin, radius, manhattanDist);

    node** s = new node* [200];
    int count = 0;

    node* curr = tree->root;
    while(curr != NULL || count !=0)
    {
         while(curr != NULL)
         {
             s[count] = curr;
             count++;
             curr = curr->left;
         }

         curr = s[count-1];
         count--;

         if(curr->color ==0)
         {
             QPen paintpen(Qt::black);
             paintpen.setWidth(6);
             painter.setPen(paintpen);
             painter.drawPoint(curr->key[0]*20,curr->key[1]*20);
         }

         else if(curr->color  == 1)
         {
             QPen paintpen(Qt::cyan);
             paintpen.setWidth(6);
             painter.setPen(paintpen);
             painter.drawPoint(curr->key[0]*20,curr->key[1]*20);
         }

         else if(curr->color == 2)
         {
             QPen paintpen(Qt::green);
             paintpen.setWidth(6);
             painter.setPen(paintpen);
             painter.drawPoint(curr->key[0]*20,curr->key[1]*20);
         }

         curr = curr->right;
     }

     delete[](s);
     delete(origin);
     delete(tree);
     datafile.close();
}

