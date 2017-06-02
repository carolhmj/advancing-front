#include "geometry.h"
#include <set>
#include <iostream>

using namespace Geometry;
using std::cout;
using std::endl;

Geometry::Vertex::Vertex(size_t id, glm::vec3 position, Geometry::WEdge* iedge) : id(id), position(position), iedge(iedge) {}

bool WEdge::shouldRemove()
{
    return (visits == 1 && type == WEdgeType::BORDER) || (visits == 2 && (type == WEdgeType::INTERNAL || WEdgeType == RESTRICTION));
}

Geometry::WEdge::WEdge(size_t id) : id(id) {}

Geometry::WEdge::WEdge(size_t id, Vertex* vstart, Vertex* vend, Loop* cwloop, Loop* ccwloop, WEdge* cwpred, WEdge* cwsucc, WEdge* ccwpred, WEdge* ccwsucc) : id(id), vstart(vstart), vend(vend), cwloop(cwloop), ccwloop(ccwloop), cwpred(cwpred), cwsucc(cwsucc), ccwpred(ccwpred), ccwsucc(ccwsucc) {}

Geometry::Loop::Loop(size_t id, WEdge* iedge) : id(id), iedge(iedge) {}

std::vector<Geometry::WEdge*> Geometry::Vertex::adjedge() {
    WEdge* curredge = iedge;
    std::vector<WEdge*> adjedgev;

    do {
        adjedgev.push_back(curredge);
        //Checa se é vstart ou vend
        if (curredge->vstart == this) {
            curredge = curredge->ccwpred;
        } else {
            curredge = curredge->cwpred;
        }

    } while (curredge != iedge);

    return adjedgev;
}
std::vector<Geometry::Vertex*> Geometry::Vertex::adjvertex() {
    WEdge* curredge = iedge;
    std::vector<Vertex*> adjvertexv;

    do {
        //Checa se é vstart ou vend
        if (curredge->vstart == this) {
            curredge = curredge->ccwpred;
            adjvertexv.push_back(curredge->vend);
        } else {
            curredge = curredge->cwpred;
            adjvertexv.push_back(curredge->vend);
        }

    } while (curredge != iedge);

    return adjvertexv;
}
std::vector<Geometry::Loop*> Geometry::Vertex::adjloop() {
    WEdge* curredge = iedge;
    std::vector<Loop*> adjloopv;
    //Como a gente pode encontrar faces repetidas, adicionamos em um set para
    //garantir que são únicas
    auto less = [](auto* a, auto* b){return a->id < b->id;};
    std::set<Loop*,decltype(less)> adjloops(less);
    do {

        adjloops.insert(curredge->ccwloop);
        adjloops.insert(curredge->cwloop);

        //Checa se é vstart ou vend
        if (curredge->vstart == this) {
            curredge = curredge->ccwpred;
        } else {
            curredge = curredge->cwpred;
        }

    } while (curredge != iedge);

    //Copia os valores para um vetor
    std::copy(adjloops.begin(), adjloops.end(), std::back_inserter(adjloopv));

    return adjloopv;
}

std::vector<Geometry::WEdge*> Geometry::Loop::adjedge() {
    WEdge* curredge = iedge;
    std::vector<WEdge*> adjedge;
    auto less = [](auto* a, auto* b){return a->id < b->id;};
    std::set<WEdge*,decltype(less)> adjedgev(less);

    // Qual loop eu sou?
    bool ccwtravel = (iedge->ccwloop->id == this->id);
    bool og_ccwtravel = ccwtravel;

    do {
        printf("%zu %s ", curredge->id,ccwtravel?"t":"f");
        adjedgev.insert(curredge);
        if (ccwtravel) {
            if ((curredge->vstart->id == curredge->ccwsucc->vstart->id) || (curredge->vend->id == curredge->ccwsucc->vend->id))
                ccwtravel = !ccwtravel;
            curredge = curredge->ccwsucc;
            printf("%zu %s\n", curredge->id,ccwtravel?"t":"f");
        }
        else {
            if ((curredge->vstart->id == curredge->cwsucc->vstart->id) || (curredge->vend->id == curredge->cwsucc->vend->id))
                ccwtravel = !ccwtravel;
            curredge = curredge->cwsucc;
            printf("%zu %s\n", curredge->id,ccwtravel?"t":"f");
        }
    } while (curredge->id != iedge->id || og_ccwtravel != ccwtravel);

    printf("%zu\n\n",id);
    std::copy(adjedgev.begin(), adjedgev.end(), std::back_inserter(adjedge));
    return adjedge;
}

std::vector<Geometry::Vertex*> Geometry::Loop::adjvertex() {
    std::vector<Vertex*> adjvertexv;
    for (auto&& e : adjedge()) {
        adjvertexv.push_back(e->vstart);
        if(e->vend) adjvertexv.push_back(e->vend);
    }

    return adjvertexv;
}

std::vector<Geometry::Loop*> Geometry::Loop::adjloop() {

    WEdge* curredge = iedge;
    std::vector<Loop*> adjloopv;

    do {
        cout << "curredge: " << curredge->id << endl;

        //Primeiro, descobre se é ccwloop ou cwloop,
        if (curredge->ccwloop == this) {
            adjloopv.push_back(curredge->cwloop);
            curredge = curredge->ccwsucc;
        } else {
            adjloopv.push_back(curredge->ccwloop);
            curredge = curredge->cwsucc;
        }

    } while (curredge != iedge);

    return adjloopv;
}


std::vector<WEdge*> Geometry::WEdge::adjedge() {
    std::vector<WEdge*> adjedgev;

    adjedgev.push_back(cwpred);
    adjedgev.push_back(cwsucc);
    adjedgev.push_back(ccwpred);
    adjedgev.push_back(ccwsucc);

    return adjedgev;
}
std::vector<Vertex*> Geometry::WEdge::adjvertex() {
    std::vector<Vertex*> adjvertexv;

    adjvertexv.push_back(vstart);
    adjvertexv.push_back(vend);

    return adjvertexv;
}
std::vector<Loop*> Geometry::WEdge::adjloop() {
    std::vector<Loop*> adjloopv;

    adjloopv.push_back(cwloop);
    adjloopv.push_back(ccwloop);

    return adjloopv;
}
