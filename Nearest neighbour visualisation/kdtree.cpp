// program to pictorially represent the given high dimensional data
// as a projection in 2D plane

#include "kdtree.h"
#include "ui_kdtree.h"

#include <iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>

#include <fstream>
#include <sstream>

// Here the dimension of data is 5
#define dim 5

// defining a node in KD Tree
typedef struct Node
{
    float key[dim];
    struct Node* left;
    struct Node* right;
    struct Node* p;
    int depth;
    int side;
    int color;// to store color of node
}node;


// defining KD Tree with its root node pointer
typedef struct
{
    node* root;
}KDTree;


void initializePoint(node* z)
{
    /*
       parameters:
       z : node pointer

       working:
       This function initializes a
       given node.

       return:
       void
    */
    for (int i = 0; i < dim; i++)
    {
        z->key[i] = 0;
    }
    z->depth = 0;
    z->side = 0;
    z->color = 0;
    z->left = NULL;
    z->right = NULL;
    z->p = NULL;
}


kdtree::kdtree(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kdtree)
{
    ui->setupUi(this);
}


kdtree::~kdtree()
{
    delete ui;
}


void kdtree::on_pushButton_clicked()
{
    neigh = new knn(this);
    neigh->show();
}


void kdtree::paintEvent(QPaintEvent *point)
{
    QPainter painter(this);
    painter.translate(this->rect().center());
    painter.scale(1, -1);

    // loading the data from a file

    std::fstream datafile;
    datafile.open("D:/qtLearn/KDTree/values.csv",std::ios::in);
    if(!datafile){
        return;
    }


    // declaring and initialising a KD TREE

    KDTree* tree = new KDTree;
    tree->root = NULL;

    std::string line;

    // for the sake of understanding of how colors are
    // being allocated to a node we constructing the KD Tree
    // along with all data given as nodes even though this does'nt
    // make any difference in the visual representation of data

    while (getline(datafile, line))
    {
         std::stringstream s(line);
         std::string word;

         node* currentNode = new node;
         initializePoint(currentNode);

         for(int i = 0 ; i < dim ; i++)
         {
             getline(s, word, ',');
             std::stringstream hostconv(word);

             float num{0};
             hostconv>>num;
             currentNode->key[i]= num;
         }

         if(currentNode->color == 0)
         {
            QPen paintpen(Qt::black);
            paintpen.setWidth(6);
            painter.setPen(paintpen);

            // only drawing first two coordinates
            // as this is a projection in 2D plane

            painter.drawPoint(currentNode->key[0]*20, currentNode->key[1]*20);
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

     datafile.close();
     delete(tree);

}

