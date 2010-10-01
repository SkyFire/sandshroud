#include "MMapCommon.h"

#include "Collision/MapTree.h"
#include "Collision/VMapManager2.h"
#include "Collision/WorldModel.h"
#include "Collision/ModelInstance.h"

namespace VMAP
{
    // Need direct access to encapsulated VMAP data, so we add functions for MMAP generator
    // maybe add MapBuilder as friend to all of the below classes would be better?

    // declared in src/shared/vmap/MapTree.h
    void StaticMapTree::getModelInstances(ModelInstance* &models, uint32 &count)
    {
        models = iTreeValues;
        count = iNTreeValues;
    }

    // declared in src/shared/vmap/VMapManager2.h
    void VMapManager2::getInstanceMapTree(InstanceTreeMap &instanceMapTree)
    {
        instanceMapTree = iInstanceMapTrees;
    }

    // declared in src/shared/vmap/WorldModel.h
    void WorldModel::getGroupModels(vector<GroupModel> &groupModels)
    {
        groupModels = this->groupModels;
    }
    
    // declared in src/shared/vmap/WorldModel.h
    void GroupModel::getMeshData(vector<Vector3> &vertices, vector<MeshTriangle> &triangles)
    {
        vertices = this->vertices;
        triangles = this->triangles;
    }

    // declared in src/shared/vmap/ModelInstance.h
    WorldModel* const ModelInstance::getWorldModel()
    {
        return this->iModel;
    }
}