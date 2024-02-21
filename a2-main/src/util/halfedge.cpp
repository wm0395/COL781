#include "halfedge.hpp"

void Vertex::traverse(void (*func)(Face *face)){
    HalfEdge *he = halfedge;
    bool bdd = false;
    do{
        Face *face = he->left;
        func(face);
        //check for boundary
        if(he->next->pair){
            he = he->next->pair;
        }
        else{
            bdd = true;
            break;
        }

    }while(he != halfedge);

    he = halfedge->pair;
    // reverse traversal for opposite of boundary
    while(he && bdd){
        Face *face = he->left;
        func(face);
        he = he->next->next->pair;
    }
}

void Face::traverse(void (*func)(Vertex *vertex)){
    HalfEdge *he = halfedge;
    do{
        Vertex *vertex = he->head;
        func(vertex);
        he = he->next;

    }while(he != halfedge);    
}